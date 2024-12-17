#include "Rgb.h"

std::ostream& operator<<(std::ostream& out, const Svg::Rgb& rgb)
{
    out << "rgb(" << static_cast<int>(rgb.red) << ','
        << static_cast<int>(rgb.green) << ','
        << static_cast<int>(rgb.blue) << ')';
    return out;
}
