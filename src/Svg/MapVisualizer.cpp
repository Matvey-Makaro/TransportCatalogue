#include <algorithm>
#include "MapVisualizer.h"
#include "Objects.h"

using namespace Svg;

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
    RenderBuses();
    RenderStopCircles();
    RenderStopNames();
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

void MapVisualizer::RenderBuses() const
{
    size_t busIndex = 0;
    const std::string defaultStrokeLineCap("round");
    const std::string defaultStrokeLineJoin("round");
    for(const auto& [_, bus] : _buses)
    {
        Polyline busPath;
        for(const auto& stopName : bus->stops)
        {
            auto it = _stops.find(stopName);
            if(it == _stops.end())
                continue;
            const auto& stop = it->second;
            busPath.AddPoint(_coordMapper.Map(stop->position));
        }
        busPath.SetStrokeColor(_renderSettins.colorPalette[busIndex % _renderSettins.colorPalette.size()]);
        busPath.SetStrokeWidth(_renderSettins.busLineWidth);
        busPath.SetStrokeLineCap(defaultStrokeLineCap);
        busPath.SetStrokeLineJoin(defaultStrokeLineJoin);
        busIndex++;
        _mapDoc.Add(busPath);
    }
}

void MapVisualizer::RenderStopCircles() const
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
    const std::string defaultFontFamily = "Verdana";
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
            .SetFontFamily(defaultFontFamily)
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
            .SetFontFamily(defaultFontFamily)
            .SetData(stop->name)
            .SetFillColor(textColor);
        _mapDoc.Add(stopName);
    }
}
