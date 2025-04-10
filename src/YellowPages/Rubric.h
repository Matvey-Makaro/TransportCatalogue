#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace YellowPages
{
    namespace BLL
    {
        struct Rubric
        {
            using IdType = uint64_t;
            std::string name;
            std::vector<std::string> keywords;
        };
        bool operator==(const Rubric& lhs, const Rubric& rhs);
    }
}
