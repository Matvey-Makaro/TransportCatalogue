#include "TransportDatabase.h"

#include <sstream>

using namespace std;
using namespace Router;

TransportDatabase::TransportDatabase(Descriptions::InputQueries data,
    const Router::RoutingSettings& routingSettings) :
    _busesDescr(std::move(data.buses)),
    _stopsDescr(std::move(data.stops)),
    _stops(),
    _buses(),
    _routingSettings(routingSettings),
    _router()
{
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
            _stops.at(stop_name).bus_names.insert(bus.name);
        }
    }

    _router = make_unique<Router::TransportRouter>(stops_dict, buses_dict, _routingSettings);
}

const TransportDatabase::Stop* TransportDatabase::GetStop(const string& name) const {
    return GetValuePointer(_stops, name);
}

const TransportDatabase::Bus* TransportDatabase::GetBus(const string& name) const {
    return GetValuePointer(_buses, name);
}

std::vector<const Descriptions::Bus*> TransportDatabase::GetBusesDescriptions() const
{
    std::vector<const Descriptions::Bus*> buses;
    buses.reserve(_busesDescr.size());
    for (const auto& bus : _busesDescr)
        buses.push_back(&bus);
    return buses;
}

std::vector<const Descriptions::Stop*> TransportDatabase::GetStopsDescriptions() const
{
    std::vector<const Descriptions::Stop*> stops;
    stops.reserve(_stops.size());
    for (const auto& stop : _stopsDescr)
        stops.push_back(&stop);
    return stops;
}

optional<TransportRouter::RouteInfo> TransportDatabase::FindRoute(const string& stopFrom, const string& stopTo) const {
    return _router->FindRoute(stopFrom, stopTo);
}

int TransportDatabase::ComputeRoadRouteLength(
    const vector<string>& stops,
    const Descriptions::StopsDict& stops_dict
) {
    int result = 0;
    for (size_t i = 1; i < stops.size(); ++i) {
        result += Descriptions::ComputeStopsDistance(*stops_dict.at(stops[i - 1]), *stops_dict.at(stops[i]));
    }
    return result;
}

double TransportDatabase::ComputeGeoRouteDistance(
    const vector<string>& stops,
    const Descriptions::StopsDict& stops_dict
) {
    double result = 0;
    for (size_t i = 1; i < stops.size(); ++i) {
        result += Sphere::Distance(
            stops_dict.at(stops[i - 1])->position, stops_dict.at(stops[i])->position
        );
    }
    return result;
}

Router::RoutingSettings TransportDatabase::GetRoutingSettings() const
{
    return _routingSettings;
}
