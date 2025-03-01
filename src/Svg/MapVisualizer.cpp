#include <algorithm>
#include <functional>
#include <unordered_map>
#include "MapVisualizer.h"
#include "Objects.h"

using namespace Svg;

const std::string MapVisualizer::DefaultFontFamily = "Verdana";

namespace
{
    template <typename T>
    std::map<std::string, T> Map(const std::vector<T>& vec)
    {
        std::map<std::string, T> result;
        for (const auto* obj : vec)
        {
            result.insert({ obj->name, obj });
        }
        return result;
    }
}

MapVisualizer::MapVisualizer(
    const std::vector<const Descriptions::Stop*>& stops,
    const std::vector<const Descriptions::Bus*>& buses,
    const RenderSettings& renderSettings) : _stopsMapper(std::make_unique<InterpolationZipWithGluingStopMapper>(renderSettings, buses)),
    _stops(_stopsMapper->Map(stops)),
    _buses(Map(buses)),
    _renderSettings(renderSettings),
    _mapDoc(),
    _busNameToColor()
{
    CalculateBusColors();
}

void MapVisualizer::Render(std::ostream& out) const
{
    for (const auto& layer : _renderSettings.layers)
    {
        out << std::setprecision(Precision);
        auto renderFunc = GetRenderFuncByLayerName(layer);
        renderFunc(*this);
    }
    _mapDoc.Render(out);
}

void Svg::MapVisualizer::RenderRoute(std::ostream& out, const TransportRouter::RouteInfo& routeInfo)
{
    // TODO: Построить всю карту, если необходимо
    // TODO: Скопировать всю карту в Document для конкретного маршрута
    // TODO: Вывод покрывающего карту прямоугольника

    // RenderRouteBusesLines
    {
        for (const auto& item : routeInfo.items)
        {
            std::string firstStopName;
            std::visit([this, &firstStopName](auto&& item)
                {
                    using T = std::decay_t<decltype(item)>;
                    if constexpr (std::is_same_v<T, TransportRouter::RouteInfo::WaitItem>)
                    {
                        const TransportRouter::RouteInfo::WaitItem& waitItem = item;
                        if (firstStopName.empty())
                        {
                            firstStopName = waitItem.stop_name;
                        }
                    }
                    else if constexpr (std::is_same_v<T, TransportRouter::RouteInfo::BusItem>)
                    {
                        const TransportRouter::RouteInfo::BusItem& busItem = item;
                        const auto* bus = _buses[busItem.bus_name];

                    }
                    else
                    {
                        static_assert(false, "non-exhaustive visitor!");
                    }
                }, item);
        }
        // Беру каждый item из routeInfo
        // Если это waitItem:
            // Запоминаю стартовую остановку
        // Если это busItem:
            // Нахожу маршурт с заданным именем 
            // Рисую маршрут от стартовой остановки и определенное количество остановок
            // последнюю остановку маршрута запоминаю как стартовую
    }
}

MapVisualizer::RenderFunc MapVisualizer::GetRenderFuncByLayerName(const std::string& layerName) const
{
    static const std::unordered_map<std::string, RenderFunc> layerNameToRenderFunc = {
        {"bus_lines", &MapVisualizer::RenderAllBusesLines},
        {"bus_labels", &MapVisualizer::RenderAllBusesNames},
        {"stop_points", &MapVisualizer::RenderAllStopPoints},
        {"stop_labels", &MapVisualizer::RenderAllStopNames} };
    return layerNameToRenderFunc.at(layerName);
}

void MapVisualizer::RenderAllBusesLines() const
{
    const std::string defaultStrokeLineCap("round");
    const std::string defaultStrokeLineJoin("round");
    for (auto it = cbegin(_buses); it != cend(_buses); it++)
    {
        const auto* bus = it->second;
        RenderBusLine(_mapDoc, bus, bus->stops.front(), bus->stops.size());
    }
}

void MapVisualizer::RenderAllBusesNames() const
{
    for (auto it = cbegin(_buses); it != cend(_buses); it++)
    {
        const auto* bus = it->second;
        auto [firstStop, lastStop] = bus->GetTerminals();
        RenderBusName(bus->name, firstStop, GetBusColor(bus->name));
        if (firstStop != lastStop)
            RenderBusName(bus->name, lastStop, GetBusColor(bus->name));
    }
}

void MapVisualizer::RenderAllStopPoints() const
{
    const Color defaultStopColor("white");
    for (const auto& [_, pos] : _stops)
    {
        Circle busCirle;
        busCirle.SetPrecision(Precision);
        busCirle.SetCenter(pos);
        busCirle.SetRadius(_renderSettings.stopRadius);
        busCirle.SetFillColor(defaultStopColor);
        _mapDoc.Add(busCirle);
    }
}

