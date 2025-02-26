#include <algorithm>
#include "StopsMappers.h"

using namespace Svg;

Svg::GeoStopMapper::GeoStopMapper(const RenderSettings &renderSettings) : _renderSettings(renderSettings),
                                                                          _coordMapper()
{
}

std::map<std::string, Point> Svg::GeoStopMapper::Map(const std::vector<const Descriptions::Stop *> &stops)
{
    SetupCoordinateMapper(stops);
    std::map<std::string, Point> positions;
    for (const auto *s : stops)
    {
        positions.insert({s->name, _coordMapper.Map(s->position)});
    }
    return positions;
}

void Svg::GeoStopMapper::SetupCoordinateMapper(const std::vector<const Descriptions::Stop *> &stops)
{
    _coordMapper.SetMaxWidth(_renderSettings.maxMapWidth);
    _coordMapper.SetMaxHeight(_renderSettings.maxMapHeight);
    _coordMapper.SetPadding(_renderSettings.padding);
    if (stops.empty())
        return;

    auto [minLonIt, maxLonIt] = std::minmax_element(std::cbegin(stops), std::cend(stops),
                                                    [](const Descriptions::Stop *lhs, const Descriptions::Stop *rhs)
                                                    {
                                                        return lhs->position.longitude < rhs->position.longitude;
                                                    });

    _coordMapper.SetMaxLon((*maxLonIt)->position.longitude);
    _coordMapper.SetMinLon((*minLonIt)->position.longitude);
    auto [minLat, maxLat] = std::minmax_element(std::cbegin(stops), std::cend(stops),
                                                [](const Descriptions::Stop *lhs, const Descriptions::Stop *rhs)
                                                {
                                                    return lhs->position.latitude < rhs->position.latitude;
                                                });
    _coordMapper.SetMaxLat((*maxLat)->position.latitude);
    _coordMapper.SetMinLat((*minLat)->position.latitude);
}

Svg::ZipStopMapper::ZipStopMapper(const RenderSettings &renderSettings) : _renderSettings(renderSettings)
{
}

std::map<std::string, Point> Svg::ZipStopMapper::Map(const std::vector<const Descriptions::Stop *> &stops)
{
    std::vector<const Descriptions::Stop *> sortedStops(cbegin(stops), cend(stops));
    std::sort(begin(sortedStops), end(sortedStops), [](const Descriptions::Stop *lhs, const Descriptions::Stop *rhs)
              { return lhs->position.longitude < rhs->position.longitude; });
    std::map<std::string, Point> stopsPositions;
    for (const auto *s : stops)
    {
        stopsPositions.insert({s->name, Point{}});
    }

    {
        double xCurr = _renderSettings.padding;
        double xStep = stopsPositions.size() > 1 ? (_renderSettings.maxMapWidth - 2 * _renderSettings.padding) / (stopsPositions.size() - 1) : 0.0;
        for (const auto *s : sortedStops)
        {
            auto &pos = stopsPositions[s->name];
            pos.x = xCurr;
            xCurr += xStep;
        }
    }

    std::sort(begin(sortedStops), end(sortedStops), [](const Descriptions::Stop *lhs, const Descriptions::Stop *rhs)
              { return lhs->position.latitude > rhs->position.latitude; });

    {
        double yCurr = stopsPositions.size() > 1 ? _renderSettings.padding : (_renderSettings.maxMapHeight - _renderSettings.padding);
        double yStep = stopsPositions.size() > 1 ? (_renderSettings.maxMapHeight - 2 * _renderSettings.padding) / (stopsPositions.size() - 1) : 0.0;
        for (const auto *s : sortedStops)
        {
            auto &pos = stopsPositions[s->name];
            pos.y = yCurr;
            yCurr += yStep;
        }
    }
    return stopsPositions;
}

Svg::ZipWithGluingStopMapper::ZipWithGluingStopMapper(const RenderSettings &renderSettings, const std::vector<const Descriptions::Bus *> &buses) :
    _renderSettings(renderSettings),
    _buses(buses),
    _stopsPositions(),
    _sortedStops()
{}

