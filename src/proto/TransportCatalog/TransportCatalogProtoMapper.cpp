#include "TransportCatalogProtoMapper.h"
#include "TransportDatabase.h"
#include "Descriptions.h"
#include "Sphere.h"
#include "RenderSettings.h"
#include "Svg/Rgb.h"
#include "Svg/Rgba.h"

using namespace Serialization;

TransportCatalog Serialization::TransportCatalogProtoMapper::Map(const TransportDatabase& db)
{
    auto pbBuses = Map(db.GetBusesDescriptions());
    auto pbStops = Map(db.GetStopsDescriptions());
    TransportCatalog catalog;
    for (auto&& pbBus : pbBuses)
    {
        *catalog.add_buses() = std::move(pbBus);
    }
    for (auto&& pbStop : pbStops)
    {
        *catalog.add_stops() = std::move(pbStop);
    }
    *catalog.mutable_routing_settings() = Map(db.GetRoutingSettings());
    *catalog.mutable_render_settings() = Map(db.GetRenderSettings());
    return catalog;
}

TransportDatabase Serialization::TransportCatalogProtoMapper::Map(const TransportCatalog& catalog)
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
    return TransportDatabase(
        Descriptions::InputQueries{ .buses = std::move(buses), .stops = std::move(stops) },
        Map(catalog.routing_settings()),
        Map(catalog.render_settings())
    );
}

Serialization::Bus Serialization::TransportCatalogProtoMapper::Map(const Descriptions::Bus& bus)
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

Descriptions::Bus Serialization::TransportCatalogProtoMapper::Map(const Serialization::Bus& pbBus)
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

std::vector<Serialization::Bus> Serialization::TransportCatalogProtoMapper::Map(const std::vector<const Descriptions::Bus*>& buses)
{
    std::vector<Serialization::Bus> pbBuses;
    pbBuses.reserve(buses.size());
    for (const auto* bus : buses)
    {
        pbBuses.emplace_back(Map(*bus));
    }
    return pbBuses;
}

std::vector<Descriptions::Bus> Serialization::TransportCatalogProtoMapper::Map(const std::vector<Serialization::Bus>& pbBuses)
{
    std::vector<Descriptions::Bus> buses;
    buses.reserve(pbBuses.size());
    for (const auto& pbBus : pbBuses)
    {
        buses.emplace_back(Map(pbBus));
    }
    return buses;
}

Serialization::Stop Serialization::TransportCatalogProtoMapper::Map(const Descriptions::Stop& stop)
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

Descriptions::Stop Serialization::TransportCatalogProtoMapper::Map(const Serialization::Stop& pbStop)
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

std::vector<Serialization::Stop> Serialization::TransportCatalogProtoMapper::Map(const std::vector<const Descriptions::Stop*>& stops)
{
    std::vector<Serialization::Stop> pbStops;
    pbStops.reserve(stops.size());
    for (const auto* s : stops)
    {
        pbStops.emplace_back(Map(*s));
    }
    return pbStops;
}

std::vector<Descriptions::Stop> Serialization::TransportCatalogProtoMapper::Map(const std::vector<Serialization::Stop>& pbStops)
{
    std::vector<Descriptions::Stop> stops;
    stops.reserve(pbStops.size());
    for (const auto& pbStop : pbStops)
    {
        stops.emplace_back(Map(pbStop));
    }
    return stops;
}

Serialization::Point Serialization::TransportCatalogProtoMapper::Map(const Sphere::Point& point)
{
    Serialization::Point pbPoing;
    pbPoing.set_latitude(point.latitude);
    pbPoing.set_longitude(point.longitude);
    return pbPoing;
}

Sphere::Point Serialization::TransportCatalogProtoMapper::Map(const Serialization::Point& pbPoint)
{
    return Sphere::Point{ .latitude = pbPoint.latitude(),
                            .longitude = pbPoint.longitude() };
}

Serialization::RoutingSettings Serialization::TransportCatalogProtoMapper::Map(const Router::RoutingSettings& settings)
{
    Serialization::RoutingSettings pbSettings;
    pbSettings.set_bus_wait_time(settings.busWaitTime);
    pbSettings.set_bus_velocity(settings.busVelocity);
    pbSettings.set_pedestrian_velocity(settings.pedestrianVelocity);
    return pbSettings;
}

Router::RoutingSettings Serialization::TransportCatalogProtoMapper::Map(const Serialization::RoutingSettings& pbSettings)
{
    return Router::RoutingSettings{
        .busWaitTime = pbSettings.bus_wait_time(),
        .busVelocity = pbSettings.bus_velocity(),
        .pedestrianVelocity = pbSettings.pedestrian_velocity()
    };
}

Serialization::RenderSettings Serialization::TransportCatalogProtoMapper::Map(const Visualization::RenderSettings& settings)
{
    Serialization::RenderSettings pbSettings;
    pbSettings.set_max_map_width(settings.maxMapWidth);
    pbSettings.set_max_map_height(settings.maxMapHeight);
    pbSettings.set_padding(settings.padding);
    pbSettings.set_stop_radius(settings.stopRadius);
    pbSettings.set_bus_line_width(settings.busLineWidth);
    pbSettings.set_stop_label_font_size(settings.stopLabelFontSize);
    *pbSettings.mutable_stop_label_offset() = Map(settings.stopLabelOffset);
    *pbSettings.mutable_substrate_underlayer_color() = Map(settings.substrateUnderlayerColor);
    pbSettings.set_underlayer_width(settings.underlayerWidth);
    for (const auto& color : settings.colorPalette)
    {
        *pbSettings.add_color_palette() = Map(color);
    }
    pbSettings.set_bus_label_font_size(settings.busLabelFontSize);
    *pbSettings.mutable_bus_label_offset() = Map(settings.busLabelOffset);
    for (const auto& l : settings.layers)
    {
        pbSettings.add_layers(l);
    }
    pbSettings.set_outer_margin(settings.outerMargin);
    pbSettings.set_company_radius(settings.companyRadius);
    pbSettings.set_company_line_width(settings.companyLineWidth);
    return pbSettings;
}

