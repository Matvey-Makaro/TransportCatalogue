#pragma once

#include "Descriptions.h"
#include "Graph.h"
#include "Json.h"
#include "Router.h"

#include <memory>
#include <unordered_map>
#include <vector>

namespace Router
{
  struct RoutingSettings {
    int bus_wait_time;  // in minutes
    double bus_velocity;  // km/h
  
    static RoutingSettings FromJson(const Json::Dict& json);
  };

  class TransportRouter {
  private:
    using BusGraph = Graph::DirectedWeightedGraph<double>;
    using Router = Graph::Router<double>;
  
  public:
    TransportRouter(const Descriptions::StopsDict& stops_dict,
                    const Descriptions::BusesDict& buses_dict,
                    const RoutingSettings& routingSettings);
  
    struct RouteInfo {
      double total_time;
  
      struct BusItem {
        std::string bus_name;
        double time;
        size_t span_count;
      };
      struct WaitItem {
        std::string stop_name;
        double time;
      };
  
      using Item = std::variant<BusItem, WaitItem>;
      std::vector<Item> items;
    };
  
    std::optional<RouteInfo> FindRoute(const std::string& stopFrom, const std::string& stopTo) const;
  
  private:
    void FillGraphWithStops(const Descriptions::StopsDict& stops_dict);
  
    void FillGraphWithBuses(const Descriptions::StopsDict& stops_dict,
                            const Descriptions::BusesDict& buses_dict);
  
    struct StopVertexIds {
      Graph::VertexId in;
      Graph::VertexId out;
    };
    struct VertexInfo {
      std::string stop_name;
    };
  
    struct BusEdgeInfo {
      std::string bus_name;
      size_t span_count;
    };
    struct WaitEdgeInfo {};
    using EdgeInfo = std::variant<BusEdgeInfo, WaitEdgeInfo>;
  
    RoutingSettings routing_settings_;
    BusGraph graph_;
    std::unique_ptr<Router> router_;
    std::unordered_map<std::string, StopVertexIds> stops_vertex_ids_;
    std::vector<VertexInfo> vertices_info_;
    std::vector<EdgeInfo> edges_info_;
  };
}
