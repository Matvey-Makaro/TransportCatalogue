#include "Utils.h"

#include <cctype>
#include <cmath>

using namespace std;

string_view Strip(string_view line)
{
    while (!line.empty() && isspace(line.front()))
    {
        line.remove_prefix(1);
    }
    while (!line.empty() && isspace(line.back()))
    {
        line.remove_suffix(1);
    }
    return line;
}
bool IsEqual(double lhs, double rhs, double eps)
{
    return std::fabs(lhs - rhs) < eps;
}


std::string EscapeSpecialCharacters(const std::string &input)
{
#ifndef NoEscapeNeeded
    static std::unordered_map<char, std::string> escape_map = {
        {'\n', "\\n"},
        {'\t', "\\t"},
        {'\r', "\\r"},
        {'\\', "\\\\"},
        {'"', "\\\""},
        {'\'', "\\\'"}};
    std::string escaped_string;
    for (char ch : input)
    {
        if (escape_map.find(ch) != escape_map.end())
        {
            escaped_string += escape_map[ch];
        }
        else
        {
            escaped_string += ch;
        }
    }
    return escaped_string;
#else
    return input;
#endif
}
