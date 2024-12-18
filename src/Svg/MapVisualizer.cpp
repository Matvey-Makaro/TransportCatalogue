#include <algorithm>
#include <functional>
#include <unordered_map>
#include "MapVisualizer.h"
#include "Objects.h"

using namespace Svg;

const std::string MapVisualizer::DefaultFontFamily = "Verdana";


namespace
{
template<typename T>
std::map<std::string, T> Map(const std::vector<T>& vec)
{
    std::map<std::string, T> result;
    for(const auto* obj : vec)
    {
        result.insert({obj->name, obj});
    }
    return result;
}
}

MapVisualizer::MapVisualizer(
    const std::vector<const Descriptions::Stop*>& stops,
    const std::vector<const Descriptions::Bus*>& buses,
    const RenderSettings &renderSettins) :
    _stops(Map(stops)),
    _buses(Map(buses)),
    _renderSettins(renderSettins),
    _coordMapper(),
    _mapDoc()
{
    SetupCoordinateMapper();
}

void MapVisualizer::Render(std::ostream &out) const
{
    for(const auto& layer : _renderSettins.layers)
    {
        auto renderFunc = GetRenderFuncByLayerName(layer);
        renderFunc(*this);
    }
    _mapDoc.Render(out);
}

void MapVisualizer::SetupCoordinateMapper()
{
    _coordMapper.SetMaxWidth(_renderSettins.maxMapWidth);
    _coordMapper.SetMaxHeight(_renderSettins.maxMapHeight);
    _coordMapper.SetPadding(_renderSettins.padding);
    if(_stops.empty())
        return;

    auto [minLonIt, maxLonIt] = std::minmax_element(std::cbegin(_stops), std::cend(_stops),
                                                    [](const auto lhs, const auto rhs)
                                                    {
                                                        const auto& lhsPosition = lhs.second->position;
                                                        const auto& rhsPosition = rhs.second->position;
                                                        return lhsPosition.longitude < rhsPosition.longitude;
                                                    });

    _coordMapper.SetMaxLon(maxLonIt->second->position.longitude);
    _coordMapper.SetMinLon(minLonIt->second->position.longitude);
    auto [minLat, maxLat] = std::minmax_element(std::cbegin(_stops), std::cend(_stops),
                                                [](const auto lhs, const auto rhs)
                                                {
                                                    const auto& lhsPosition = lhs.second->position;
                                                    const auto& rhsPosition = rhs.second->position;
                                                    return lhsPosition.latitude < rhsPosition.latitude;
                                                });
    _coordMapper.SetMaxLat(maxLat->second->position.latitude);
    _coordMapper.SetMinLat(minLat->second->position.latitude);
}

MapVisualizer::RenderFunc MapVisualizer::GetRenderFuncByLayerName(const std::string &layerName) const
{
    static const std::unordered_map<std::string, RenderFunc> layerNameToRenderFunc = {
        {"bus_lines", &MapVisualizer::RenderBusesLines},
        {"bus_labels", &MapVisualizer::RenderBusesNames},
        {"stop_points", &MapVisualizer::RenderStopPoints},
        {"stop_labels", &MapVisualizer::RenderStopNames}
    };
    return layerNameToRenderFunc.at(layerName);
}

void MapVisualizer::RenderBusesLines() const
{
    size_t busIndex = 0;
    const std::string defaultStrokeLineCap("round");
    const std::string defaultStrokeLineJoin("round");
    for(auto it = cbegin(_buses); it != cend(_buses); it++, busIndex++)
    {
        const auto* bus = it->second;
        Polyline busPath;
        for(const auto& stopName : bus->stops)
        {
            auto it = _stops.find(stopName);
            if(it == _stops.end())
                continue;
            const auto& stop = it->second;
            busPath.AddPoint(_coordMapper.Map(stop->position));
        }
        busPath.SetStrokeColor(GetBusColorByIndex(busIndex));
        busPath.SetStrokeWidth(_renderSettins.busLineWidth);
        busPath.SetStrokeLineCap(defaultStrokeLineCap);
        busPath.SetStrokeLineJoin(defaultStrokeLineJoin);
        _mapDoc.Add(busPath);
    }
}

