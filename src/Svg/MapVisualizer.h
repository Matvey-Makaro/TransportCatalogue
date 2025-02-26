#pragma once

#include <iostream>
#include <map>
#include <functional>
#include <string_view>
#include <memory>
#include "StopsMappers.h"
#include "descriptions.h"
#include "RenderSettings.h"
#include "Document.h"

namespace Svg
{
class MapVisualizer
{
public:
    MapVisualizer(const std::vector<const Descriptions::Stop*>& stops,
                  const std::vector<const Descriptions::Bus*>& buses,
                  const RenderSettings& renderSettins);

    void Render(std::ostream& out) const;

private:
    std::map<std::string, Point> MapStops(const std::vector<const Descriptions::Stop*>& stops) const;

    using RenderFunc = std::function<void(const MapVisualizer&)>;
    RenderFunc GetRenderFuncByLayerName(const std::string& layerName) const;
    void RenderBusesLines() const;
    void RenderBusesNames() const;
    void RenderStopPoints() const;
    void RenderStopNames() const;

    void RenderBusName(const std::string& busName, const std::string& stopName, const Color& busColor) const;

    const Color& GetBusColorByIndex(size_t index) const;

private:
    static const std::string DefaultFontFamily;
    
    IStopsMapperUnp _stopsMapper;
    std::map<std::string, Point> _stops;
    std::map<std::string, const Descriptions::Bus*> _buses;
    RenderSettings _renderSettings;
    mutable Document _mapDoc;
};

}