std::map<std::string, Point> Svg::ZipWithGluingStopMapper::Map(const std::vector<const Descriptions::Stop *> &stops)
{
    if (stops.empty())
        return {};

    FillStopsPositions(stops);
    FillSortedStops(stops);

    {
        SortStopsByLongitude();
        auto indexToStops = CalculateIndexToStops();
        CalculateX(indexToStops);
    }

    {
        SortStopsByLatitude();
        auto indexToStops = CalculateIndexToStops();
        CalculateY(indexToStops);
    }
    auto result = std::move(_stopsPositions);
    ClearCache();
    return result;
}

void Svg::ZipWithGluingStopMapper::FillSortedStops(const std::vector<const Descriptions::Stop *> &stops)
{
    _sortedStops.reserve(stops.size());
    std::copy(cbegin(stops), cend(stops), std::back_inserter(_sortedStops));
}

void Svg::ZipWithGluingStopMapper::SortStopsByLongitude()
{
    std::sort(begin(_sortedStops), end(_sortedStops), [](const Descriptions::Stop *lhs, const Descriptions::Stop *rhs)
              { return lhs->position.longitude < rhs->position.longitude; });
}

void Svg::ZipWithGluingStopMapper::SortStopsByLatitude()
{
    std::sort(begin(_sortedStops), end(_sortedStops), [](const Descriptions::Stop *lhs, const Descriptions::Stop *rhs)
              { return lhs->position.latitude < rhs->position.latitude; });
}

std::vector<std::vector<const Descriptions::Stop *>> Svg::ZipWithGluingStopMapper::CalculateIndexToStops()
{
    std::vector<std::vector<const Descriptions::Stop *>> indexToStops;
    size_t currIndex = 0;
    indexToStops.emplace_back();
    indexToStops.front().push_back(_sortedStops.front());
    for (size_t i = 1; i < _sortedStops.size(); i++)
    {
        for (const auto *s : indexToStops[currIndex])
        {
            if (IsRouteNeighbors(_sortedStops[i], s))
            {
                currIndex++;
                indexToStops.emplace_back();
                break;
            }
        }
        indexToStops[currIndex].push_back(_sortedStops[i]);
    }
    return indexToStops;
}

void Svg::ZipWithGluingStopMapper::CalculateX(const std::vector<std::vector<const Descriptions::Stop*>>& indexToStops)
{
    double xCurr = _renderSettings.padding;
    double xStep = indexToStops.size() > 1 ? (_renderSettings.maxMapWidth - 2 * _renderSettings.padding) / (indexToStops.size() - 1) : 0.0;
    for (const auto& stops : indexToStops)
    {
        for (const auto *s : stops)
        {
            auto &pos = _stopsPositions[s->name];
            pos.x = xCurr;
        }
        xCurr += xStep;
    }
}

void Svg::ZipWithGluingStopMapper::CalculateY(const std::vector<std::vector<const Descriptions::Stop *>> &indexToStops)
{
    double yCurr = _renderSettings.maxMapHeight - _renderSettings.padding;
    double yStep = indexToStops.size() > 1 ? (_renderSettings.maxMapHeight - 2 * _renderSettings.padding) / (indexToStops.size() - 1) : 0.0;
    for (const auto &stops : indexToStops)
    {
        for (const auto *s : stops)
        {
            auto &pos = _stopsPositions[s->name];
            pos.y = yCurr;
        }
        yCurr -= yStep;
    }    
}

bool Svg::ZipWithGluingStopMapper::IsRouteNeighbors(const Descriptions::Stop *lhs, const Descriptions::Stop *rhs) const
{
    for(const auto* bus : _buses)
    {
        for(int i = 0; i < bus->stops.size() - 1; i++)
        {
            const auto& stopName = bus->stops[i];
            const auto& nextStopName = bus->stops[i + 1];
            if((lhs->name == stopName && rhs->name == nextStopName) ||
                lhs->name == nextStopName && rhs->name == stopName)
            {
                return true;
            }
        }
    }
    return false;
}

void Svg::ZipWithGluingStopMapper::ClearCache()
{
    _stopsPositions = {};
    _sortedStops = {};
}

void Svg::ZipWithGluingStopMapper::FillStopsPositions(const std::vector<const Descriptions::Stop *> &stops)
{
    for (const auto *s : stops)
    {
        _stopsPositions.insert({s->name, Point{}});
    }
}
