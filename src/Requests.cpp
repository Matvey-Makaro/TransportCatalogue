#include <vector>
#include <sstream>

#include "Requests.h"
#include "TransportRouter.h"
#include "TransportDatabase.h"
#include "Svg/MapVisualizer.h"
#include "YellowPages/Company.h"
#include "Utils.h"

using namespace std;
using namespace Router;

namespace Requests
{
  Stop::Stop(std::string stopName, TransportDatabaseShp transportDb) : _name(std::move(stopName)),
    _transportDb(std::move(transportDb))
  {
  }

  Json::Dict Stop::Process() const
  {
    if (_transportDb == nullptr)
    {
      return {};
    }
    const auto* stop = _transportDb->GetStop(_name);
    Json::Dict dict;
#ifndef OnlyMap
    if (!stop)
    {
      dict["error_message"] = Json::Node("not found"s);
    }
    else
    {
      vector<Json::Node> bus_nodes;
      bus_nodes.reserve(stop->busNames.size());
      for (const auto& bus_name : stop->busNames)
      {
        bus_nodes.emplace_back(bus_name);
      }
      dict["buses"] = Json::Node(std::move(bus_nodes));
    }
#endif
    return dict;
  }

  Bus::Bus(std::string busName, TransportDatabaseShp transportDb) : _name(std::move(busName)),
    _transportDb(std::move(transportDb))
  {
  }

  Json::Dict Bus::Process() const
  {
    if (_transportDb == nullptr)
    {
      return {};
    }
    const auto* bus = _transportDb->GetBus(_name);
    Json::Dict dict;
#ifndef OnlyMap
    if (!bus)
    {
      dict["error_message"] = Json::Node("not found"s);
    }
    else
    {
      dict = {
          {"stop_count", Json::Node(static_cast<int>(bus->stopCount))},
          {"unique_stop_count", Json::Node(static_cast<int>(bus->uniqueStopCount))},
          {"route_length", Json::Node(bus->roadRouteLength)},
          {"curvature", Json::Node(bus->roadRouteLength / bus->geoRouteLength)},
      };
    }
#endif
    return dict;
  }

  struct RouteItemResponseBuilder
  {
    Json::Dict operator()(const TransportRouter::RouteInfo::RideBusItem& bus_item) const
    {
      return Json::Dict{
          {"type", Json::Node("RideBus"s)},
          {"bus", Json::Node(bus_item.bus_name)},
          {"time", Json::Node(bus_item.time)},
          {"span_count", Json::Node(static_cast<int>(bus_item.span_count))} };
    }
    Json::Dict operator()(const TransportRouter::RouteInfo::WaitBusItem& wait_item) const
    {
      return Json::Dict{
          {"type", Json::Node("WaitBus"s)},
          {"stop_name", Json::Node(wait_item.stop_name)},
          {"time", Json::Node(wait_item.time)},
      };
    }
    Json::Dict operator()(const TransportRouter::RouteInfo::WalkToCompany& walk_to_company) const
    {
      return Json::Dict{
          {"type", Json::Node("WalkToCompany"s)},
          {"time", Json::Node(walk_to_company.time)},
          {"stop_name", Json::Node(walk_to_company.stop_name)},
          {"company", Json::Node(walk_to_company.company_name)},
      };
    }
  };

  Route::Route(std::string stopFrom, std::string stopTo, ContextShp context) : _stopFrom(std::move(stopFrom)),
    _stopTo(std::move(stopTo)),
    _context(std::move(context))
  {
  }

  Json::Dict Route::Process() const
  {
    if (_context == nullptr || _context->transportDb == nullptr)
    {
      return {};
    }

    Json::Dict dict;
    const auto route = _context->transportDb->FindRoute(_stopFrom, _stopTo);
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
      if (_context->mapVisualizer != nullptr)
      {
        _context->mapVisualizer->RenderRoute(mapSvg, *route, _stopTo);
        dict["map"] = mapSvg.str();
      }
    }

