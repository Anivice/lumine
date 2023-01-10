#include <debug.h>
#include <cstdio>
#include <vector>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

std::vector < std::string > str2lines(const std::string& input)
{
    std::string line;
    std::vector < std::string > ret;

    for (auto i : input)
    {
        if (i == '\n')
        {
            ret.emplace_back(line);
            line.clear();
            continue;
        }

        line += i;
    }

    if (!line.empty())
    {
        ret.emplace_back(line);
    }

    return ret;
}

std::string get_src_loc(const std::string & input)
{
    std::string ret;

    auto fill3 = [](decltype(input.begin()) it,
            decltype(input.end()) end)->std::string
    {
        if (it > (end - 3))
        {
            return "";
        }

        std::string lret;
        lret += *it;
        lret += *(it + 1);
        lret += *(it + 2);

        return lret;
    };

    for (auto i = input.begin(); i != input.end(); i++)
    {
        if (fill3(i, input.end()) == "at ")
        {
            for (auto start = i + 3; start != input.end(); start++)
            {
                ret += *start;
            }
        }
    }

    if (ret == "??:?")
    {
        ret = "(no info)";
    }

    return ret;
}

std::string clean_addr2line_output(const std::string& input)
{
    std::vector < std::string > line = str2lines(input);
    for (const auto& i : line)
    {
        if (i == "?? ??:0")
        {
            continue;
        }

        return get_src_loc(i);
    }

    return "(no info)";
}

std::string exec_cmd(const std::string& cmd)
{
    std::array<char, 128> buffer{};
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }

    return result;
}

bool check_addr2line_avail()
{
    return (system("addr2line --help >/dev/null 2>/dev/null") == 0);
}
