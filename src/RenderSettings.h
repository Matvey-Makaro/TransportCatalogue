#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include "json.h"
#include "Svg/Point.h"
#include "Svg/Color.h"

struct RenderSettings
{
    double maxMapWidth;
    double maxMapHeight;
    double padding;
    double stopRadius;
    double busLineWidth;
    uint32_t stopLabelFontSize;
    Svg::Point stopLabelOffset;
    Svg::Color substrateUnderlayerColor;
    double underlayerWidth;
    std::vector<Svg::Color> colorPalette;
    uint32_t busLabelFontSize;
    Svg::Point busLabelOffset;
    std::vector<std::string> layers;
    double outerMargin;

    static RenderSettings ParseFrom(const Json::Dict& attrs);
};
