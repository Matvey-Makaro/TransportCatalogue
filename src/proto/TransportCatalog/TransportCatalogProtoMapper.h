#pragma once

#include <vector>
#include "transport_catalog.pb.h"
#include "Point.pb.h"
#include "TransportRouter.h"

class TransportDatabase;
struct RoutingSettings;
namespace Descriptions
{
    struct Bus;
    struct Stop;
}
namespace Sphere
{
    struct Point;
}

namespace Visualization
{
    struct RenderSettings;
}
namespace Svg
{
    class Color;
    struct Rgb;
    struct Rgba;
    struct Point;
}

namespace Serialization
{
    class TransportCatalogProtoMapper
    {
    public:
        TransportCatalogProtoMapper() = delete;
    
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

        static Serialization::RoutingSettings Map(const Router::RoutingSettings& settings);
        static Router::RoutingSettings Map(const Serialization::RoutingSettings& pbSettings);

        static Serialization::RenderSettings Map(const Visualization::RenderSettings& settings);
        static Visualization::RenderSettings Map(const Serialization::RenderSettings& pbSettings);

        static Serialization::SvgPoint Map(const Svg::Point& point);
        static Svg::Point Map(const Serialization::SvgPoint& pbPoint);

        static Serialization::SvgColor Map(const Svg::Color& color);
        static Svg::Color Map(const Serialization::SvgColor& pbColor);

        static Serialization::Rgb Map(const Svg::Rgb& rgb);
        static Svg::Rgb Map(const Serialization::Rgb& pbRgb);
        
        static Serialization::Rgba Map(const Svg::Rgba& rgba);
        static Svg::Rgba Map(const Serialization::Rgba& pbRgba);
    };
}