    return dict;
  }

  Map::Map(Svg::MapVisualizerShp mapVisualizer) : _mapVisualizer(std::move(mapVisualizer))
  {
  }

  Json::Dict Map::Process() const
  {
    Json::Dict dict;
#ifndef OnlyMap
    std::stringstream ss;
    _mapVisualizer->Render(ss);
    dict["map"] = ss.str();
#endif
    return dict;
  }

  Request Read(const ContextShp& context, const Json::Dict& attrs)
  {
    assert(context != nullptr);

    const string& type = attrs.at("type").AsString();
    if (type == "Bus")
    {
      return Bus(attrs.at("name").AsString(), context->transportDb);
    }
    else if (type == "Stop")
    {
      return Stop(attrs.at("name").AsString(), context->transportDb);
    }
    else if (type == "Route")
    {
      return Route(attrs.at("from").AsString(), attrs.at("to").AsString(), context);
    }
    else if (type == "Map")
    {
      return Map(context->mapVisualizer);
    }
    else if (type == "FindCompanies")
    {
      return FindCompanies(context->yellowPagesDb,
        YellowPages::BLL::CompanyRestrictions::FromJson(attrs));
    }
    else if (type == "RouteToCompany")
    {
      return RouteToCompany(context,
        attrs.at("from").AsString(),
        YellowPages::BLL::CompanyRestrictions::FromJson(attrs.at("companies").AsMap()));
    }
    else
    {
      assert(false);
      return Map(context->mapVisualizer);
    }
  }

  vector<Json::Node> ProcessAll(const ContextShp& context,
    const vector<Json::Node>& requests)
  {
    vector<Json::Node> responses;
    responses.reserve(requests.size());
    for (const Json::Node& requestNode : requests)
    {
      Json::Dict dict = visit([&context](auto&& request)
        { return request.Process(); },
        Requests::Read(context, requestNode.AsMap()));
      dict["request_id"] = Json::Node(requestNode.AsMap().at("id").AsInt());
      responses.push_back(Json::Node(dict));
    }
    return responses;
  }

  FindCompanies::FindCompanies(YellowPages::BLL::YellowPagesDatabaseShp yellowPagesDb,
    YellowPages::BLL::CompanyRestrictions companyRestrictions)
    : _yellowPagesDb(std::move(yellowPagesDb)),
    _companyRestrictions(std::move(companyRestrictions))
  {
  }

  Json::Dict FindCompanies::Process() const
  {
    using namespace YellowPages::BLL;
    if (_yellowPagesDb == nullptr)
    {
      return {};
    }
    const auto companies = _yellowPagesDb->FindCompanies(_companyRestrictions);
    Json::Arr companiesNames;
    companiesNames.reserve(companies.size());
    for (const auto* company : companies)
    {
      if (company == nullptr)
        continue;
      companiesNames.emplace_back(company->GetMainName().value);
    }
    Json::Dict dict;
    dict["companies"] = std::move(companiesNames);
    return dict;
  }

  RouteToCompany::RouteToCompany(ContextShp context,
    std::string stopFrom,
    YellowPages::BLL::CompanyRestrictions companyRestrictions)
    : _context(std::move(context)),
    _stopFrom(std::move(stopFrom)),
    _companyRestrictions(std::move(companyRestrictions))
  {
  }

  Json::Dict RouteToCompany::Process() const
  {
    // TODO: Вынести общий код с Route?
    if (_context == nullptr || _context->transportDb == nullptr || _context->yellowPagesDb == nullptr)
    {
      return {};
    }
    const auto companies = _context->yellowPagesDb->FindCompanies(_companyRestrictions);
    const auto route = _context->transportDb->FindRouteToCompany(_stopFrom, companies);
    Json::Dict dict;
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
      if (_context->mapVisualizer != nullptr)
      {
        // TODO: Implement RenderRouteToCompany
        // _context->mapVisualizer->RenderRoute(mapSvg, *route, _stopTo);
        dict["map"] = mapSvg.str();
      }
    }
    return dict;
  }
}
