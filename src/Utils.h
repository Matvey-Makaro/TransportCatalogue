#pragma once

#include <iterator>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <limits>

namespace Json
{
  class Node;
  using Dict = std::map<std::string, Node>;
}

template <typename It>
class Range {
public:
  using ValueType = typename std::iterator_traits<It>::value_type;

  Range(It begin, It end) : begin_(begin), end_(end) {}
  It begin() const { return begin_; }
  It end() const { return end_; }

private:
  It begin_;
  It end_;
};

template <typename C>
auto AsRange(const C& container) {
  return Range{std::begin(container), std::end(container)};
}

template <typename It>
size_t ComputeUniqueItemsCount(Range<It> range) {
  return std::unordered_set<typename Range<It>::ValueType>{
      range.begin(), range.end()
  }.size();
}

template <typename K, typename V>
const V* GetValuePointer(const std::unordered_map<K, V>& map, const K& key) {
  if (auto it = map.find(key); it != end(map)) {
    return &it->second;
  } else {
    return nullptr;
  }
}

std::string_view Strip(std::string_view line);

bool IsEqualAbs(double lhs, double rhs, double eps = 0.0001);
bool IsEqualRel(double lhs, double rhs, double eps = std::numeric_limits<double>::epsilon());
std::string EscapeSpecialCharacters(const std::string& input);
const Json::Node* GetNodeByName(const Json::Dict &dict, const std::string &name);

