#pragma once

#include <variant>
#include <string>
#include <iostream>
#include "Rgb.h"
#include "Rgba.h"

namespace Svg
{

class Color
{
public:
using ColorType = std::variant<std::monostate,
                                   std::string,
                                   Rgb,
                                   Rgba>;

    Color();
    Color(std::string colorName);
    Color(std::string_view colorName);
    Color(const char* colorName);
    Color(const Rgb& rgb);
    Color(const Rgba& rgba);
    ~Color() = default;

    Color(const Color& other) = default;
    Color(Color&& other) = default;

    Color& operator=(const Color& other) = default;
    Color& operator=(Color&& other) = default;

    const ColorType& GetColor() const;

private:
    ColorType _color;

    friend std::ostream& operator<<(std::ostream& out, const Svg::Color& color)
    {
        std::visit([&out, &color](auto&& c)
                   {
                       using T = std::decay_t<decltype(c)>;
                       if constexpr (std::is_same_v<T, std::monostate>)
                       {
                           out << "none";
                       }
                       else
                       {
                           out << c;
                       }

                   }, color._color);
        return out;
    }
};

inline Color NoneColor = Color();
}
