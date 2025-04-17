#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include "StopsMappers.h"
#include "YellowPages/Company.h"
#include "Utils.h"

using namespace Svg;

template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

Svg::GeoStopMapper::GeoStopMapper(const Visualization::RenderSettings& renderSettings) : _renderSettings(renderSettings),
_coordMapper()
{
}

std::pair<GeoStopMapper::StopNameToCoords, GeoStopMapper::CompanyNameToCoords> GeoStopMapper::Map(
    const std::vector<const Descriptions::Stop*>& stops,
    const std::vector<const YellowPages::BLL::Company*>& companies)
{
    SetupCoordinateMapper(stops);
    std::map<std::string, Point> stopPositions;
    for (const auto* s : stops)
    {
        stopPositions.insert({ s->name, _coordMapper.Map(s->position) });
    }
    std::map<std::string, Point> companyPositions;
    for (const auto* c : companies)
    {
        companyPositions.insert({ c->GetMainName().value, _coordMapper.Map(c->address.coords) });
    }
    return std::make_pair(std::move(stopPositions), std::move(companyPositions));
}

void Svg::GeoStopMapper::SetupCoordinateMapper(const std::vector<const Descriptions::Stop*>& stops)
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

Svg::ZipStopMapper::ZipStopMapper(const Visualization::RenderSettings& renderSettings) : _renderSettings(renderSettings)
{
}

std::pair<Svg::ZipStopMapper::StopNameToCoords, Svg::ZipStopMapper::CompanyNameToCoords> Svg::ZipStopMapper::Map(
    const std::vector<const Descriptions::Stop*>& stops,
    const std::vector<const YellowPages::BLL::Company*>& companies)
{
    std::vector<const Descriptions::Stop*> sortedStops(cbegin(stops), cend(stops));
    std::sort(begin(sortedStops), end(sortedStops), [](const Descriptions::Stop* lhs, const Descriptions::Stop* rhs)
        { return lhs->position.longitude < rhs->position.longitude; });
    std::map<std::string, Point> stopsPositions;
    for (const auto* s : stops)
    {
        stopsPositions.insert({ s->name, Point{} });
    }

    {
        double xCurr = _renderSettings.padding;
        double xStep = stopsPositions.size() > 1 ? (_renderSettings.maxMapWidth - 2 * _renderSettings.padding) / (stopsPositions.size() - 1) : 0.0;
        for (const auto* s : sortedStops)
        {
            auto& pos = stopsPositions[s->name];
            pos.x = xCurr;
            xCurr += xStep;
        }
    }

    std::sort(begin(sortedStops), end(sortedStops), [](const Descriptions::Stop* lhs, const Descriptions::Stop* rhs)
        { return lhs->position.latitude > rhs->position.latitude; });

    {
        double yCurr = stopsPositions.size() > 1 ? _renderSettings.padding : (_renderSettings.maxMapHeight - _renderSettings.padding);
        double yStep = stopsPositions.size() > 1 ? (_renderSettings.maxMapHeight - 2 * _renderSettings.padding) / (stopsPositions.size() - 1) : 0.0;
        for (const auto* s : sortedStops)
        {
            auto& pos = stopsPositions[s->name];
            pos.y = yCurr;
            yCurr += yStep;
        }
    }

    CompanyNameToCoords companyPositions;
    for (const auto* c : companies)
    {
        Svg::Point point{
            .x = c->address.coords.latitude,
            .y = c->address.coords.longitude
        };
        companyPositions.insert({ c->GetMainName().value, point });
    }
    return std::make_pair(std::move(stopsPositions), std::move(companyPositions));
}

Svg::ZipWithGluingStopMapper::ZipWithGluingStopMapper(const Visualization::RenderSettings& renderSettings, const std::vector<const Descriptions::Bus*>& buses) : _renderSettings(renderSettings),
_buses(buses),
_stopsPositions(),
_sortedTransportPoints()
{
}

