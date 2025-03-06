#pragma once

#include <vector>
#include "transport_catalog.pb.h"
#include "Point.pb.h"

class TransportDatabase;
namespace Descriptions
{
    struct Bus;
    struct Stop;
}
namespace Sphere
{
    struct Point;
}

namespace Serialization
{
    class ProtoMapper
    {
    public:
        ProtoMapper() = delete;
    
        static TransportCatalog Map(const TransportDatabase& db);
        static TransportDatabase Map(const TransportCatalog& catalog);
    
        static Serialization::Bus Map(const Descriptions::Bus& bus);
        static Descriptions::Bus Map(const Serialization::Bus& pbBus);
        static std::vector<Serialization::Bus> Map(const std::vector<const Descriptions::Bus*>& buses);
        static std::vector<Descriptions::Bus> Map(const std::vector<Serialization::Bus>& pbBuses);
    
        static Serialization::Stop Map(const Descriptions::Stop& stop);
        static Descriptions::Stop Map(const Serialization::Stop& pbStop);
        static std::vector<Serialization::Stop> Map(const std::vector<const Descriptions::Stop*>& stops);
        static std::vector<Descriptions::Stop> Map(const std::vector<Serialization::Stop>& pbStops);
    
        static Serialization::Point Map(const Sphere::Point& point);
        static Sphere::Point Map(const Serialization::Point& pbPoint);  
    };
}