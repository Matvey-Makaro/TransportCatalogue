#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include "StopsMappers.h"

using namespace Svg;

Svg::GeoStopMapper::GeoStopMapper(const Visualization::RenderSettings &renderSettings) : _renderSettings(renderSettings),
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

Svg::ZipStopMapper::ZipStopMapper(const Visualization::RenderSettings &renderSettings) : _renderSettings(renderSettings)
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

Svg::ZipWithGluingStopMapper::ZipWithGluingStopMapper(const Visualization::RenderSettings &renderSettings, const std::vector<const Descriptions::Bus *> &buses) : _renderSettings(renderSettings),
                                                                                                                                                   _buses(buses),
                                                                                                                                                   _stopsPositions(),
                                                                                                                                                   _sortedStops()
{
}

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

void Svg::ZipWithGluingStopMapper::CalculateX(const std::vector<std::vector<const Descriptions::Stop *>> &indexToStops)
{
    double xCurr = _renderSettings.padding;
    double xStep = indexToStops.size() > 1 ? (_renderSettings.maxMapWidth - 2 * _renderSettings.padding) / (indexToStops.size() - 1) : 0.0;
    for (const auto &stops : indexToStops)
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
    for (const auto *bus : _buses)
    {
        for (int i = 0; i < bus->stops.size() - 1; i++)
        {
            const auto &stopName = bus->stops[i];
            const auto &nextStopName = bus->stops[i + 1];
            if ((lhs->name == stopName && rhs->name == nextStopName) ||
                (lhs->name == nextStopName && rhs->name == stopName))
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

Svg::InterpolationZipWithGluingStopMapper::InterpolationZipWithGluingStopMapper(const Visualization::RenderSettings &renderSettings, const std::vector<const Descriptions::Bus *> &buses) : _renderSettings(renderSettings), _buses(buses)
{
}

std::map<std::string, Point> Svg::InterpolationZipWithGluingStopMapper::Map(const std::vector<const Descriptions::Stop *> &stops)
{
    FillStops(stops);
    auto pivotStops = CalculatePivotStops();
    RecalculateNonPivotStopsPosition(pivotStops);
    auto mappedStops = ZipWithGluing();
    ClearCache();
    return mappedStops;
}

void Svg::InterpolationZipWithGluingStopMapper::FillStops(const std::vector<const Descriptions::Stop *> &stops)
{
    _stops.reserve(stops.size());
    for (const auto *s : stops)
    {
        _stops.insert({s->name, *s});
    }
}

std::unordered_set<std::string> Svg::InterpolationZipWithGluingStopMapper::CalculatePivotStops()
{
    std::unordered_set<std::string> pivotStops;
    for (const auto *bus : _buses)
    {
        auto terminals = bus->GetTerminals();
        pivotStops.insert(std::move(terminals.first));
        pivotStops.insert(std::move(terminals.second));
    }

    std::unordered_map<std::string, size_t> nameToEntriesCount;
    for (const auto *bus : _buses)
    {
        std::unordered_set<std::string> processedStops;
        for (const auto &stopName : bus->stops)
        {
            if (nameToEntriesCount.count(stopName) && !processedStops.count(stopName))
            {
                pivotStops.insert(stopName);
                continue;
            }
            nameToEntriesCount[stopName] += 1;
            processedStops.insert(stopName);
        }
    }

    constexpr size_t minPivotStopEntriesCount = 3;
    for (const auto &[stopName, entriesCount] : nameToEntriesCount)
    {
        if (entriesCount >= minPivotStopEntriesCount)
        {
            pivotStops.insert(stopName);
        }
    }
    return pivotStops;
}

void Svg::InterpolationZipWithGluingStopMapper::RecalculateNonPivotStopsPosition(const std::unordered_set<std::string> &pivotStops)
{
    for (const auto *bus : _buses)
    {
        size_t startIndex = 0;
        // First stop is always pivot
        const auto &stopNames = bus->stops;
        for (size_t currIndex = 1; currIndex < stopNames.size(); currIndex++)
        {
            if (pivotStops.count(stopNames[currIndex]))
            {
                auto &startStop = _stops[stopNames[startIndex]];
                auto &finishStop = _stops[stopNames[currIndex]];
                double lonStep = (finishStop.position.longitude - startStop.position.longitude) / (currIndex - startIndex);
                double latStep = (finishStop.position.latitude - startStop.position.latitude) / (currIndex - startIndex);
                double lonCurr = startStop.position.longitude + lonStep;
                double latCurr = startStop.position.latitude + latStep;

                for (size_t k = startIndex + 1; k < currIndex; k++)
                {
                    auto &stop = _stops[stopNames[k]];
                    stop.position.longitude = lonCurr;
                    stop.position.latitude = latCurr;
                    lonCurr += lonStep;
                    latCurr += latStep;
                }
                startIndex = currIndex;
            }
        }
    }
}

std::map<std::string, Point> Svg::InterpolationZipWithGluingStopMapper::ZipWithGluing()
{
    std::vector<const Descriptions::Stop *> stopsWithNewPositions;
    stopsWithNewPositions.reserve(_stops.size());
    for (const auto &[_, stop] : _stops)
    {
        stopsWithNewPositions.emplace_back(&stop);
    }
    ImprovedZipWithGluingStopMapper zipWithGluingStopMapper(_renderSettings, _buses);
    return zipWithGluingStopMapper.Map(stopsWithNewPositions);
}

void Svg::InterpolationZipWithGluingStopMapper::ClearCache()
{
    _stops = {};
}

std::vector<std::vector<const Descriptions::Stop *>> Svg::ImprovedZipWithGluingStopMapper::CalculateIndexToStops()
{
    std::vector<std::vector<const Descriptions::Stop *>> indexToStops;
    indexToStops.emplace_back();
    indexToStops.front().push_back(_sortedStops.front());
    for (size_t sortedStopsIndex = 1; sortedStopsIndex < _sortedStops.size(); sortedStopsIndex++)
    {
        size_t currStopIndex = 0;
        bool isCurrStopIndexCalculated = false;
        for (int i = indexToStops.size() - 1; i >= 0 && !isCurrStopIndexCalculated; i--)
        {
            for (const auto *s : indexToStops[i])
            {
                if (IsRouteNeighbors(_sortedStops[sortedStopsIndex], s))
                {
                    currStopIndex = i + 1;
                    isCurrStopIndexCalculated = true;
                    break;
                }
            }
        }

        if (currStopIndex == indexToStops.size())
        {
            indexToStops.emplace_back();
        }
        indexToStops[currStopIndex].push_back(_sortedStops[sortedStopsIndex]);
    }
    return indexToStops;
}
