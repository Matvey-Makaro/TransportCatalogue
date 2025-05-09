#pragma once

#include "Json.h"
#include "Sphere.h"

#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace Descriptions {
  struct Stop {
    std::string name;
    Sphere::Point position;
    std::unordered_map<std::string, int> distances;

    static Stop ParseFrom(const Json::Dict& attrs);
  };

  int ComputeStopsDistance(const Stop& lhs, const Stop& rhs);

  std::vector<std::string> ParseStops(const std::vector<Json::Node>& stop_nodes, bool is_roundtrip);

  struct Bus {
    std::string name;
    std::vector<std::string> stops;
    bool isRoundtrip;

    using Terminals = std::pair<std::string, std::string>;
    Terminals GetTerminals() const;
    static Bus ParseFrom(const Json::Dict& attrs);
  };

  struct InputQueries
  {
    std::vector<Descriptions::Bus> buses;
    std::vector<Descriptions::Stop> stops;
  };

  InputQueries ReadDescriptions(const std::vector<Json::Node>& nodes);

  template <typename Object>
  using Dict = std::unordered_map<std::string, const Object*>;

  using StopsDict = Dict<Stop>;
  using BusesDict = Dict<Bus>;
}
