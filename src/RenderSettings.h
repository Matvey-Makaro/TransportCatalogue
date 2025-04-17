#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include "Json.h"
#include "Svg/Point.h"
#include "Svg/Color.h"

namespace Visualization
{
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
        double companyRadius;
        double companyLineWidth;
    
        static RenderSettings ParseFrom(const Json::Dict& attrs);
    };
}
