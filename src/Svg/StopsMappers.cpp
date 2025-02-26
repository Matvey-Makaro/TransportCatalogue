#include <algorithm>
#include "StopsMappers.h"

using namespace Svg;

Svg::GeoStopMapper::GeoStopMapper(const RenderSettings &renderSettings) :
    _renderSettings(renderSettings),
    _coordMapper()
{}

std::map<std::string, Point> Svg::GeoStopMapper::Map(const std::vector<const Descriptions::Stop *>& stops)
{
    SetupCoordinateMapper(stops);
    std::map<std::string, Point> positions;
    for(const auto* s : stops)
    {
        positions.insert({s->name, _coordMapper.Map(s->position)});
    }
    return positions;
}

void Svg::GeoStopMapper::SetupCoordinateMapper(const std::vector<const Descriptions::Stop *>& stops)
{
    _coordMapper.SetMaxWidth(_renderSettings.maxMapWidth);
    _coordMapper.SetMaxHeight(_renderSettings.maxMapHeight);
    _coordMapper.SetPadding(_renderSettings.padding);
    if (stops.empty())
        return;

    auto [minLonIt, maxLonIt] = std::minmax_element(std::cbegin(stops), std::cend(stops),
                                                    [](const Descriptions::Stop* lhs, const Descriptions::Stop* rhs)
                                                    {
                                                        return lhs->position.longitude < rhs->position.longitude;
                                                    });

    _coordMapper.SetMaxLon((*maxLonIt)->position.longitude);
    _coordMapper.SetMinLon((*minLonIt)->position.longitude);
    auto [minLat, maxLat] = std::minmax_element(std::cbegin(stops), std::cend(stops),
                                                [](const Descriptions::Stop* lhs, const Descriptions::Stop* rhs)
                                                {
                                                    return lhs->position.latitude < rhs->position.latitude;
                                                });
    _coordMapper.SetMaxLat((*maxLat)->position.latitude);
    _coordMapper.SetMinLat((*minLat)->position.latitude);
}
