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
    std::map<std::string, T> Map(const std::vector<T> &vec)
    {
        std::map<std::string, T> result;
        for (const auto *obj : vec)
        {
            result.insert({obj->name, obj});
        }
        return result;
    }
}

MapVisualizer::MapVisualizer(
    const std::vector<const Descriptions::Stop *> &stops,
    const std::vector<const Descriptions::Bus *> &buses,
    const RenderSettings &renderSettings) : _stopsMapper(std::make_unique<GeoStopMapper>(renderSettings)),
                                           _stops(_stopsMapper->Map(stops)),
                                           _buses(Map(buses)),
                                           _renderSettings(renderSettings),
                                           _mapDoc()
{}

void MapVisualizer::Render(std::ostream &out) const
{
    for (const auto &layer : _renderSettings.layers)
    {
        auto renderFunc = GetRenderFuncByLayerName(layer);
        renderFunc(*this);
    }
    _mapDoc.Render(out);
}

std::map<std::string, Point> MapVisualizer::MapStops(const std::vector<const Descriptions::Stop *> &stops) const
{
    std::vector<const Descriptions::Stop *> sortedStops(cbegin(stops), cend(stops));
    std::sort(begin(sortedStops), end(sortedStops), [](const Descriptions::Stop *lhs, const Descriptions::Stop *rhs)
            { 
                return lhs->position.longitude < rhs->position.longitude; 
            });
    std::map<std::string, Point> stopsPositions;
    for(const auto* s : stops)
    {
        stopsPositions.insert({s->name, Point{}});
    }


    {
        double xCurr = _renderSettings.padding;
        double xStep = stopsPositions.size() > 1 ? (_renderSettings.maxMapWidth - 2 * _renderSettings.padding) / (stopsPositions.size() - 1) :
                                                    0.0;
        for(const auto* s : sortedStops)
        {
            auto& pos = stopsPositions[s->name];
            pos.x = xCurr;
            xCurr += xStep;
        }
    }
    
    {
        double yCurr = _renderSettings.padding;
        double yStep = stopsPositions.size() > 1 ? (_renderSettings.maxMapWidth - 2 * _renderSettings.padding) / (stopsPositions.size() - 1) :
                                                    0.0;
        for(const auto* s : sortedStops)
        {
            auto& pos = stopsPositions[s->name];
            pos.y = yCurr;
            yCurr += yStep;
        }
    }
    return stopsPositions;
}

MapVisualizer::RenderFunc MapVisualizer::GetRenderFuncByLayerName(const std::string &layerName) const
{
    static const std::unordered_map<std::string, RenderFunc> layerNameToRenderFunc = {
        {"bus_lines", &MapVisualizer::RenderBusesLines},
        {"bus_labels", &MapVisualizer::RenderBusesNames},
        {"stop_points", &MapVisualizer::RenderStopPoints},
        {"stop_labels", &MapVisualizer::RenderStopNames}};
    return layerNameToRenderFunc.at(layerName);
}

void MapVisualizer::RenderBusesLines() const
{
    size_t busIndex = 0;
    const std::string defaultStrokeLineCap("round");
    const std::string defaultStrokeLineJoin("round");
    for (auto it = cbegin(_buses); it != cend(_buses); it++, busIndex++)
    {
        const auto *bus = it->second;
        Polyline busPath;
        for (const auto &stopName : bus->stops)
        {
            auto it = _stops.find(stopName);
            if (it == _stops.end())
                continue;
            const auto &position = it->second;
            busPath.AddPoint(position);
        }
        busPath.SetStrokeColor(GetBusColorByIndex(busIndex));
        busPath.SetStrokeWidth(_renderSettings.busLineWidth);
        busPath.SetStrokeLineCap(defaultStrokeLineCap);
        busPath.SetStrokeLineJoin(defaultStrokeLineJoin);
        _mapDoc.Add(busPath);
    }
}

void MapVisualizer::RenderBusesNames() const
{
    size_t busIndex = 0;
    for (auto it = cbegin(_buses); it != cend(_buses); it++, busIndex++)
    {
        const auto *bus = it->second;
        auto [firstStop, lastStop] = bus->GetTerminals();
        RenderBusName(bus->name, firstStop, GetBusColorByIndex(busIndex));
        if (firstStop != lastStop)
            RenderBusName(bus->name, lastStop, GetBusColorByIndex(busIndex));
    }
}

void MapVisualizer::RenderStopPoints() const
{
    const Color defaultStopColor("white");
    for (const auto &[_, pos] : _stops)
    {
        Circle busCirle;
        busCirle.SetCenter(pos);
        busCirle.SetRadius(_renderSettings.stopRadius);
        busCirle.SetFillColor(defaultStopColor);
        _mapDoc.Add(busCirle);
    }
}

void MapVisualizer::RenderStopNames() const
{
    const std::string defaultStrokeLineCap("round");
    const std::string defaultStrokeLineJoin("round");
    const Color textColor("black");
    for (const auto &[stopName, stopPoint] : _stops)
    {
        Text substrate;
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
        stopNameText.SetPoint(stopPoint)
            .SetOffset(_renderSettings.stopLabelOffset)
            .SetFontSize(_renderSettings.stopLabelFontSize)
            .SetFontFamily(DefaultFontFamily)
            .SetData(stopName)
            .SetFillColor(textColor);
        _mapDoc.Add(stopNameText);
    }
}

void MapVisualizer::RenderBusName(const std::string &busName, const std::string &stopName, const Color &busColor) const
{
    auto stopIt = _stops.find(stopName);
    if (stopIt == cend(_stops))
        return;
    const auto& coord = stopIt->second;
    const std::string defaultStrokeLineCap("round");
    const std::string defaultStrokeLineJoin("round");
    Text substrate;
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
    busText.SetPoint(coord)
        .SetOffset(_renderSettings.busLabelOffset)
        .SetFontSize(_renderSettings.busLabelFontSize)
        .SetFontFamily(DefaultFontFamily)
        .SetFontWeight("bold")
        .SetData(busName)
        .SetFillColor(busColor);
    _mapDoc.Add(busText);
}

const Color &MapVisualizer::GetBusColorByIndex(size_t index) const
{
    return _renderSettings.colorPalette[index % _renderSettings.colorPalette.size()];
}
