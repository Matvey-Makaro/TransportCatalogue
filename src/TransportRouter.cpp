#include "TransportRouter.h"
#include "Utils.h"
#include "ProgramState.h"

using namespace std;
using namespace Router;

RoutingSettings RoutingSettings::FromJson(const Json::Dict& json)
{
  REGISTER_CURR_FUNC();
  return RoutingSettings{
    json.at("bus_wait_time").AsInt(),
    json.at("bus_velocity").AsDouble(),
    json.at("pedestrian_velocity").AsDouble(),
  };
}

TransportRouter::TransportRouter(const Descriptions::StopsDict& stops_dict,
  const Descriptions::BusesDict& buses_dict,
  const RoutingSettings& routingSettings)
  : routing_settings_(routingSettings)
{
  REGISTER_CURR_FUNC();
  const size_t vertex_count = stops_dict.size() * 2;
  vertices_info_.resize(vertex_count);
  graph_ = BusGraph(vertex_count);

  FillGraphWithStops(stops_dict);
  FillGraphWithBuses(stops_dict, buses_dict);

  router_ = std::make_unique<Router>(graph_);
}

void TransportRouter::FillGraphWithStops(const Descriptions::StopsDict& stops_dict) {
  REGISTER_CURR_FUNC();
  Graph::VertexId vertex_id = 0;

  for (const auto& [stop_name, _] : stops_dict) {
    auto& vertex_ids = stops_vertex_ids_[stop_name];
    vertex_ids.in = vertex_id++;
    vertex_ids.out = vertex_id++;
    vertices_info_[vertex_ids.in] = { stop_name };
    vertices_info_[vertex_ids.out] = { stop_name };

    edges_info_.push_back(WaitEdgeInfo{});
    const Graph::EdgeId edge_id = graph_.AddEdge({
        vertex_ids.out,
        vertex_ids.in,
        static_cast<double>(routing_settings_.busWaitTime)
      });
    assert(edge_id == edges_info_.size() - 1);
  }

  assert(vertex_id == graph_.GetVertexCount());
}

void TransportRouter::FillGraphWithBuses(const Descriptions::StopsDict& stops_dict,
  const Descriptions::BusesDict& buses_dict) {
  REGISTER_CURR_FUNC();
  for (const auto& [_, bus_item] : buses_dict) {
    const auto& bus = *bus_item;
    const size_t stop_count = bus.stops.size();
    if (stop_count <= 1) {
      continue;
    }
    auto compute_distance_from = [&stops_dict, &bus](size_t lhs_idx) {
      return Descriptions::ComputeStopsDistance(*stops_dict.at(bus.stops[lhs_idx]), *stops_dict.at(bus.stops[lhs_idx + 1]));
      };
    for (size_t start_stop_idx = 0; start_stop_idx + 1 < stop_count; ++start_stop_idx) {
      const Graph::VertexId start_vertex = stops_vertex_ids_[bus.stops[start_stop_idx]].in;
      int total_distance = 0;
      for (size_t finish_stop_idx = start_stop_idx + 1; finish_stop_idx < stop_count; ++finish_stop_idx) {
        total_distance += compute_distance_from(finish_stop_idx - 1);
        edges_info_.push_back(BusEdgeInfo{
            .bus_name = bus.name,
            .span_count = finish_stop_idx - start_stop_idx,
          });
        const Graph::EdgeId edge_id = graph_.AddEdge({
            start_vertex,
            stops_vertex_ids_[bus.stops[finish_stop_idx]].out,
            total_distance * 1.0 / ConvertToMetersPerMin(routing_settings_.busVelocity)  // m / (m/min) = min
          });
        assert(edge_id == edges_info_.size() - 1);
      }
    }
  }
}

optional<TransportRouter::RouteInfo> TransportRouter::FindRoute(const string& stopFrom, const string& stopTo) const {
  REGISTER_CURR_FUNC();
  const Graph::VertexId vertex_from = stops_vertex_ids_.at(stopFrom).out;
  const Graph::VertexId vertex_to = stops_vertex_ids_.at(stopTo).out;
  const auto route = router_->BuildRoute(vertex_from, vertex_to);
  if (!route) {
    return nullopt;
  }

  RouteInfo route_info = { .total_time = route->weight };
  route_info.items.reserve(route->edge_count);
  for (size_t edge_idx = 0; edge_idx < route->edge_count; ++edge_idx) {
    const Graph::EdgeId edge_id = router_->GetRouteEdge(route->id, edge_idx);
    const auto& edge = graph_.GetEdge(edge_id);
    const auto& edge_info = edges_info_[edge_id];
    if (holds_alternative<BusEdgeInfo>(edge_info)) {
      const BusEdgeInfo& bus_edge_info = get<BusEdgeInfo>(edge_info);
      route_info.items.push_back(RouteInfo::RideBusItem{
          .bus_name = bus_edge_info.bus_name,
          .time = edge.weight,
          .span_count = bus_edge_info.span_count,
        });
    }
    else {
      const Graph::VertexId vertex_id = edge.from;
      route_info.items.push_back(RouteInfo::WaitBusItem{
          .stop_name = vertices_info_[vertex_id].stop_name,
          .time = edge.weight,
        });
    }
  }

  // Releasing in destructor of some proxy object would be better,
  // but we do not expect exceptions in normal workflow
  router_->ReleaseRoute(route->id);
  return route_info;
}

bool Router::operator==(const TransportRouter::RouteInfo::RideBusItem& lhs, const TransportRouter::RouteInfo::RideBusItem& rhs)
{
  REGISTER_CURR_FUNC();
  return lhs.bus_name == rhs.bus_name &&
    IsEqualRel(lhs.time, rhs.time) &&
    lhs.span_count == rhs.span_count;
}

bool Router::operator==(const TransportRouter::RouteInfo::WaitBusItem& lhs, const TransportRouter::RouteInfo::WaitBusItem& rhs)
{
  REGISTER_CURR_FUNC();
  return lhs.stop_name == rhs.stop_name &&
    IsEqualRel(lhs.time, rhs.time);
}

bool Router::operator==(const TransportRouter::RouteInfo::WalkToCompany& lhs, const TransportRouter::RouteInfo::WalkToCompany& rhs)
{
  REGISTER_CURR_FUNC();
  return IsEqualRel(lhs.time, rhs.time) &&
    lhs.stop_name == rhs.stop_name &&
    lhs.company_name == rhs.company_name;
}

bool Router::operator==(const TransportRouter::RouteInfo::Item& lhs, const TransportRouter::RouteInfo::Item& rhs)
{
  REGISTER_CURR_FUNC();
  if (lhs.index() != rhs.index())
    return false;

  return std::visit([](auto&& lhs_arg, auto&& rhs_arg)
    {
      return lhs_arg == rhs_arg;
    }, lhs, rhs);
}

bool Router::operator==(const TransportRouter::RouteInfo& lhs, const TransportRouter::RouteInfo& rhs)
{
  REGISTER_CURR_FUNC();
  return IsEqualRel(lhs.total_time, rhs.total_time) &&
    std::equal(begin(lhs.items), end(lhs.items), begin(rhs.items), end(rhs.items));
}
