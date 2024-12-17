#pragma once

#include <cstdint>
#include <iostream>

namespace Svg
{

struct Rgba
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    double alpha;
};

}

std::ostream& operator<<(std::ostream& out, const Svg::Rgba& rgba);
