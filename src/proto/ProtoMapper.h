#pragma once

#include <vector>
#include "transport_catalog.pb.h"
#include "Bus.pb.h"
#include "Stop.pb.h"
#include "Point.pb.h"

class TransportDatabase;
namespace Descriptions
{
    struct Bus;
    struct Stop;
}
namespace Responses
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
    
        using Name = std::string;
        static Serialization::ResponseBus Map(std::string_view busName, const Responses::Bus& bus);
        static std::pair<Name, Responses::Bus> Map(const Serialization::ResponseBus& pbBus);
    
        static Serialization::ResponseStop Map(std::string_view stopName, const Responses::Stop& stop);
        static std::pair<Name, Responses::Stop> Map(const Serialization::ResponseStop& pbStop);
    
        static Serialization::Point Map(const Sphere::Point& point);
        static Sphere::Point Map(const Serialization::Point& pbPoint);  
    };
}