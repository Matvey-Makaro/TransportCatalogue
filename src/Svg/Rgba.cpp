#include <iomanip>
#include "Rgba.h"

using namespace Svg;

std::ostream& operator<<(std::ostream& out, const Svg::Rgba& rgba)
{
    out << "rgba(" << static_cast<int>(rgba.red) << ','
        << static_cast<int>(rgba.green) << ','
        << static_cast<int>(rgba.blue) << ','
        << std::setprecision(15) << rgba.alpha << ')';
    return out;
}
