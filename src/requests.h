#pragma once

#include "Json.h"
#include "TransportCatalog.h"

#include <string>
#include <variant>


namespace Svg
{
  class MapVisualizer;
}

namespace Requests {
  class Stop {
  public:
    Stop(std::string stopName);
    Json::Dict Process(const TransportCatalog& db) const;

  private:
    std::string _name;
  };

  class Bus {
  public:
    Bus(std::string busName);

    Json::Dict Process(const TransportCatalog& db) const;
  private:
    std::string _name;
  };

  class Route {
  public:
    Route(std::string stopFrom,
      std::string stopTo,
      const Svg::MapVisualizer* mapVisualizer);

    Json::Dict Process(const TransportCatalog& db) const;
  private:
    std::string _stopFrom;
    std::string _stopTo;
    const Svg::MapVisualizer* _mapVisualizer;
  };

  class Map
  {
  public:
    Map(const Svg::MapVisualizer* mapVisualizer);
    Json::Dict Process(const TransportCatalog& db) const;

  private:
    const Svg::MapVisualizer* _mapVisualizer;
  };

  std::variant<Stop, Bus, Route, Map> Read(const Svg::MapVisualizer& mapVisualizer, const Json::Dict& attrs);

  std::vector<Json::Node> ProcessAll(const TransportCatalog& db,
    const Svg::MapVisualizer& mapVisualizer,
    const std::vector<Json::Node>& requests);
}
