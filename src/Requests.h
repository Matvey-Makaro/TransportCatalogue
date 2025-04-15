#pragma once

#include <string>
#include <variant>
#include <memory>

#include "Json.h"
#include "YellowPages/YellowPagesDatabase.h"

class TransportDatabase;
using TransportDatabaseShp = std::shared_ptr<TransportDatabase>;

namespace Svg
{
  class MapVisualizer;
  using MapVisualizerShp = std::shared_ptr<MapVisualizer>;
}

namespace Requests
{
  struct Context
  {
    TransportDatabaseShp transportDb;
    YellowPages::BLL::YellowPagesDatabaseShp yellowPagesDb;
    Svg::MapVisualizerShp mapVisualizer;
  };
  using ContextShp = std::shared_ptr<Context>;

  class Stop
  {
  public:
    Stop(std::string stopName, TransportDatabaseShp transportDb);
    Json::Dict Process() const;

  private:
    std::string _name;
    TransportDatabaseShp _transportDb;
  };

  class Bus
  {
  public:
    Bus(std::string busName, TransportDatabaseShp transportDb);
    Json::Dict Process() const;

  private:
    std::string _name;
    TransportDatabaseShp _transportDb;
  };

  class Route
  {
  public:
    Route(std::string stopFrom,
          std::string stopTo,
          ContextShp context);

    Json::Dict Process() const;

  private:
    std::string _stopFrom;
    std::string _stopTo;
    ContextShp _context;
  };

  class Map
  {
  public:
    Map(Svg::MapVisualizerShp mapVisualizer);
    Json::Dict Process() const;

  private:
    Svg::MapVisualizerShp _mapVisualizer;
  };

  class FindCompanies
  {
  public:
    FindCompanies(YellowPages::BLL::YellowPagesDatabaseShp yellowPagesDb,
                  YellowPages::BLL::CompanyRestrictions companyRestrictions);
    Json::Dict Process() const;

  private:
    YellowPages::BLL::YellowPagesDatabaseShp _yellowPagesDb;
    YellowPages::BLL::CompanyRestrictions _companyRestrictions;
  };

  class RouteToCompany
  {
  public:
    RouteToCompany(ContextShp context, std::string stopFrom, YellowPages::BLL::CompanyRestrictions companyRestrictions);
    Json::Dict Process() const;

  private:
    ContextShp _context;
    std::string _stopFrom;
    YellowPages::BLL::CompanyRestrictions _companyRestrictions;
  };

  using Request = std::variant<Stop, Bus, Route, Map, FindCompanies, RouteToCompany>;
  Request Read(const ContextShp &context, const Json::Dict &attrs);

  std::vector<Json::Node> ProcessAll(const ContextShp &context,
                                     const std::vector<Json::Node> &requests);
}
