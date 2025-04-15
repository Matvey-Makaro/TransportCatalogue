#include <algorithm>
#include "Rubric.h"

bool YellowPages::BLL::operator==(const YellowPages::BLL::Rubric &lhs, const YellowPages::BLL::Rubric &rhs)
{
    if (lhs.name != rhs.name)
        return false;
    return std::equal(cbegin(lhs.keywords), cend(lhs.keywords),
                      cbegin(rhs.keywords), cend(rhs.keywords));
}
