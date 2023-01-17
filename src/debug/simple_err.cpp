#include <simple_err.h>
#include <execinfo.h>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <cstring>

#define ERROR_SWITCH_CASE(val_name) case val_name##_CDX : output << val_name##_ERR_TEXT; break

#define BACKTRACE_SZ 64    /* Max stack frame size */

struct addr_pack_t
{
    uint64_t runtime_addr;
    uint64_t offset;
    std::string file_loc;
};

// get address from stack frame raw output
addr_pack_t get_addr_from_stack_frame(const std::string& input)
{
    std::string output_ad, output_sq, output_floc;
    bool start = false;
    addr_pack_t ret { };

    // extract file location
    ///////////////////////////
    for (auto i: input)
    {
        if (i == '(')
        {
            break;
        }

        output_floc += i;
    }

    ret.file_loc = output_floc;

    // extract [*]
    ///////////////////////////
    for (auto i: input)
    {
        if (i == '[')
        {
            start = true;
            continue;
        }

        if (start)
        {
            if (i != ']')
            {
                output_ad += i;
            }
            else
            {
                break;
            }
        }
    }

    if (!output_ad.empty())
    {
        ret.runtime_addr =  strtoll(output_ad.c_str(), nullptr, 16);
    }
    else
    {
        ret.runtime_addr = 0x00;
    }

    ///////////////////////////

    // extract (+*)
    start = false;

    for (auto i: input)
    {
        if (i == '+')
        {
            start = true;
            continue;
        }

        if (start)
        {
            if (i != ')')
            {
                output_sq += i;
            }
            else
            {
                break;
            }
        }
    }

    if (!output_sq.empty())
    {
        ret.offset =  strtoll(output_sq.c_str(), nullptr, 16);
    }
    else
    {
        ret.offset = 0x00;
    }

    return ret;
}

template < typename Type >
std::string ltostr(Type num)
{
    char buff[32] { };
    sprintf(buff, "%lX", (uint64_t)num);
    return buff;
}

simple_error_t::simple_error_t(uint64_t _error_code, const std::string& extra)
{
    // Automatically obtain stack frame
    void *_array[BACKTRACE_SZ];
    int _size, _i;
    char **_strings;
    _size = backtrace (_array, BACKTRACE_SZ);
    _strings = backtrace_symbols (_array, _size);

    if (_strings != nullptr)
    {
        for (_i = 0; _i < _size; _i++)
        {
            auto frame_address = get_addr_from_stack_frame(_strings[_i]);
            auto func_name = find_function_in_map(reinterpret_cast<void *>(frame_address.runtime_addr));
            _trace_info.emplace_back(trace_func_info_t { .func_name = func_name,
                                                         .file_loc = frame_address.file_loc,
                                                         .traced_address = (void*)frame_address.runtime_addr,
                                                         .offset = (void*)frame_address.offset
                                                        });
        }
        free (_strings);
    }
    std::reverse(_trace_info.begin(), _trace_info.end());
    if (!_trace_info.empty()) {
        _trace_info.pop_back(); // delete current trace (which is meaningless)
    }

    // save error code
    error_code = _error_code;

    // generate error message
    std::stringstream output;
    output << "Obtained stack frame:" << std::endl;
    for (const auto & i : trace_info)
    {
        output << "\t-- (" << i.traced_address << ")[+0x"
               << std::setfill('0') << std::setw(8)
               << std::hex << (uint64_t)i.offset << "]"
               << " > " << i.func_name;

        if (check_addr2line_avail())
        {
            output << ": " << clean_addr2line_output(
                    exec_cmd(
                            std::string("addr2line a -f -p -e ")
                            + i.file_loc + " "
                            + ltostr(i.traced_address)
                    )
            );
        }

        output << std::endl;
    }

    output << "Error: ";

    switch (error_code)
    {
        ERROR_SWITCH_CASE(SUCCESS);
        ERROR_SWITCH_CASE(NCURSES_INIT_FAILED);
        ERROR_SWITCH_CASE(SIGNAL_OVERRIDE_FAILED);
        ERROR_SWITCH_CASE(NO_SPACE_LEFT_ON_SCRREN);
        default :
            output << "(Unknown error)";
            break;
    }

    output << " (errno=" << strerror(errno) << ")";

    if (!extra.empty())
    {
        output << ": " << extra;
    }

    output << "\n";

    what_str = output.str();
}

const char *simple_error_t::what()
{
    return what_str.c_str();
}