ZipWithGluingStopMapper::CoordsPair ZipWithGluingStopMapper::Map(
    const std::vector<const Descriptions::Stop*>& stops,
    const std::vector<const YellowPages::BLL::Company*>& companies)
{
    // TODO: Координаты остановок и компаний могут совпадать, надо это учесть в коде 
    if (stops.empty())
        return {};

    FillStopsPositions(stops);
    FillCompaniesPositions(companies);
    FillSortedTransportPoints(stops, companies);

    {
        SortStopsByLongitude();
        auto indexToStops = CalculateIndexToStops(Dimension::x);
        CalculateX(indexToStops);
    }

    {
        SortStopsByLatitude();
        auto indexToStops = CalculateIndexToStops(Dimension::y);
        CalculateY(indexToStops);
    }
    auto result = std::make_pair(std::move(_stopsPositions), std::move(_companiesPositions));
    ClearCache();
    return result;
}

void Svg::ZipWithGluingStopMapper::FillSortedTransportPoints(const std::vector<const Descriptions::Stop*>& stops,
    const std::vector<const YellowPages::BLL::Company*>& companies)
{
    _sortedTransportPoints.reserve(stops.size() + companies.size());
    for (const auto* s : stops)
    {
        _sortedTransportPoints.emplace_back(s);
    }
    for (const auto* c : companies)
    {
        _sortedTransportPoints.emplace_back(c);
    }
}

void Svg::ZipWithGluingStopMapper::SortStopsByLongitude()
{
    std::sort(begin(_sortedTransportPoints), end(_sortedTransportPoints), [](const TransportPoint& lhs, const TransportPoint& rhs)
        { return lhs.GetCoords().longitude < rhs.GetCoords().longitude; });
}

void Svg::ZipWithGluingStopMapper::SortStopsByLatitude()
{
    std::sort(begin(_sortedTransportPoints), end(_sortedTransportPoints), [](const TransportPoint& lhs, const TransportPoint& rhs)
        { return lhs.GetCoords().latitude < rhs.GetCoords().latitude; });
}

std::vector<std::vector<Svg::ZipWithGluingStopMapper::TransportPoint*>> Svg::ZipWithGluingStopMapper::CalculateIndexToStops(Dimension dimension)
{
    std::vector<std::vector<TransportPoint*>> indexToStops;
    size_t currIndex = 0;
    indexToStops.emplace_back();
    indexToStops.front().push_back(&_sortedTransportPoints.front());
    for (size_t i = 1; i < _sortedTransportPoints.size(); i++)
    {
        for (const auto* s : indexToStops[currIndex])
        {
            auto isSameAsPrev = dimension == Dimension::x ?
                IsEqualRel(_sortedTransportPoints[i].GetCoords().longitude, s->GetCoords().longitude) :
                IsEqualRel(_sortedTransportPoints[i].GetCoords().latitude, s->GetCoords().latitude);
            if (isSameAsPrev)
            {
                break;
            }
            if (IsNeighbors(_sortedTransportPoints[i], *s))
            {
                currIndex++;
                indexToStops.emplace_back();
                break;
            }
        }
        indexToStops[currIndex].push_back(&_sortedTransportPoints[i]);
    }
    return indexToStops;
}

bool Svg::ZipWithGluingStopMapper::IsNeighbors(const TransportPoint& lhs, const TransportPoint& rhs) const
{
    return std::visit([this](auto&& lhs, auto&& rhs)
        {
            return IsNeighbors(lhs, rhs);
        }, lhs.GetEntity(), rhs.GetEntity());
}

bool Svg::ZipWithGluingStopMapper::IsNeighbors(const YellowPages::BLL::Company* lhs, const YellowPages::BLL::Company* rhs) const
{
    return false;
}

bool Svg::ZipWithGluingStopMapper::IsNeighbors(const Descriptions::Stop* lhs, const YellowPages::BLL::Company* rhs) const
{
    auto it = std::find_if(std::cbegin(rhs->nearbyStops), std::cend(rhs->nearbyStops), [lhs](const auto& nearbyStop)
        {
            return lhs->name == nearbyStop.name;
        });
    return it != std::cend(rhs->nearbyStops);
}

bool Svg::ZipWithGluingStopMapper::IsNeighbors(const YellowPages::BLL::Company* lhs, const Descriptions::Stop* rhs) const
{
    return IsNeighbors(rhs, lhs);
}


