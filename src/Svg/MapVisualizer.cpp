#include <algorithm>
#include <functional>
#include <unordered_map>
#include "MapVisualizer.h"
#include "Objects.h"
#include "YellowPages/YellowPagesDatabase.h"

using namespace Svg;
using namespace Router;
using namespace Visualization;

const std::string MapVisualizer::DefaultFontFamily("Verdana");
const std::string MapVisualizer::DefaultStrokeLineCap("round");
const std::string MapVisualizer::DefaultStrokeLineJoin("round");
const std::string MapVisualizer::BusLinesLayerName("bus_lines");
const std::string MapVisualizer::BusLabelsLayerName("bus_labels");
const std::string MapVisualizer::StopPointsLayerName("stop_points");
const std::string MapVisualizer::StopLabelsLayerName("stop_labels");
const std::string MapVisualizer::CompanyLinesLayerName("company_lines");
const std::string MapVisualizer::CompanyPointsLayerName("company_points");
const std::string MapVisualizer::CompanyLabelsLayerName("company_labels");

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
    YellowPages::BLL::YellowPagesDatabaseShp yellowPagesDatabase,
    const std::vector<const Descriptions::Stop*>& stops,
    const std::vector<const Descriptions::Bus*>& buses,
    const RenderSettings& renderSettings) :
    _yellowPagesDatabase(std::move(yellowPagesDatabase)),
    _stopsMapper(std::make_unique<InterpolationZipWithGluingStopMapper>(renderSettings, buses)),
    _stops(),
    _companies(),
    _buses(Map(buses)),
    _renderSettings(renderSettings),
    _mapDoc(),
    _busNameToColor()
{
    auto [stopsDTO, companiesDTO] = _stopsMapper->Map(stops, _yellowPagesDatabase->GetCompaniesPtr());
    _stops = std::move(stopsDTO);
    _companies = std::move(companiesDTO);
    CalculateBusColors();
}

void MapVisualizer::Render(std::ostream& out) const
{
    RenderWholeMapIfNeeded();
    _mapDoc.Render(out);
}

