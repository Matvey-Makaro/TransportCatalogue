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

Svg::ZipStopMapper::ZipStopMapper(const RenderSettings &renderSettings) :
    _renderSettings(renderSettings)
{}

std::map<std::string, Point> Svg::ZipStopMapper::Map(const std::vector<const Descriptions::Stop *> &stops)
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
    
    std::sort(begin(sortedStops), end(sortedStops), [](const Descriptions::Stop *lhs, const Descriptions::Stop *rhs)
            { 
                return lhs->position.latitude > rhs->position.latitude; 
            });

    {
        double yCurr = stopsPositions.size() > 1 ? _renderSettings.padding : (_renderSettings.maxMapHeight - _renderSettings.padding);
        double yStep = stopsPositions.size() > 1 ? (_renderSettings.maxMapHeight - 2 * _renderSettings.padding) / (stopsPositions.size() - 1) :
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

// TODO: Commit finish task J

// std::map<std::string, Point> Svg::ZipWithGluing::Map(const std::vector<const Descriptions::Stop *> &stops)
// {
//     std::map<std::string, Point> stopsPositions;
//     for(const auto* s : stops)
//     {
//         stopsPositions.insert({s->name, Point{}});
//     }

//     std::vector<const Descriptions::Stop *> sortedStops(cbegin(stops), cend(stops));
//     std::sort(begin(sortedStops), end(sortedStops), [](const Descriptions::Stop *lhs, const Descriptions::Stop *rhs)
//             { 
//                 return lhs->position.longitude < rhs->position.longitude; 
//             });

//     std::vector<std::vector<const Descriptions::Stop*>> indexToStops;
//     size_t currIndex = 0;
//     indexToStops.emplace_back();
//     indexToStops.front().push_back(sortedStops.front());
//     for(size_t i = 1; i < sortedStops.size(); i++)
//     {
//         // TODO: is needed?
//         // if(currIndex >= indexToStops.size())
//         for(const auto* s : indexToStops[currIndex])
//         {
//             if(IsRouteNeighbors(sortedStops[i], s))
//             {
//                 currIndex++;
//                 indexToStops.emplace_back();

//             }
//         }

//     }

    
// }