void Svg::ZipWithGluingStopMapper::CalculateX(const std::vector<std::vector<TransportPoint*>>& indexToTransportPoints)
{
    double xCurr = _renderSettings.padding;
    double xStep = indexToTransportPoints.size() > 1 ?
        (_renderSettings.maxMapWidth - 2 * _renderSettings.padding) / (indexToTransportPoints.size() - 1) : 0.0;
    for (const auto& trPoints : indexToTransportPoints)
    {
        for (const auto* p : trPoints)
        {
            auto& pos = std::visit(overloaded{
                [this](const Descriptions::Stop* stop) -> Svg::Point& { return _stopsPositions[stop->name]; },
                [this](const YellowPages::BLL::Company* company) -> Svg::Point& { return _companiesPositions[company->GetMainName().value]; }
            }, p->GetEntity());
            pos.x = xCurr;
        }
        xCurr += xStep;
    }
}

void Svg::ZipWithGluingStopMapper::CalculateY(const std::vector<std::vector<TransportPoint*>>& indexToTransportPoints)
{
    double yCurr = _renderSettings.maxMapHeight - _renderSettings.padding;
    double yStep = indexToTransportPoints.size() > 1 ? (_renderSettings.maxMapHeight - 2 * _renderSettings.padding) / (indexToTransportPoints.size() - 1) : 0.0;
    for (const auto& trPoints : indexToTransportPoints)
    {
        for (const auto* p : trPoints)
        {
            auto& pos = std::visit(overloaded{
                [this](const Descriptions::Stop* stop) -> Svg::Point& { return _stopsPositions[stop->name]; },
                [this](const YellowPages::BLL::Company* company) -> Svg::Point& { return _companiesPositions[company->GetMainName().value]; }
            }, p->GetEntity());
            pos.y = yCurr;
        }
        yCurr -= yStep;
    }
}