void Svg::MapVisualizer::RenderRoute(std::ostream& out, const TransportRouter::RouteInfo& routeInfo) const
{
    RenderWholeMapIfNeeded();
    Document routeDoc = _mapDoc;
    RenderTranslucentRect(routeDoc);
    auto busRoute = MapRoute(routeInfo);

    bool isRouteWithCompany = std::holds_alternative<TransportRouter::RouteInfo::WalkToCompany>(routeInfo.items.back());
    bool isRouteWithBus = !busRoute.empty();

    using RenderBusRouteLayerFunc = void (MapVisualizer::*)(Document&, const Route&) const;
    static const std::unordered_map<std::string, RenderBusRouteLayerFunc> renderBusRouteFuncs = {
        {BusLinesLayerName, &MapVisualizer::RenderRouteBusesLines},
        {BusLabelsLayerName, &MapVisualizer::RenderRouteBusesNames},
        {StopPointsLayerName, &MapVisualizer::RenderRouteStopPoints},
        {StopLabelsLayerName, &MapVisualizer::RenderRouteStopNames}
    };

    using RenderCompanyRouteLayerFunc = void (MapVisualizer::*)(Document&, const TransportRouter::RouteInfo&) const;
    static const std::unordered_map<std::string, RenderCompanyRouteLayerFunc> renderCompanyRouteFuncs = {
        {CompanyLinesLayerName, &MapVisualizer::RenderCompanyLines},
        {CompanyPointsLayerName, &MapVisualizer::RenderCompanyPoints},
        {CompanyLabelsLayerName, &MapVisualizer::RenderCompanyLabels},
        {StopLabelsLayerName, &MapVisualizer::RenderFromStopName}
    };

    // TODO: Тут могут быть слои, которые не отрисовываются для маршрутов без компаний
    // TODO В случае, когда маршрут не содержит элементов RideBus и WaitBus, слой stop_labels должен содержать название одной остановки — той, от которой проложен маршрут.
    for (const auto& layer : _renderSettings.layers)
    {
        if (layer == StopLabelsLayerName && isRouteWithCompany && !isRouteWithBus)
        {

            auto renderFunc = renderCompanyRouteFuncs.at(layer);
            (this->*renderFunc)(routeDoc, routeInfo);
            continue;

        }
        if (isRouteWithBus)
        {
            auto renderBusRouteFuncIt = renderBusRouteFuncs.find(layer);
            if (renderBusRouteFuncIt != cend(renderBusRouteFuncs))
            {
                RenderBusRouteLayerFunc busRouteRenderFunc = renderBusRouteFuncIt->second;
                (this->*busRouteRenderFunc)(routeDoc, busRoute);
                continue;
            }
        }
        if (isRouteWithCompany)
        {
            auto renderCompanyRouteFuncIt = renderCompanyRouteFuncs.find(layer);
            if (renderCompanyRouteFuncIt != cend(renderCompanyRouteFuncs))
            {
                RenderCompanyRouteLayerFunc companyRouteRenderFunc = renderCompanyRouteFuncIt->second;
                (this->*companyRouteRenderFunc)(routeDoc, routeInfo);
            }
        }
    }
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
        {BusLinesLayerName, &MapVisualizer::RenderAllBusesLines},
        {BusLabelsLayerName, &MapVisualizer::RenderAllBusesNames},
        {StopPointsLayerName, &MapVisualizer::RenderAllStopPoints},
        {StopLabelsLayerName, &MapVisualizer::RenderAllStopNames}
    };
    auto it = layerNameToRenderFunc.find(layerName);
    if (it == cend(layerNameToRenderFunc))
    {
        return [](const MapVisualizer&) {};
    }
    return it->second;
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
    for (const auto& [_, pos] : _stops)
    {
        RenderStopPoint(_mapDoc, pos);
    }
}

void MapVisualizer::RenderAllStopNames() const
{
    static const Color textColor("black");
    for (const auto& [stopName, stopPoint] : _stops)
    {
        RenderName(_mapDoc, stopName, stopPoint);
    }
}

