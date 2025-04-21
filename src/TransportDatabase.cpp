#include "TransportDatabase.h"
#include "RenderSettings.h"
#include "YellowPages/Company.h"
#include "ProgramState.h"
#include <sstream>


using namespace std;
using namespace Router;

TransportDatabase::TransportDatabase(Descriptions::InputQueries data,
    const Router::RoutingSettings& routingSettings,
    const Visualization::RenderSettings& renderSettings) :
    _busesDescr(std::move(data.buses)),
    _stopsDescr(std::move(data.stops)),
    _stops(),
    _buses(),
    _routingSettings(routingSettings),
    _router(),
    _renderSettings(renderSettings)
{
    REGISTER_CURR_FUNC();
    Descriptions::StopsDict stops_dict;
    for (const auto& stop : _stopsDescr) {
        stops_dict[stop.name] = &stop;
        _stops.insert({ stop.name, {} });
    }

    Descriptions::BusesDict buses_dict;
    for (const auto& bus : _busesDescr) {
        buses_dict[bus.name] = &bus;
        _buses[bus.name] = Bus{
            bus.stops.size(),
            ComputeUniqueItemsCount(AsRange(bus.stops)),
            ComputeRoadRouteLength(bus.stops, stops_dict),
            ComputeGeoRouteDistance(bus.stops, stops_dict)
        };

        for (const string& stop_name : bus.stops) {
            _stops.at(stop_name).busNames.insert(bus.name);
        }
    }

    _router = make_unique<Router::TransportRouter>(stops_dict, buses_dict, _routingSettings);
}

const TransportDatabase::Stop* TransportDatabase::GetStop(const string& name) const {
    REGISTER_CURR_FUNC();
    return GetValuePointer(_stops, name);
}

const TransportDatabase::Bus* TransportDatabase::GetBus(const string& name) const {
    REGISTER_CURR_FUNC();
    return GetValuePointer(_buses, name);
}

std::vector<const Descriptions::Bus*> TransportDatabase::GetBusesDescriptions() const
{
    REGISTER_CURR_FUNC();
    std::vector<const Descriptions::Bus*> buses;
    buses.reserve(_busesDescr.size());
    for (const auto& bus : _busesDescr)
        buses.push_back(&bus);
    return buses;
}

std::vector<const Descriptions::Stop*> TransportDatabase::GetStopsDescriptions() const
{
    REGISTER_CURR_FUNC();
    std::vector<const Descriptions::Stop*> stops;
    stops.reserve(_stops.size());
    for (const auto& stop : _stopsDescr)
        stops.push_back(&stop);
    return stops;
}

optional<TransportRouter::RouteInfo> TransportDatabase::FindRoute(const string& stopFrom, const string& stopTo) const {
    REGISTER_CURR_FUNC();
    return _router->FindRoute(stopFrom, stopTo);
}

int TransportDatabase::ComputeRoadRouteLength(
    const vector<string>& stops,
    const Descriptions::StopsDict& stops_dict) {
    REGISTER_CURR_FUNC();
    int result = 0;
    for (size_t i = 1; i < stops.size(); ++i) {
        result += Descriptions::ComputeStopsDistance(*stops_dict.at(stops[i - 1]), *stops_dict.at(stops[i]));
    }
    return result;
}

double TransportDatabase::ComputeGeoRouteDistance(
    const vector<string>& stops,
    const Descriptions::StopsDict& stops_dict) {
    REGISTER_CURR_FUNC();
    double result = 0;
    for (size_t i = 1; i < stops.size(); ++i) {
        result += Sphere::Distance(
            stops_dict.at(stops[i - 1])->position, stops_dict.at(stops[i])->position
        );
    }
    return result;
}

const Router::RoutingSettings& TransportDatabase::GetRoutingSettings() const
{
    REGISTER_CURR_FUNC();
    return _routingSettings;
}

const Visualization::RenderSettings& TransportDatabase::GetRenderSettings() const
{
    REGISTER_CURR_FUNC();
    return _renderSettings;
}

std::optional<Router::TransportRouter::RouteInfo> TransportDatabase::FindRouteToCompany(const std::string& stopFrom,
    const std::vector<const YellowPages::BLL::Company*>& companies) const
{
    REGISTER_CURR_FUNC();
    std::optional<Router::TransportRouter::RouteInfo> bestRoute;
    for(const auto* company : companies)
    {
        if(company == nullptr)
        {
            std::cerr << __FILE__ << ":" << __LINE__ << " Company is nullptr" << std::endl;
            continue;
        }
        for(const auto& nearbyStop : company->nearbyStops)
        {
            double walkTime = nearbyStop.meters / ConvertToMetersPerMin(_routingSettings.pedestrianVelocity); // m / (m/min) = min  
            if(auto route = FindRoute(stopFrom, nearbyStop.name); route.has_value())
            {
                if(!bestRoute || bestRoute->total_time > route->total_time + walkTime)
                {
                    bestRoute = route;
                    bestRoute->items.emplace_back(TransportRouter::RouteInfo::WalkToCompany{
                        .time = walkTime,
                        .stop_name = nearbyStop.name,
                        .company_name = company->GetMainName().value
                    });
                    bestRoute->total_time += walkTime;
                }
            }
        }
    }
    return bestRoute;
}
