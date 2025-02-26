#include "requests.h"
#include "transport_router.h"
#include "Svg/MapVisualizer.h"

#include <vector>
#include <sstream>

using namespace std;

// TODO: Turn of
// #define OnlyMap

namespace Requests
{

  Json::Dict Stop::Process(const TransportCatalog &db) const
  {
    const auto *stop = db.GetStop(name);
    Json::Dict dict;
#ifndef OnlyMap
    if (!stop)
    {
      dict["error_message"] = Json::Node("not found"s);
    }
    else
    {
      vector<Json::Node> bus_nodes;
      bus_nodes.reserve(stop->bus_names.size());
      for (const auto &bus_name : stop->bus_names)
      {
        bus_nodes.emplace_back(bus_name);
      }
      dict["buses"] = Json::Node(std::move(bus_nodes));
    }
#endif
    return dict;
  }

  Json::Dict Bus::Process(const TransportCatalog &db) const
  {
    const auto *bus = db.GetBus(name);
    Json::Dict dict;
#ifndef OnlyMap
    if (!bus)
    {
      dict["error_message"] = Json::Node("not found"s);
    }
    else
    {
      dict = {
          {"stop_count", Json::Node(static_cast<int>(bus->stop_count))},
          {"unique_stop_count", Json::Node(static_cast<int>(bus->unique_stop_count))},
          {"route_length", Json::Node(bus->road_route_length)},
          {"curvature", Json::Node(bus->road_route_length / bus->geo_route_length)},
      };
    }
#endif
    return dict;
  }

  struct RouteItemResponseBuilder
  {
    Json::Dict operator()(const TransportRouter::RouteInfo::BusItem &bus_item) const
    {
      return Json::Dict{
          {"type", Json::Node("Bus"s)},
          {"bus", Json::Node(bus_item.bus_name)},
          {"time", Json::Node(bus_item.time)},
          {"span_count", Json::Node(static_cast<int>(bus_item.span_count))}};
    }
    Json::Dict operator()(const TransportRouter::RouteInfo::WaitItem &wait_item) const
    {
      return Json::Dict{
          {"type", Json::Node("Wait"s)},
          {"stop_name", Json::Node(wait_item.stop_name)},
          {"time", Json::Node(wait_item.time)},
      };
    }
  };

  Json::Dict Route::Process(const TransportCatalog &db) const
  {
    Json::Dict dict;
#ifndef OnlyMap
    const auto route = db.FindRoute(stop_from, stop_to);
    if (!route)
    {
      dict["error_message"] = Json::Node("not found"s);
    }
    else
    {
      dict["total_time"] = Json::Node(route->total_time);
      vector<Json::Node> items;
      items.reserve(route->items.size());
      for (const auto &item : route->items)
      {
        items.push_back(visit(RouteItemResponseBuilder{}, item));
      }

      dict["items"] = std::move(items);
    }
#endif
    return dict;
  }

  Json::Dict Map::Process(const TransportCatalog &db) const
  {
    Svg::MapVisualizer mapVisualizer(db.GetStopsDescriptions(),
                                     db.GetBusesDescriptions(),
                                     db.GetRenderSettings());
    std::stringstream ss;
    mapVisualizer.Render(ss);
    Json::Dict dict;
    dict["map"] = ss.str();
    return dict;
  }

  variant<Stop, Bus, Route, Map> Read(const Json::Dict &attrs)
  {
    const string &type = attrs.at("type").AsString();
    if (type == "Bus")
    {
      return Bus{attrs.at("name").AsString()};
    }
    else if (type == "Stop")
    {
      return Stop{attrs.at("name").AsString()};
    }
    else if (type == "Route")
    {
      return Route{attrs.at("from").AsString(), attrs.at("to").AsString()};
    }
    else if (type == "Map")
    {
      return Map();
    }
    else
    {
      assert(false);
      return Map();
    }
  }

  vector<Json::Node> ProcessAll(const TransportCatalog &db, const vector<Json::Node> &requests)
  {
    vector<Json::Node> responses;
    responses.reserve(requests.size());
    for (const Json::Node &request_node : requests)
    {
      Json::Dict dict = visit([&db](const auto &request)
                              { return request.Process(db); },
                              Requests::Read(request_node.AsMap()));
      dict["request_id"] = Json::Node(request_node.AsMap().at("id").AsInt());
      responses.push_back(Json::Node(dict));
    }
    return responses;
  }

}
