#ifndef DEBUG_H
#define DEBUG_H

#include <vector>
#include <string>

struct trace_func_info_t
{
    std::string func_name;
    std::string file_loc;
    void * traced_address;
    void * offset;
};

struct vec_func_info_t
{
    std::string func_name;
    void * _start;
    void * _end;
};

typedef std::vector < trace_func_info_t > trace_info_t;
typedef std::vector < vec_func_info_t > vector_symbol_table_t;

// initialize debug symbol tracer
void initialize_symbol_vector(const char * filename);

// clean addr2line output
std::string clean_addr2line_output(const std::string& input);

// find function demangled C++ name by address
std::string find_function_in_map(void * address);

// check if addr2line is available
bool check_addr2line_avail();

// execute a command and get its output
std::string exec_cmd(const std::string& cmd);

#endif //DEBUG_H