MapVisualizer::Route Svg::MapVisualizer::MapRoute(const TransportRouter::RouteInfo& routeInfo) const
{
    Route route;
    route.reserve(routeInfo.items.size() / 2);
    std::string_view firstStopName;
    const TransportRouter::RouteInfo::RideBusItem* busItem;
    for (const auto& item : routeInfo.items)
    {
        std::visit([this, &firstStopName, &busItem, &route](auto&& item)
            {
                using T = std::decay_t<decltype(item)>;
                if constexpr (std::is_same_v<T, TransportRouter::RouteInfo::WaitBusItem>)
                {
                    const TransportRouter::RouteInfo::WaitBusItem& waitItem = item;
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
                else if constexpr (std::is_same_v<T, TransportRouter::RouteInfo::RideBusItem>)
                {
                    busItem = &item;
                }
                else if constexpr (std::is_same_v<T, TransportRouter::RouteInfo::WalkToCompany>)
                {
                    // Nothing to do
                }
                else
                {
                    std::cerr << "Svg::MapVisualizer::MapRoute non-exhaustive visitor" << std::endl;
                    assert(false && "non-exhaustive visitor!");
                }
            }, item);
    }
    if (!firstStopName.empty())
    {
        route.emplace_back(MapRouteItem(busItem, firstStopName, GetFinishStopName(routeInfo)));
    }
    return route;
}

MapVisualizer::RouteItem Svg::MapVisualizer::MapRouteItem(const TransportRouter::RouteInfo::RideBusItem* busItem,
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
    return RouteItem{ .bus = bus, .stopNames = std::vector<std::string>(firstStopIt, lastStopIt) };
}

void Svg::MapVisualizer::RenderTranslucentRect(Document& doc) const
{
    Svg::Rect rect;
    rect.SetLeftTop(Point{ .x = -_renderSettings.outerMargin, .y = -_renderSettings.outerMargin });
    rect.SetWidth(_renderSettings.maxMapWidth + 2 * _renderSettings.outerMargin);
    rect.SetHeight(_renderSettings.maxMapHeight + 2 * _renderSettings.outerMargin);
    rect.SetFillColor(_renderSettings.substrateUnderlayerColor);
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

void Svg::MapVisualizer::RenderRouteStopPoints(Document& doc, const Route& route) const
{
    for (const auto& routeItem : route)
    {
        for (const auto& stop : routeItem.stopNames)
        {
            RenderStopPoint(doc, _stops.at(stop));
        }
    }
}

void Svg::MapVisualizer::RenderRouteStopNames(Document& doc, const Route& route) const
{
    if (route.empty())
        return;
    for (const auto& routeItem : route)
    {
        const auto& stopName = routeItem.stopNames.front();
        RenderName(doc, stopName, _stops.at(stopName));
    }
    if (route.back().stopNames.size() > 1)
    {
        const auto& lastStop = route.back().stopNames.back();
        RenderName(doc, lastStop, _stops.at(lastStop));
    }
}

void Svg::MapVisualizer::RenderCompanyLines(Document& doc, const Router::TransportRouter::RouteInfo& routeInfo) const
{
    const auto* walkToCompanyItem = GetWalkToCompany(routeInfo);
    if (walkToCompanyItem == nullptr)
    {
        return;
    }
    Polyline pathToCompany;
    pathToCompany.SetPrecision(Precision);
    pathToCompany.SetStrokeColor(Color("black"));
    pathToCompany.SetStrokeWidth(_renderSettings.companyLineWidth);
    pathToCompany.SetStrokeLineCap(DefaultStrokeLineCap);
    pathToCompany.SetStrokeLineJoin(DefaultStrokeLineJoin);
    pathToCompany.AddPoint(_stops.at(walkToCompanyItem->stop_name));
    pathToCompany.AddPoint(_companies.at(walkToCompanyItem->company_name));
    doc.Add(pathToCompany);
}

void Svg::MapVisualizer::RenderCompanyPoints(Document& doc, const Router::TransportRouter::RouteInfo& routeInfo) const
{
    const auto* walkToCompanyItem = GetWalkToCompany(routeInfo);
    if (walkToCompanyItem == nullptr)
    {
        return;
    }

    Circle busCirle;
    busCirle.SetPrecision(Precision);
    busCirle.SetCenter(_companies.at(walkToCompanyItem->company_name));
    busCirle.SetRadius(_renderSettings.companyRadius);
    busCirle.SetFillColor(Color("black"));
    doc.Add(busCirle);
}

void Svg::MapVisualizer::RenderCompanyLabels(Document& doc, const Router::TransportRouter::RouteInfo& routeInfo) const
{
    const auto* walkToCompanyItem = GetWalkToCompany(routeInfo);
    if (walkToCompanyItem == nullptr)
    {
        return;
    }
    const auto* company = _yellowPagesDatabase->GetCompanyByMainName(walkToCompanyItem->company_name);
    if (company == nullptr)
    {
        return;
    }
    const auto& companyPos = _companies.at(walkToCompanyItem->company_name);
    RenderName(doc, CreateFullName(*company), companyPos);
}

void Svg::MapVisualizer::RenderFromStopName(Document& doc, const Router::TransportRouter::RouteInfo& routeInfo) const
{
    const auto* walkToCompanyItem = GetWalkToCompany(routeInfo);
    if (walkToCompanyItem == nullptr)
    {
        return;
    }
    RenderName(doc, walkToCompanyItem->stop_name, _stops.at(walkToCompanyItem->stop_name));
}

const Router::TransportRouter::RouteInfo::WalkToCompany* Svg::MapVisualizer::GetWalkToCompany(
    const Router::TransportRouter::RouteInfo& routeInfo)
{
    if (routeInfo.items.empty())
    {
        return nullptr;
    }
    const auto& lastItem = routeInfo.items.back();
    return std::get_if<Router::TransportRouter::RouteInfo::WalkToCompany>(&lastItem);
}

std::string Svg::MapVisualizer::CreateFullName(const YellowPages::BLL::Company& company) const
{
    std::string fullName;
    if (!company.rubrics.empty())
    {
        const auto* rubric = _yellowPagesDatabase->GetRubricById(company.rubrics.front());
        if (rubric != nullptr)
        {
            fullName = rubric->name + " ";
        }
    }
    fullName += company.GetMainName().value;
    return fullName;
}

std::string Svg::MapVisualizer::GetFinishStopName(const Router::TransportRouter::RouteInfo& routeInfo) const
{
    if (routeInfo.items.empty())
    {
        return std::string();
    }
    const auto* walkToCompanyItem = GetWalkToCompany(routeInfo);
    if (walkToCompanyItem != nullptr)
    {
        return walkToCompanyItem->stop_name;
    }
    auto waitBusIt = std::find_if(crbegin(routeInfo.items), crend(routeInfo.items), [](const auto& item)
        {
            return std::holds_alternative<Router::TransportRouter::RouteInfo::WaitBusItem>(item);
        });
    assert(waitBusIt != crend(routeInfo.items));

    auto rideBusIt = std::find_if(crbegin(routeInfo.items), crend(routeInfo.items), [](const auto& item)
        {
            return std::holds_alternative<Router::TransportRouter::RouteInfo::RideBusItem>(item);
        });
    assert(rideBusIt != crend(routeInfo.items));
    const auto& waitBusItem = std::get<Router::TransportRouter::RouteInfo::WaitBusItem>(*waitBusIt);
    const auto& rideBusItem = std::get<Router::TransportRouter::RouteInfo::RideBusItem>(*rideBusIt);
    const auto& bus = _buses.at(rideBusItem.bus_name);
    auto firstStopIt = std::find(cbegin(bus->stops), cend(bus->stops), waitBusItem.stop_name);
    auto lastStopIt = std::next(firstStopIt, rideBusItem.span_count);
    assert(lastStopIt != cend(bus->stops));
    return *lastStopIt;
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

void Svg::MapVisualizer::RenderStopPoint(Document& doc, const Point& pos) const
{
    static const Color defaultStopColor("white");
    Circle busCirle;
    busCirle.SetPrecision(Precision);
    busCirle.SetCenter(pos);
    busCirle.SetRadius(_renderSettings.stopRadius);
    busCirle.SetFillColor(defaultStopColor);
    doc.Add(busCirle);
}

void Svg::MapVisualizer::RenderName(Document& doc, const std::string& name, const Point& point) const
{
    static const Color textColor("black");
    Text substrate;
    substrate.SetPrecision(Precision);
    substrate.SetPoint(point)
        .SetOffset(_renderSettings.stopLabelOffset)
        .SetFontSize(_renderSettings.stopLabelFontSize)
        .SetFontFamily(DefaultFontFamily)
        .SetData(name)
        .SetFillColor(_renderSettings.substrateUnderlayerColor)
        .SetStrokeColor(_renderSettings.substrateUnderlayerColor)
        .SetStrokeWidth(_renderSettings.underlayerWidth)
        .SetStrokeLineCap(DefaultStrokeLineCap)
        .SetStrokeLineJoin(DefaultStrokeLineJoin);
    doc.Add(substrate);

    Text stopNameText;
    stopNameText.SetPrecision(Precision);
    stopNameText.SetPoint(point)
        .SetOffset(_renderSettings.stopLabelOffset)
        .SetFontSize(_renderSettings.stopLabelFontSize)
        .SetFontFamily(DefaultFontFamily)
        .SetData(name)
        .SetFillColor(textColor);
    doc.Add(stopNameText);
}

void Svg::MapVisualizer::CalculateBusColors()
{
    if (_renderSettings.colorPalette.empty())
    {
        return;
    }
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


