#include "ProtoMapper.h"
#include "TransportDatabase.h"
#include "Descriptions.h"
#include "Sphere.h"

using namespace Serialization;

TransportCatalog Serialization::ProtoMapper::Map(const TransportDatabase& db)
{
    TransportCatalog catalog;
    for(const auto& [busName, bus] : db.GetBusesResponses())
    {
        *catalog.add_buses() = Map(busName, bus);
    }
    for(const auto& [stopName, stop] : db.GetStopsResponses())
    {
        *catalog.add_stops() = Map(stopName, stop);
    }
    return catalog;
}

TransportDatabase Serialization::ProtoMapper::Map(const TransportCatalog& catalog)
{
    std::unordered_map<std::string, Responses::Bus> buses;
    for (const auto& pbBus : catalog.buses())
    {
        buses.insert(Map(pbBus));
    }

    std::unordered_map<std::string, Responses::Stop> stops;
    for (const auto& pbStop : catalog.stops())
    {
        stops.insert(Map(pbStop));
    }

    // TODO: Временное решение, дальше будет исправлено
    Json::Dict tmpDefaultRoutingSettingsJson;
    tmpDefaultRoutingSettingsJson["bus_wait_time"] = 5;
    tmpDefaultRoutingSettingsJson["bus_velocity"] = 30.5;

    return TransportDatabase(
        std::move(stops),
        std::move(buses),
        tmpDefaultRoutingSettingsJson
    );
}

Serialization::ResponseBus ProtoMapper::Map(std::string_view busName, const Responses::Bus& bus)
{
    ResponseBus pbBus;
    pbBus.set_name(std::string(busName));
    pbBus.set_stop_count(bus.stop_count);
    pbBus.set_unique_stop_count(bus.unique_stop_count);
    pbBus.set_road_route_length(bus.road_route_length);
    pbBus.set_geo_route_length(bus.geo_route_length);
    return pbBus;
}

std::pair<ProtoMapper::Name, Responses::Bus> ProtoMapper::Map(const Serialization::ResponseBus& pbBus)
{
    Responses::Bus bus {
        .stop_count = pbBus.stop_count(),
        .unique_stop_count = pbBus.unique_stop_count(),
        .road_route_length = pbBus.road_route_length(),
        .geo_route_length = pbBus.geo_route_length()
    };
    return std::make_pair(pbBus.name(), std::move(bus));
}

Serialization::ResponseStop ProtoMapper::Map(std::string_view stopName, const Responses::Stop& stop)
{
    ResponseStop pbStop;
    pbStop.set_name(std::string(stopName));
    for(const auto& busName : stop.bus_names)
    {
        pbStop.add_bus_names(busName);
    }
    return pbStop;
}

std::pair<ProtoMapper::Name, Responses::Stop> ProtoMapper::Map(const Serialization::ResponseStop& pbStop)
{
    Responses::Stop stop;
    for(const auto& busName : pbStop.bus_names())
    {
        stop.bus_names.insert(busName);
    }
    return std::make_pair(pbStop.name(), std::move(stop));
}

Serialization::Point Serialization::ProtoMapper::Map(const Sphere::Point& point)
{
    Serialization::Point pbPoing;
    pbPoing.set_latitude(point.latitude);
    pbPoing.set_longitude(point.longitude);
    return pbPoing;
}

Sphere::Point Serialization::ProtoMapper::Map(const Serialization::Point& pbPoint)
{
    return Sphere::Point{ .latitude = pbPoint.latitude(),
                            .longitude = pbPoint.longitude() };
}