bool Svg::ZipWithGluingStopMapper::IsNeighbors(const Descriptions::Stop* lhs, const Descriptions::Stop* rhs) const
{
    for (const auto* bus : _buses)
    {
        for (int i = 0; i < bus->stops.size() - 1; i++)
        {
            const auto& stopName = bus->stops[i];
            const auto& nextStopName = bus->stops[i + 1];
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
    _companiesPositions = {};
    _sortedTransportPoints = {};
}

void Svg::ZipWithGluingStopMapper::FillStopsPositions(const std::vector<const Descriptions::Stop*>& stops)
{
    for (const auto* s : stops)
    {
        _stopsPositions.insert({ s->name, Point{} });
    }
}

void Svg::ZipWithGluingStopMapper::FillCompaniesPositions(
    const std::vector<const YellowPages::BLL::Company*>& companies)
{
    for (const auto* c : companies)
    {
        _companiesPositions.insert({ c->GetMainName().value, Point{} });
    }
}

Svg::InterpolationZipWithGluingStopMapper::InterpolationZipWithGluingStopMapper(const Visualization::RenderSettings& renderSettings, const std::vector<const Descriptions::Bus*>& buses) : _renderSettings(renderSettings), _buses(buses)
{
}

Svg::InterpolationZipWithGluingStopMapper::CoordsPair Svg::InterpolationZipWithGluingStopMapper::Map(
    const std::vector<const Descriptions::Stop*>& stops, const std::vector<const YellowPages::BLL::Company*>& companies)
{
    FillStops(stops);
    auto pivotStops = CalculatePivotStops();
    RecalculateNonPivotStopsPosition(pivotStops);
    auto result = ZipWithGluing(companies);
    ClearCache();
    return result;
}



void Svg::InterpolationZipWithGluingStopMapper::FillStops(const std::vector<const Descriptions::Stop*>& stops)
{
    _stops.reserve(stops.size());
    for (const auto* s : stops)
    {
        _stops.insert({ s->name, *s });
    }
}

std::unordered_set<std::string> Svg::InterpolationZipWithGluingStopMapper::CalculatePivotStops()
{
    std::unordered_set<std::string> pivotStops;
    for (const auto* bus : _buses)
    {
        auto terminals = bus->GetTerminals();
        pivotStops.insert(std::move(terminals.first));
        pivotStops.insert(std::move(terminals.second));
    }

    std::unordered_map<std::string, size_t> nameToEntriesCount;
    for (const auto* bus : _buses)
    {
        std::unordered_set<std::string> processedStops;
        for (const auto& stopName : bus->stops)
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
    for (const auto& [stopName, entriesCount] : nameToEntriesCount)
    {
        if (entriesCount >= minPivotStopEntriesCount)
        {
            pivotStops.insert(stopName);
        }
    }
    return pivotStops;
}

void Svg::InterpolationZipWithGluingStopMapper::RecalculateNonPivotStopsPosition(const std::unordered_set<std::string>& pivotStops)
{
    for (const auto* bus : _buses)
    {
        size_t startIndex = 0;
        // First stop is always pivot
        const auto& stopNames = bus->stops;
        for (size_t currIndex = 1; currIndex < stopNames.size(); currIndex++)
        {
            if (pivotStops.count(stopNames[currIndex]))
            {
                auto& startStop = _stops[stopNames[startIndex]];
                auto& finishStop = _stops[stopNames[currIndex]];
                double lonStep = (finishStop.position.longitude - startStop.position.longitude) / (currIndex - startIndex);
                double latStep = (finishStop.position.latitude - startStop.position.latitude) / (currIndex - startIndex);
                double lonCurr = startStop.position.longitude + lonStep;
                double latCurr = startStop.position.latitude + latStep;

                for (size_t k = startIndex + 1; k < currIndex; k++)
                {
                    auto& stop = _stops[stopNames[k]];
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

 Svg::InterpolationZipWithGluingStopMapper::CoordsPair Svg::InterpolationZipWithGluingStopMapper::ZipWithGluing(
    const std::vector<const YellowPages::BLL::Company*>& companies)
{
    std::vector<const Descriptions::Stop*> stopsWithNewPositions;
    stopsWithNewPositions.reserve(_stops.size());
    for (const auto& [_, stop] : _stops)
    {
        stopsWithNewPositions.emplace_back(&stop);
    }
    ImprovedZipWithGluingStopMapper zipWithGluingStopMapper(_renderSettings, _buses);
    return zipWithGluingStopMapper.Map(stopsWithNewPositions, companies);
}

void Svg::InterpolationZipWithGluingStopMapper::ClearCache()
{
    _stops = {};
}

std::vector<std::vector<Svg::ZipWithGluingStopMapper::TransportPoint*>> Svg::ImprovedZipWithGluingStopMapper::CalculateIndexToStops(Dimension dimension)
{
    std::vector<std::vector<TransportPoint*>> indexToStops;
    indexToStops.emplace_back();
    indexToStops.front().push_back(&_sortedTransportPoints.front());
    for (size_t sortedStopsIndex = 1; sortedStopsIndex < _sortedTransportPoints.size(); sortedStopsIndex++)
    {
        size_t currStopIndex = 0;
        bool isCurrStopIndexCalculated = false;
        for (int i = indexToStops.size() - 1; i >= 0 && !isCurrStopIndexCalculated; i--)
        {
            for (const auto* s : indexToStops[i])
            {
                auto isSameAsPrev = dimension == Dimension::x ?
                IsEqualRel(_sortedTransportPoints[sortedStopsIndex].GetCoords().longitude, s->GetCoords().longitude) :
                IsEqualRel(_sortedTransportPoints[sortedStopsIndex].GetCoords().latitude, s->GetCoords().latitude);
            if (isSameAsPrev)
            {
                break;
            }
                if (IsNeighbors(_sortedTransportPoints[sortedStopsIndex], *s))
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
        indexToStops[currStopIndex].push_back(&_sortedTransportPoints[sortedStopsIndex]);
    }
    return indexToStops;
}

Svg::ZipWithGluingStopMapper::TransportPoint::TransportPoint(const EntityType& entity) :
    _entity(entity)
{
}

const Svg::ZipWithGluingStopMapper::TransportPoint::EntityType Svg::ZipWithGluingStopMapper::TransportPoint::GetEntity() const
{
    return _entity;
}

const Sphere::Point& Svg::ZipWithGluingStopMapper::TransportPoint::GetCoords() const
{
    auto getStopCoords = [](const Descriptions::Stop* stop) -> const Sphere::Point&
    {
        return stop->position;
    };
    auto getCompanyCoords = [](const YellowPages::BLL::Company* company) -> const Sphere::Point&
    {
        return company->address.coords;
    };
    return std::visit(overloaded{ getStopCoords, getCompanyCoords }, _entity);
}

const std::string Svg::ZipWithGluingStopMapper::TransportPoint::GetName() const
{
    return std::visit(overloaded {
        [](const Descriptions::Stop* stop) { return stop->name; },
        [](const YellowPages::BLL::Company* company) { return company->GetMainName().value; }
    }, _entity);
}


