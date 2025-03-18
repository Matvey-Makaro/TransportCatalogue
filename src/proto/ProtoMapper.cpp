#include "ProtoMapper.h"
#include "TransportDatabase.h"
#include "Descriptions.h"
#include "Sphere.h"

using namespace Serialization;

TransportCatalog Serialization::ProtoMapper::Map(const TransportDatabase& db)
{
    auto pbBuses = Map(db.GetBusesDescriptions());
    auto pbStops = Map(db.GetStopsDescriptions());
    TransportCatalog catalog;
    for(auto&& pbBus : pbBuses)
    {
        *catalog.add_buses() = std::move(pbBus);
    }
    for(auto&& pbStop : pbStops)
    {
        *catalog.add_stops() = std::move(pbStop);
    }
    return catalog;
}

TransportDatabase Serialization::ProtoMapper::Map(const TransportCatalog& catalog)
{
    std::vector<Descriptions::Bus> buses;
    buses.reserve(catalog.buses_size());
    for (const auto& pbBus : catalog.buses())
    {
        buses.emplace_back(Map(pbBus));
    }
    std::vector<Descriptions::Stop> stops;
    stops.reserve(catalog.stops_size());
    for (const auto& pbStop : catalog.stops())
    {
        stops.emplace_back(Map(pbStop));
    }

    // TODO: Временное решение, дальше будет исправлено
    Json::Dict tmpDefaultRoutingSettingsJson;
    tmpDefaultRoutingSettingsJson["bus_wait_time"] = 5;
    tmpDefaultRoutingSettingsJson["bus_velocity"] = 30.5;

    return TransportDatabase(
        Descriptions::InputQueries{ .buses = std::move(buses), .stops = std::move(stops) },
        tmpDefaultRoutingSettingsJson
    );
}

Serialization::Bus Serialization::ProtoMapper::Map(const Descriptions::Bus& bus)
{
    Serialization::Bus pbBus;
    pbBus.set_name(bus.name);
    pbBus.set_isroundtrip(bus.isRoundtrip);
    for (const auto& stopNames : bus.stops)
    {
        pbBus.add_stops(stopNames);
    };
    return pbBus;
}

Descriptions::Bus Serialization::ProtoMapper::Map(const Serialization::Bus& pbBus)
{
    std::vector<std::string> stopNames;
    stopNames.reserve(pbBus.stops_size());
    for (const auto& stopName : pbBus.stops())
    {
        stopNames.emplace_back(stopName);
    }
    return Descriptions::Bus{
        .name = pbBus.name(),
        .stops = std::move(stopNames),
        .isRoundtrip = pbBus.isroundtrip()
    };
}

std::vector<Serialization::Bus> Serialization::ProtoMapper::Map(const std::vector<const Descriptions::Bus*>& buses)
{
    std::vector<Serialization::Bus> pbBuses;
    pbBuses.reserve(buses.size());
    for (const auto* bus : buses)
    {
        pbBuses.emplace_back(Map(*bus));
    }
    return pbBuses;
}

std::vector<Descriptions::Bus> Serialization::ProtoMapper::Map(const std::vector<Serialization::Bus>& pbBuses)
{
    std::vector<Descriptions::Bus> buses;
    buses.reserve(pbBuses.size());
    for (const auto& pbBus : pbBuses)
    {
        buses.emplace_back(Map(pbBus));
    }
    return buses;
}

Serialization::Stop Serialization::ProtoMapper::Map(const Descriptions::Stop& stop)
{
    Serialization::Stop pbStop;
    pbStop.set_name(stop.name);
    *pbStop.mutable_position() = Map(stop.position);
    for (const auto& [stopName, distance] : stop.distances)
    {
        Serialization::Stop::NameDistance nameDistance;
        nameDistance.set_name(stopName);
        nameDistance.set_distance(distance);
        *pbStop.add_distances() = std::move(nameDistance);
    }
    return pbStop;
}

Descriptions::Stop Serialization::ProtoMapper::Map(const Serialization::Stop& pbStop)
{
    std::unordered_map<std::string, int> distances;
    distances.reserve(pbStop.distances_size());
    for (const auto& dist : pbStop.distances())
    {
        distances.insert({ dist.name(), dist.distance() });
    }
    return Descriptions::Stop{
        .name = pbStop.name(),
        .position = Map(pbStop.position()),
        .distances = std::move(distances)
    };
}

std::vector<Serialization::Stop> Serialization::ProtoMapper::Map(const std::vector<const Descriptions::Stop*>& stops)
{
    std::vector<Serialization::Stop> pbStops;
    pbStops.reserve(stops.size());
    for (const auto* s : stops)
    {
        pbStops.emplace_back(Map(*s));
    }
    return pbStops;
}

std::vector<Descriptions::Stop> Serialization::ProtoMapper::Map(const std::vector<Serialization::Stop>& pbStops)
{
    std::vector<Descriptions::Stop> stops;
    stops.reserve(pbStops.size());
    for (const auto& pbStop : pbStops)
    {
        stops.emplace_back(Map(pbStop));
    }
    return stops;
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