#pragma once

#include "Descriptions.h"
#include "Json.h"
#include "TransportRouter.h"
#include "RenderSettings.h"
#include "Utils.h"
#include "Svg/MapVisualizer.h"

#include <optional>
#include <set>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace Responses {
  struct Stop {
    std::set<std::string> bus_names;
  };

  struct Bus {
    size_t stop_count = 0;
    size_t unique_stop_count = 0;
    int road_route_length = 0;
    double geo_route_length = 0.0;
  };
}

class TransportDatabase {
private:
  using Bus = Responses::Bus;
  using Stop = Responses::Stop;

public:
  TransportDatabase(Descriptions::InputQueries data,
                     const Router::RoutingSettings& routingSettings);

  const Stop* GetStop(const std::string& name) const;
  const Bus* GetBus(const std::string& name) const;

  std::vector<const Descriptions::Bus*> GetBusesDescriptions() const;
  std::vector<const Descriptions::Stop*> GetStopsDescriptions() const;

  std::optional<Router::TransportRouter::RouteInfo> FindRoute(const std::string& stopFrom, const std::string& stopTo) const;
  Router::RoutingSettings GetRoutingSettings() const;

  private:
  static int ComputeRoadRouteLength(
      const std::vector<std::string>& stops,
      const Descriptions::StopsDict& stops_dict
  );

  static double ComputeGeoRouteDistance(
      const std::vector<std::string>& stops,
      const Descriptions::StopsDict& stops_dict
  );

  std::vector<Descriptions::Bus> _busesDescr;
  std::vector<Descriptions::Stop> _stopsDescr;
  std::unordered_map<std::string, Stop> _stops;
  std::unordered_map<std::string, Bus> _buses;
  Router::RoutingSettings _routingSettings;
  std::unique_ptr<Router::TransportRouter> _router;
};