void MapVisualizer::RenderBusesNames() const
{
    size_t busIndex = 0;
    for(auto it = cbegin(_buses); it != cend(_buses); it++, busIndex++)
    {
        const auto* bus = it->second;
        auto [firstStop, lastStop] = bus->GetTerminals();
        RenderBusName(bus->name, firstStop, GetBusColorByIndex(busIndex));
        if(firstStop != lastStop)
            RenderBusName(bus->name, lastStop, GetBusColorByIndex(busIndex));
    }
}

void MapVisualizer::RenderStopPoints() const
{
    const Color defaultStopColor("white");
    for(const auto& [_, stop] : _stops)
    {
        Circle busCirle;
        busCirle.SetCenter(_coordMapper.Map(stop->position));
        busCirle.SetRadius(_renderSettins.stopRadius);
        busCirle.SetFillColor(defaultStopColor);
        _mapDoc.Add(busCirle);
    }
}

void MapVisualizer::RenderStopNames() const
{
    const std::string defaultStrokeLineCap("round");
    const std::string defaultStrokeLineJoin("round");
    const Color textColor("black");
    for(const auto& [_, stop] : _stops)
    {
        Text substrate;
        auto stopPoint = _coordMapper.Map(stop->position);
        substrate.SetPoint(stopPoint)
            .SetOffset(_renderSettins.stopLabelOffset)
            .SetFontSize(_renderSettins.stopLabelFontSize)
            .SetFontFamily(DefaultFontFamily)
            .SetData(stop->name)
            .SetFillColor(_renderSettins.substrateUnderlayerColor)
            .SetStrokeColor(_renderSettins.substrateUnderlayerColor)
            .SetStrokeWidth(_renderSettins.underlayerWidth)
            .SetStrokeLineCap(defaultStrokeLineCap)
            .SetStrokeLineJoin(defaultStrokeLineJoin);
        _mapDoc.Add(substrate);

        Text stopName;
        stopName.SetPoint(stopPoint)
            .SetOffset(_renderSettins.stopLabelOffset)
            .SetFontSize(_renderSettins.stopLabelFontSize)
            .SetFontFamily(DefaultFontFamily)
            .SetData(stop->name)
            .SetFillColor(textColor);
        _mapDoc.Add(stopName);
    }
}

void MapVisualizer::RenderBusName(const std::string &busName, const std::string &stopName, const Color &busColor) const
{
    auto stopIt = _stops.find(stopName);
    if(stopIt == cend(_stops))
        return;
    const auto* stop = stopIt->second;
    auto coord = _coordMapper.Map(stop->position);
    const std::string defaultStrokeLineCap("round");
    const std::string defaultStrokeLineJoin("round");
    Text substrate;
    substrate.SetPoint(coord)
        .SetOffset(_renderSettins.busLabelOffset)
        .SetFontSize(_renderSettins.busLabelFontSize)
        .SetFontFamily(DefaultFontFamily)
        .SetFontWeight("bold")
        .SetData(busName)
        .SetFillColor(_renderSettins.substrateUnderlayerColor)
        .SetStrokeColor(_renderSettins.substrateUnderlayerColor)
        .SetStrokeWidth(_renderSettins.underlayerWidth)
        .SetStrokeLineCap(defaultStrokeLineCap)
        .SetStrokeLineJoin(defaultStrokeLineJoin);
    _mapDoc.Add(substrate);

    Text busText;
    busText.SetPoint(coord)
        .SetOffset(_renderSettins.busLabelOffset)
        .SetFontSize(_renderSettins.busLabelFontSize)
        .SetFontFamily(DefaultFontFamily)
        .SetFontWeight("bold")
        .SetData(busName)
        .SetFillColor(busColor);
    _mapDoc.Add(busText);
}

const Color &MapVisualizer::GetBusColorByIndex(size_t index) const
{
    return _renderSettins.colorPalette[index % _renderSettins.colorPalette.size()];
}
