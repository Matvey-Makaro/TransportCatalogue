#include "transport_catalog.h"

#include <sstream>

using namespace std;

TransportCatalog::TransportCatalog(vector<Descriptions::InputQuery> data,
                                   const Json::Dict& routing_settings_json) :
    _busesDescr(),
    _stopsDescr(),
    _stops(),
    _buses(),
    _router()
{
    auto stops_end = partition(begin(data), end(data), [](const auto& item) {
        return holds_alternative<Descriptions::Stop>(item);
    });

    Descriptions::StopsDict stops_dict;
    Range stopsRange(begin(data), stops_end);
    _stopsDescr.reserve(std::distance(stopsRange.begin(), stopsRange.end()));
    for (const auto& item : stopsRange) {
        const auto& stop = get<Descriptions::Stop>(item);
        _stopsDescr.push_back(stop);
        stops_dict[stop.name] = &stop;
        _stops.insert({stop.name, {}});
    }

    Descriptions::BusesDict buses_dict;
    Range busesRange(stops_end, end(data));
    _busesDescr.reserve(std::distance(busesRange.begin(), busesRange.end()));
    for (const auto& item : busesRange) {
        const auto& bus = get<Descriptions::Bus>(item);
        _busesDescr.push_back(bus);
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

    _router = make_unique<TransportRouter>(stops_dict, buses_dict, routing_settings_json);
}

const TransportCatalog::Stop* TransportCatalog::GetStop(const string& name) const {
    return GetValuePointer(_stops, name);
}

const TransportCatalog::Bus* TransportCatalog::GetBus(const string& name) const {
    return GetValuePointer(_buses, name);
}

std::vector<const Descriptions::Bus*> TransportCatalog::GetBusesDescriptions() const
{
    std::vector<const Descriptions::Bus*> buses;
    buses.reserve(_busesDescr.size());
    for(const auto& bus : _busesDescr)
        buses.push_back(&bus);
    return buses;
}

std::vector<const Descriptions::Stop*> TransportCatalog::GetStopsDescriptions() const
{
    std::vector<const Descriptions::Stop*> stops;
    stops.reserve(_stops.size());
    for(const auto& stop : _stopsDescr)
        stops.push_back(&stop);
    return stops;
}

optional<TransportRouter::RouteInfo> TransportCatalog::FindRoute(const string& stopFrom, const string& stopTo) const {
    return _router->FindRoute(stopFrom, stopTo);
}

int TransportCatalog::ComputeRoadRouteLength(
    const vector<string>& stops,
    const Descriptions::StopsDict& stops_dict
    ) {
    int result = 0;
    for (size_t i = 1; i < stops.size(); ++i) {
        result += Descriptions::ComputeStopsDistance(*stops_dict.at(stops[i - 1]), *stops_dict.at(stops[i]));
    }
    return result;
}

double TransportCatalog::ComputeGeoRouteDistance(
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