void MapVisualizer::RenderAllStopNames() const
{
    const std::string defaultStrokeLineCap("round");
    const std::string defaultStrokeLineJoin("round");
    const Color textColor("black");
    for (const auto& [stopName, stopPoint] : _stops)
    {
        Text substrate;
        substrate.SetPrecision(Precision);
        substrate.SetPoint(stopPoint)
            .SetOffset(_renderSettings.stopLabelOffset)
            .SetFontSize(_renderSettings.stopLabelFontSize)
            .SetFontFamily(DefaultFontFamily)
            .SetData(stopName)
            .SetFillColor(_renderSettings.substrateUnderlayerColor)
            .SetStrokeColor(_renderSettings.substrateUnderlayerColor)
            .SetStrokeWidth(_renderSettings.underlayerWidth)
            .SetStrokeLineCap(defaultStrokeLineCap)
            .SetStrokeLineJoin(defaultStrokeLineJoin);
        _mapDoc.Add(substrate);

        Text stopNameText;
        stopNameText.SetPrecision(Precision);
        stopNameText.SetPoint(stopPoint)
            .SetOffset(_renderSettings.stopLabelOffset)
            .SetFontSize(_renderSettings.stopLabelFontSize)
            .SetFontFamily(DefaultFontFamily)
            .SetData(stopName)
            .SetFillColor(textColor);
        _mapDoc.Add(stopNameText);
    }
}

void MapVisualizer::RenderBusName(const std::string& busName, const std::string& stopName, const Color& busColor) const
{
    auto stopIt = _stops.find(stopName);
    if (stopIt == cend(_stops))
        return;
    const auto& coord = stopIt->second;
    const std::string defaultStrokeLineCap("round");
    const std::string defaultStrokeLineJoin("round");
    Text substrate;
    substrate.SetPrecision(Precision);
    substrate.SetPoint(coord)
        .SetOffset(_renderSettings.busLabelOffset)
        .SetFontSize(_renderSettings.busLabelFontSize)
        .SetFontFamily(DefaultFontFamily)
        .SetFontWeight("bold")
        .SetData(busName)
        .SetFillColor(_renderSettings.substrateUnderlayerColor)
        .SetStrokeColor(_renderSettings.substrateUnderlayerColor)
        .SetStrokeWidth(_renderSettings.underlayerWidth)
        .SetStrokeLineCap(defaultStrokeLineCap)
        .SetStrokeLineJoin(defaultStrokeLineJoin);
    _mapDoc.Add(substrate);

    Text busText;
    busText.SetPrecision(Precision);
    busText.SetPoint(coord)
        .SetOffset(_renderSettings.busLabelOffset)
        .SetFontSize(_renderSettings.busLabelFontSize)
        .SetFontFamily(DefaultFontFamily)
        .SetFontWeight("bold")
        .SetData(busName)
        .SetFillColor(busColor);
    _mapDoc.Add(busText);
}

void Svg::MapVisualizer::RenderBusLine(Document& doc,
    const Descriptions::Bus* bus,
    std::string_view startStop,
    size_t stopsCount) const 
{
    static const std::string defaultStrokeLineCap("round");
    static const std::string defaultStrokeLineJoin("round");
    Polyline busPath;
    busPath.SetPrecision(Precision);
    auto startStopNameIt = std::find(cbegin(bus->stops), cend(bus->stops), startStop);
    for(auto it = startStopNameIt; it != std::next(startStopNameIt, stopsCount); it++)
    {
        auto stopIt = _stops.find(*it);
        if (stopIt == _stops.end())
            continue;
        const auto& position = stopIt->second;
        busPath.AddPoint(position);
    }
    busPath.SetStrokeColor(GetBusColor(bus->name));
    busPath.SetStrokeWidth(_renderSettings.busLineWidth);
    busPath.SetStrokeLineCap(defaultStrokeLineCap);
    busPath.SetStrokeLineJoin(defaultStrokeLineJoin);
    doc.Add(busPath);
}

void Svg::MapVisualizer::CalculateBusColors()
{
    size_t busIndex = 0;
    for (auto it = cbegin(_buses); it != cend(_buses); it++, busIndex++)
    {
        const auto* bus = it->second;
        _busNameToColor[bus->name] = _renderSettings.colorPalette[busIndex % _renderSettings.colorPalette.size()];
    }
}

const Color& MapVisualizer::GetBusColor(std::string_view busName) const
{
    return _busNameToColor.at(busName);
}
