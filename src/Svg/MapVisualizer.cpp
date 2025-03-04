#include <algorithm>
#include <functional>
#include <unordered_map>
#include "MapVisualizer.h"
#include "Objects.h"

using namespace Svg;

const std::string MapVisualizer::DefaultFontFamily("Verdana");
const std::string MapVisualizer::DefaultStrokeLineCap("round");
const std::string MapVisualizer::DefaultStrokeLineJoin("round");

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
    RenderWholeMapIfNeeded();
    _mapDoc.Render(out);
}

void Svg::MapVisualizer::RenderRoute(std::ostream& out, const TransportRouter::RouteInfo& routeInfo, const std::string& finishStopName) const
{
    RenderWholeMapIfNeeded();
    // TODO: Сделать Render согласно очередности слоев в settings
    Document routeDoc = _mapDoc;
    RenderTranslucentRect(routeDoc);
    auto route = MapRoute(routeInfo, finishStopName);
    RenderRouteBusesLines(routeDoc, route);
    RenderRouteBusesNames(routeDoc, route);

    routeDoc.Render(out);
}

void Svg::MapVisualizer::RenderWholeMapIfNeeded() const
{
    if (!_mapDoc.Empty())
        return;
    for (const auto& layer : _renderSettings.layers)
    {
        auto renderFunc = GetRenderFuncByLayerName(layer);
        renderFunc(*this);
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
    for (auto it = cbegin(_buses); it != cend(_buses); it++)
    {
        const auto* bus = it->second;
        RenderBusLine(_mapDoc, bus, cbegin(bus->stops), cend(bus->stops));
    }
}

void MapVisualizer::RenderAllBusesNames() const
{
    for (auto it = cbegin(_buses); it != cend(_buses); it++)
    {
        const auto* bus = it->second;
        auto [firstStop, lastStop] = bus->GetTerminals();
        RenderBusName(_mapDoc, bus->name, firstStop, GetBusColor(bus));
        if (firstStop != lastStop)
            RenderBusName(_mapDoc, bus->name, lastStop, GetBusColor(bus));
    }
}

void MapVisualizer::RenderAllStopPoints() const
{
    static const Color defaultStopColor("white");
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
    static const Color textColor("black");
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
            .SetStrokeLineCap(DefaultStrokeLineCap)
            .SetStrokeLineJoin(DefaultStrokeLineJoin);
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

MapVisualizer::Route Svg::MapVisualizer::MapRoute(const TransportRouter::RouteInfo& routeInfo,
    const std::string& finishStopName) const
{
    Route route;
    route.reserve(routeInfo.items.size() / 2);
    std::string_view firstStopName;
    const TransportRouter::RouteInfo::BusItem* busItem;
    for (const auto& item : routeInfo.items)
    {
        std::visit([this, &firstStopName, &busItem, &route](auto&& item)
            {
                using T = std::decay_t<decltype(item)>;
                if constexpr (std::is_same_v<T, TransportRouter::RouteInfo::WaitItem>)
                {
                    const TransportRouter::RouteInfo::WaitItem& waitItem = item;
                    if (!firstStopName.empty())
                    {
                        std::string_view lastStopName = waitItem.stop_name;
                        route.emplace_back(MapRouteItem(busItem, firstStopName, lastStopName));
                        firstStopName = lastStopName;
                    }
                    else
                    {
                        firstStopName = waitItem.stop_name;
                    }
                }
                else if constexpr (std::is_same_v<T, TransportRouter::RouteInfo::BusItem>)
                {
                    busItem = &item;
                }
                else
                {
                    static_assert(false, "non-exhaustive visitor!");
                }
            }, item);
    }
    route.emplace_back(MapRouteItem(busItem, firstStopName, finishStopName));
    return route;
}

MapVisualizer::RouteItem Svg::MapVisualizer::MapRouteItem(const TransportRouter::RouteInfo::BusItem* busItem,
    std::string_view firstStopName,
    std::string_view lastStopName) const
{
    const auto* bus = _buses.at(busItem->bus_name);
    auto firstStopIt = std::find(cbegin(bus->stops), cend(bus->stops), firstStopName);
    while (true)
    {
        if (firstStopIt == cend(bus->stops))
        {
            break;
        }
        if (*std::next(firstStopIt, busItem->span_count) == lastStopName)
        {
            break;
        }
        firstStopIt = std::find(std::next(firstStopIt), cend(bus->stops), firstStopName);
    }
    assert(firstStopIt != cend(bus->stops));
    auto lastStopIt = std::next(firstStopIt, busItem->span_count + 1);
    std::cerr << "Svg::MapVisualizer::MapRouteItem() " << "bus name " << bus->name << " stopCount: " << std::distance(firstStopIt, lastStopIt) << std::endl;
    return RouteItem{ .bus = bus, .stopNames = std::vector<std::string>(firstStopIt, lastStopIt) };
}

void Svg::MapVisualizer::RenderTranslucentRect(Document& doc) const
{
    Svg::Rect rect;
    rect.SetLeftTop(Point{ .x = -_renderSettings.outerMargin, .y = -_renderSettings.outerMargin });
    rect.SetWidth(_renderSettings.maxMapWidth + 2 * _renderSettings.outerMargin);
    rect.SetHeight(_renderSettings.maxMapHeight + 2 * _renderSettings.outerMargin);
    rect.SetFillColor(_renderSettings.substrateUnderlayerColor);
    rect.SetStrokeColor(_renderSettings.substrateUnderlayerColor);
    rect.SetStrokeWidth(_renderSettings.underlayerWidth);
    doc.Add(rect);
}

void Svg::MapVisualizer::RenderRouteBusesLines(Document& doc, const Route& route) const
{
    for (const auto& routeItem : route)
    {
        RenderBusLine(doc, routeItem.bus, cbegin(routeItem.stopNames), cend(routeItem.stopNames));
    }
}

void Svg::MapVisualizer::RenderRouteBusesNames(Document& doc, const Route& route) const
{
    for (const auto& routeItem : route)
    {
        auto [firstStop, lastStop] = routeItem.bus->GetTerminals();
        if (routeItem.stopNames.front() == firstStop || routeItem.stopNames.front() == lastStop)
        {
            RenderBusName(doc, routeItem.bus->name, routeItem.stopNames.front(), GetBusColor(routeItem.bus));
        }
        if (routeItem.stopNames.back() == firstStop || routeItem.stopNames.back() == lastStop)
        {
            RenderBusName(doc, routeItem.bus->name, routeItem.stopNames.back(), GetBusColor(routeItem.bus));
        };
    }
}

void MapVisualizer::RenderBusName(Document& doc, const std::string& busName, const std::string& stopName, const Color& busColor) const
{
    auto stopIt = _stops.find(stopName);
    if (stopIt == cend(_stops))
        return;
    const auto& coord = stopIt->second;
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
        .SetStrokeLineCap(DefaultStrokeLineCap)
        .SetStrokeLineJoin(DefaultStrokeLineJoin);
    doc.Add(substrate);

    Text busText;
    busText.SetPrecision(Precision);
    busText.SetPoint(coord)
        .SetOffset(_renderSettings.busLabelOffset)
        .SetFontSize(_renderSettings.busLabelFontSize)
        .SetFontFamily(DefaultFontFamily)
        .SetFontWeight("bold")
        .SetData(busName)
        .SetFillColor(busColor);
    doc.Add(busText);
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

const Color& MapVisualizer::GetBusColor(const Descriptions::Bus* bus) const
{

    return _busNameToColor.at(bus->name);
}
