#pragma once

#include <iostream>
#include <map>
#include <functional>
#include <string_view>
#include <memory>
#include <string_view>
#include <unordered_map>
#include "StopsMappers.h"
#include "descriptions.h"
#include "RenderSettings.h"
#include "Document.h"
#include "transport_router.h"

namespace Svg
{
class MapVisualizer
{
public:
    MapVisualizer(const std::vector<const Descriptions::Stop*>& stops,
                  const std::vector<const Descriptions::Bus*>& buses,
                  const RenderSettings& renderSettins);

    void Render(std::ostream& out) const;
    void RenderRoute(std::ostream& out, const TransportRouter::RouteInfo& routeInfo);

private:
    using RenderFunc = std::function<void(const MapVisualizer&)>;
    RenderFunc GetRenderFuncByLayerName(const std::string& layerName) const;
    void RenderAllBusesLines() const;
    void RenderAllBusesNames() const;
    void RenderAllStopPoints() const;
    void RenderAllStopNames() const;

    

    void RenderBusName(const std::string& busName, const std::string& stopName, const Color& busColor) const;

    void RenderBusLine(Document& doc, 
        const Descriptions::Bus* bus, 
        std::string_view startStop, 
        size_t stopsCount) const;

    void CalculateBusColors();
    const Color& GetBusColor(std::string_view busName) const;

private:
    static const std::string DefaultFontFamily;
    static const int Precision = 15;
    
    IStopsMapperUnp _stopsMapper;
    std::map<std::string, Point> _stops;
    // TODO: Переделать на string_view из самих buses
    std::map<std::string, const Descriptions::Bus*> _buses;
    RenderSettings _renderSettings;
    // TODO: Оптимизация, сделать ленивую инициализацию на всю карту, а дальше по за просу на Route, дополнять только Route
    mutable Document _mapDoc;
    std::unordered_map<std::string_view, Color> _busNameToColor;
};

}

