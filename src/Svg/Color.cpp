#include <iostream>
#include "Color.h"

using namespace Svg;

Color::Color() :
    _color()
{}

Color::Color(std::string colorName) :
    _color(std::move(colorName))
{}

Color::Color(std::string_view colorName) :
    _color(std::string(colorName))
{}

Color::Color(const char *colorName):
    _color(std::string(colorName))
{

}

Color::Color(const Rgb& rgb) :
    _color(rgb)
{}

Color::Color(const Rgba &rgba) :
    _color(rgba)
{}

