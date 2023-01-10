#include <sys/types.h>
#include <fcntl.h>
#include <libelf.h>
#include <gelf.h>
#include <unistd.h>
#include <debug.h>
#include <algorithm>
#include <cxxabi.h>
#include <iostream>

struct func_info_t
{
    std::string func_name;
    void * runtime_entry_address = nullptr;
};

typedef std::vector < func_info_t > symbol_table_t;

// get symbol table in my ELF file
symbol_table_t get_symbol_table(const char * filename);
// convert static table to runtime table
symbol_table_t convert_to_runtime_symbol_map(const symbol_table_t & symbol_table);
// convert runtime table to vector with _start and _end (ranged table)
vector_symbol_table_t map_elf_symbols(const symbol_table_t & symbol_table);
// convert mangled name to readable name
std::string demangle(const std::string & mangled_name);

vector_symbol_table_t executable_symbol_table;

void landmark () __attribute__ ((section (".text")));
void landmark () { }

template < typename Type >
int64_t calculate_distance(const Type & _a, const Type & _b)
{
    return (int64_t)_a - (int64_t)_b;
}

symbol_table_t get_symbol_table(const char * filename)
{
    Elf *       elf;
    Elf_Scn *   scn = nullptr;
    GElf_Shdr   shdr;
    Elf_Data *  data;
    int fd;
    unsigned long count;
    symbol_table_t symbol_table;

    elf_version(EV_CURRENT);

    fd = open(filename, O_RDONLY);
    elf = elf_begin(fd, ELF_C_READ, nullptr);

    while ((scn = elf_nextscn(elf, scn)) != nullptr)
    {
        gelf_getshdr(scn, &shdr);
        if (shdr.sh_type == SHT_SYMTAB)
        {
            /* found a symbol table, go print it. */
            goto _found;
        }
    }

    // Symbol table not found, exit
    std::cerr << "ELF Symbol Table initialization failed! Exiting..." << std::endl;
    exit(EXIT_FAILURE);

    // found ELF symbol
    _found:
    data = elf_getdata(scn, nullptr);
    count = shdr.sh_size / shdr.sh_entsize;

    /* print the symbol names */
    for (int i = 0; i < count; i++)
    {
        GElf_Sym sym;
        gelf_getsym(data, i, &sym);
        std::string func_name = elf_strptr(elf, shdr.sh_link, sym.st_name);

        // ignore unwanted symbols
        if ( (sym.st_value == 0) || (func_name.empty() || func_name[0] != '_') )
        {
            continue;
        }

        func_info_t table_entry;
        table_entry.func_name = func_name;
        table_entry.runtime_entry_address = reinterpret_cast<void *>(sym.st_value);
        symbol_table.emplace_back(table_entry);
    }
    elf_end(elf);
    close(fd);

    std::sort(symbol_table.begin(), symbol_table.end(),
              [](const func_info_t& a, const func_info_t& b)->bool
              {
                   return a.runtime_entry_address < b.runtime_entry_address;
              });

    return symbol_table;
}

symbol_table_t convert_to_runtime_symbol_map(const symbol_table_t & symbol_table)
{
    struct f_vec_t
    {
        int64_t offset;
        void * org_addr;
        std::string func_name;
    };

    std::vector < f_vec_t > vector_table;
    symbol_table_t new_table;
    void * landmark_table_entry;

    // find landmark
    for (const auto & i : symbol_table)
    {
        if (i.func_name == "_Z8landmarkv" )
        {
            landmark_table_entry = i.runtime_entry_address;
            break;
        }
    }

    // convert to offset table
    for (const auto & i : symbol_table)
    {
        vector_table.emplace_back(
                f_vec_t { .offset = calculate_distance(i.runtime_entry_address, landmark_table_entry),
                          .org_addr = i.runtime_entry_address,
                          .func_name = i.func_name
                });
    }

    // convert back to vector table, but in runtime addressing
    for (const auto & i : vector_table)
    {
        new_table.emplace_back(
                func_info_t {
                    .func_name = i.func_name,
                    .runtime_entry_address = (void*) (i.offset + (int64_t)landmark),
                });
    }

    return new_table;
}

vector_symbol_table_t map_elf_symbols(const symbol_table_t & symbol_table)
{
    vector_symbol_table_t vec_sym_table;

    // convert back to vector table, but in runtime addressing
    for (auto i = symbol_table.begin(); i != symbol_table.end(); i++)
    {
        if (i != (symbol_table.end() - 1))
        {
            vec_sym_table.emplace_back(vec_func_info_t { .func_name = i->func_name,
                                                         ._start = i->runtime_entry_address,
                                                         ._end = (i + 1)->runtime_entry_address,
            });
        }
        else
        {
            vec_sym_table.emplace_back(vec_func_info_t{.func_name = i->func_name,
                    ._start = i->runtime_entry_address,
                    ._end = i->runtime_entry_address,
            });
        }
    }

    return vec_sym_table;
}

std::string find_function_in_map(void * address)
{
    for (const auto & i : executable_symbol_table)
    {
        // if in range
        if (i._start <= address && i._end > address)
        {
            try {
                return demangle(i.func_name);
            } catch (std::exception & e)
            {
                return i.func_name;
            }
        }
    }

    return "(undefined)";
}

std::string demangle(const std::string & mangled_name)
{
    int status = -1;
    char *demangled_name = abi::__cxa_demangle(mangled_name.c_str(),
                                               nullptr,
                                               nullptr,
                                               &status);
    std::string ret = demangled_name;
    free(demangled_name);
    return ret;
}

void initialize_symbol_vector(const char * filename)
{
    symbol_table_t elf_symbol_table = get_symbol_table(filename);
    auto runtime_symbol_table = convert_to_runtime_symbol_map(elf_symbol_table);
    executable_symbol_table = map_elf_symbols(runtime_symbol_table);
    std::cout << "ELF table loaded. " << executable_symbol_table.size() << " entries in total." << std::endl;
}