Visualization::RenderSettings Serialization::TransportCatalogProtoMapper::Map(const Serialization::RenderSettings& pbSettings)
{
    Visualization::RenderSettings settings;
    settings.maxMapWidth = pbSettings.max_map_width();
    settings.maxMapHeight = pbSettings.max_map_height();
    settings.padding = pbSettings.padding();
    settings.stopRadius = pbSettings.stop_radius();
    settings.busLineWidth = pbSettings.bus_line_width();
    settings.stopLabelFontSize = pbSettings.stop_label_font_size();
    settings.stopLabelOffset = Map(pbSettings.stop_label_offset());
    settings.substrateUnderlayerColor = Map(pbSettings.substrate_underlayer_color());
    settings.underlayerWidth = pbSettings.underlayer_width();
    settings.colorPalette.reserve(pbSettings.color_palette_size());
    for (const auto& color : pbSettings.color_palette())
    {
        settings.colorPalette.emplace_back(Map(color));
    }
    settings.busLabelFontSize = pbSettings.bus_label_font_size();
    settings.busLabelOffset = Map(pbSettings.bus_label_offset());
    settings.layers.reserve(pbSettings.layers_size());
    for (const auto& l : pbSettings.layers())
    {
        settings.layers.emplace_back(l);
    }
    settings.outerMargin = pbSettings.outer_margin();
    settings.companyRadius = pbSettings.company_radius();
    settings.companyLineWidth = pbSettings.company_line_width();
    return settings;
}

Serialization::SvgPoint Serialization::TransportCatalogProtoMapper::Map(const Svg::Point& point)
{
    Serialization::SvgPoint pbPoint;
    pbPoint.set_x(point.x);
    pbPoint.set_y(point.y);
    return pbPoint;
}

Svg::Point Serialization::TransportCatalogProtoMapper::Map(const Serialization::SvgPoint& pbPoint)
{
    return Svg::Point{
        .x = pbPoint.x(),
        .y = pbPoint.y()
    };
}

Serialization::SvgColor Serialization::TransportCatalogProtoMapper::Map(const Svg::Color& color)
{
    struct ColorMapper
    {
        Serialization::SvgColor pbColor;
        void Map(std::monostate)
        {
            // No actions
        }

        void Map(const std::string& c)
        {
            pbColor.set_color_name(c);
        }
        void Map(const Svg::Rgb& c)
        {
            *pbColor.mutable_rgb() = TransportCatalogProtoMapper::Map(c);
        }
        void Map(const Svg::Rgba& c)
        {
            *pbColor.mutable_rgba() = TransportCatalogProtoMapper::Map(c);
        }

    };
    ColorMapper mapper;
    std::visit([&mapper](auto&& c)
        {
            mapper.Map(c);
        }, color.GetColor());
    return mapper.pbColor;
}

Svg::Color Serialization::TransportCatalogProtoMapper::Map(const Serialization::SvgColor& pbColor)
{
    switch (pbColor.color_case())
    {
    case SvgColor::ColorCase::kColorName:
        return Svg::Color(pbColor.color_name());
    case SvgColor::ColorCase::kRgb:
        return Svg::Color(Map(pbColor.rgb()));
    case SvgColor::ColorCase::kRgba:
        return Svg::Color(Map(pbColor.rgba()));
    }
    return Svg::NoneColor;
}

Serialization::Rgb Serialization::TransportCatalogProtoMapper::Map(const Svg::Rgb& rgb)
{
    Serialization::Rgb pbRgb;
    pbRgb.set_red(rgb.red);
    pbRgb.set_green(rgb.green);
    pbRgb.set_blue(rgb.blue);
    return pbRgb;
}

Svg::Rgb Serialization::TransportCatalogProtoMapper::Map(const Serialization::Rgb& pbRgb)
{
    return Svg::Rgb{
        .red = static_cast<uint8_t>(pbRgb.red()),
        .green = static_cast<uint8_t>(pbRgb.green()),
        .blue = static_cast<uint8_t>(pbRgb.blue())
    };
}

Serialization::Rgba Serialization::TransportCatalogProtoMapper::Map(const Svg::Rgba& rgba)
{
    Serialization::Rgba pbRgba;
    pbRgba.set_red(rgba.red);
    pbRgba.set_green(rgba.green);
    pbRgba.set_blue(rgba.blue);
    pbRgba.set_alpha(rgba.alpha);
    return pbRgba;
}

Svg::Rgba Serialization::TransportCatalogProtoMapper::Map(const Serialization::Rgba& pbRgba)
{
    return Svg::Rgba{
        .red = static_cast<uint8_t>(pbRgba.red()),
        .green = static_cast<uint8_t>(pbRgba.green()),
        .blue = static_cast<uint8_t>(pbRgba.blue()),
        .alpha = pbRgba.alpha()
    };
}