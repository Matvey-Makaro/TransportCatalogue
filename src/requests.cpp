#include "Requests.h"
#include "TransportRouter.h"
#include "Svg/MapVisualizer.h"

#include <vector>
#include <sstream>

using namespace std;


namespace Requests
{
  Stop::Stop(std::string stopName) :
    _name(std::move(stopName))
  {
  }


  Json::Dict Stop::Process(const TransportDatabase& db) const
  {
    const auto* stop = db.GetStop(_name);
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
      for (const auto& bus_name : stop->bus_names)
      {
        bus_nodes.emplace_back(bus_name);
      }
      dict["buses"] = Json::Node(std::move(bus_nodes));
    }
#endif
    return dict;
  }

  Bus::Bus(std::string busName) :
    _name(std::move(busName))
  {
  }

  Json::Dict Bus::Process(const TransportDatabase& db) const
  {
    const auto* bus = db.GetBus(_name);
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
    Json::Dict operator()(const TransportRouter::RouteInfo::BusItem& bus_item) const
    {
      return Json::Dict{
          {"type", Json::Node("Bus"s)},
          {"bus", Json::Node(bus_item.bus_name)},
          {"time", Json::Node(bus_item.time)},
          {"span_count", Json::Node(static_cast<int>(bus_item.span_count))} };
    }
    Json::Dict operator()(const TransportRouter::RouteInfo::WaitItem& wait_item) const
    {
      return Json::Dict{
          {"type", Json::Node("Wait"s)},
          {"stop_name", Json::Node(wait_item.stop_name)},
          {"time", Json::Node(wait_item.time)},
      };
    }
  };

  Route::Route(std::string stopFrom, std::string stopTo, const Svg::MapVisualizer* mapVisualizer) :
    _stopFrom(std::move(stopFrom)),
    _stopTo(std::move(stopTo)),
    _mapVisualizer(mapVisualizer)
  {
  }

  Json::Dict Route::Process(const TransportDatabase& db) const
  {
    Json::Dict dict;

    const auto route = db.FindRoute(_stopFrom, _stopTo);
    if (!route)
    {
#ifndef OnlyMap
      dict["error_message"] = Json::Node("not found"s);
#endif
    }
    else
    {
#ifndef OnlyMap
      dict["total_time"] = Json::Node(route->total_time);
      vector<Json::Node> items;
      items.reserve(route->items.size());
      for (const auto& item : route->items)
      {
        items.push_back(visit(RouteItemResponseBuilder{}, item));
      }

      dict["items"] = std::move(items);
#endif

      std::stringstream mapSvg;
      _mapVisualizer->RenderRoute(mapSvg, *route, _stopTo);
      dict["map"] = mapSvg.str();
    }

    return dict;
  }

  Map::Map(const Svg::MapVisualizer* mapVisualizer) :
    _mapVisualizer(mapVisualizer)
  {
  }

  Json::Dict Map::Process(const TransportDatabase& db) const
  {
    Json::Dict dict;
#ifndef OnlyMap
    std::stringstream ss;
    _mapVisualizer->Render(ss);
    dict["map"] = ss.str();
#endif
    return dict;
  }

  variant<Stop, Bus, Route, Map> Read(const Svg::MapVisualizer& mapVisualizer, const Json::Dict& attrs)
  {
    const string& type = attrs.at("type").AsString();
    if (type == "Bus")
    {
      return Bus{ attrs.at("name").AsString() };
    }
    else if (type == "Stop")
    {
      return Stop{ attrs.at("name").AsString() };
    }
    else if (type == "Route")
    {
      return Route{ attrs.at("from").AsString(), attrs.at("to").AsString(), &mapVisualizer };
    }
    else if (type == "Map")
    {
      return Map(&mapVisualizer);
    }
    else
    {
      assert(false);
      return Map(&mapVisualizer);
    }
  }

  vector<Json::Node> ProcessAll(const TransportDatabase& db,
    const Svg::MapVisualizer& mapVisualizer,
    const vector<Json::Node>& requests)
  {
    vector<Json::Node> responses;
    responses.reserve(requests.size());
    for (const Json::Node& request_node : requests)
    {
      Json::Dict dict = visit([&db](auto&& request)
        {
          return request.Process(db);
        },
        Requests::Read(mapVisualizer, request_node.AsMap()));
      dict["request_id"] = Json::Node(request_node.AsMap().at("id").AsInt());
      responses.push_back(Json::Node(dict));
    }
    return responses;
  }

}
