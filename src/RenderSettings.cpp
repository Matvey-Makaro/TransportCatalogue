#include <cassert>
#include "RenderSettings.h"

using namespace Svg;

namespace
{
Svg::Point ParsePointFrom(const Json::Node& node)
{
    const auto& arr = node.AsArray();
    assert(arr.size() == 2);
    return Svg::Point{
        .x = arr[0].AsDouble(),
        .y = arr[1].AsDouble()
    };
}

Svg::Color ParseColorFrom(const Json::Node& node)
{
    if(node.IsString())
    {
        return Svg::Color(node.AsString());
    }
    if(node.IsArray())
    {
        const auto& arr = node.AsArray();
        constexpr size_t rgbSize = 3;
        if(arr.size() == rgbSize)
        {
            return Color(Rgb{
                .red = static_cast<decltype(Rgb::red)>(arr[0].AsInt()),
                .green = static_cast<decltype(Rgb::green)>(arr[1].AsInt()),
                .blue = static_cast<decltype(Rgb::blue)>(arr[2].AsInt()),
            });
        }
        constexpr size_t rgbaSize = 4;
        if(arr.size() == rgbaSize)
        {
            return Color(Rgba{
                .red = static_cast<decltype(Rgba::red)>(arr[0].AsInt()),
                .green = static_cast<decltype(Rgba::green)>(arr[1].AsInt()),
                .blue = static_cast<decltype(Rgba::blue)>(arr[2].AsInt()),
                .alpha = arr[3].AsDouble()
            });
        }
    }
    return Svg::NoneColor;
}

std::vector<Color> ParseColorPaletteFrom(const Json::Node& node)
{
    std::vector<Color> colorPalette;
    const auto& jsonArr = node.AsArray();
    colorPalette.reserve(jsonArr.size());
    for(const auto& n : jsonArr)
    {
        colorPalette.push_back(ParseColorFrom(n));
    }
    return colorPalette;
}

std::vector<std::string> ParseStringVecFrom(const Json::Node& node)
{
    const auto& jsonArr = node.AsArray();
    std::vector<std::string> stringVec;
    stringVec.reserve(jsonArr.size());
    for(const auto& n : jsonArr)
    {
        stringVec.push_back(n.AsString());
    }
    return stringVec;
}

}

RenderSettings RenderSettings::ParseFrom(const Json::Dict &attrs)
{
    RenderSettings settings {
        .maxMapWidth = attrs.at("width").AsDouble(),
        .maxMapHeight = attrs.at("height").AsDouble(),
        .padding = attrs.at("padding").AsDouble(),
        .stopRadius = attrs.at("stop_radius").AsDouble(),
        .busLineWidth = attrs.at("line_width").AsDouble(),
        .stopLabelFontSize = static_cast<decltype(RenderSettings::stopLabelFontSize)>(attrs.at("stop_label_font_size").AsInt()),
        .stopLabelOffset = ParsePointFrom(attrs.at("stop_label_offset")),
        .substrateUnderlayerColor = ParseColorFrom(attrs.at("underlayer_color")),
        .underlayerWidth = attrs.at("underlayer_width").AsDouble(),
        .colorPalette = ParseColorPaletteFrom(attrs.at("color_palette")),
        .busLabelFontSize = static_cast<decltype(RenderSettings::busLabelFontSize)>(attrs.at("bus_label_font_size").AsInt()),
        .busLabelOffset = ParsePointFrom(attrs.at("bus_label_offset")),
        .layers = ParseStringVecFrom(attrs.at("layers")),
        .outerMargin = attrs.at("outer_margin").AsDouble()
    };
    return settings;
}
