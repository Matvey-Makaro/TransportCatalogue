#pragma once

#include <cstdint>
#include <iostream>

namespace Svg
{

struct Rgb
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

}

std::ostream& operator<<(std::ostream& out, const Svg::Rgb& rgb);
