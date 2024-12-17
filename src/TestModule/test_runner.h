#pragma once
#include <sstream>
#include <exception>
#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <set>
#include <vector>
#include <optional>


template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec)
{
    os << "(";
    bool first = true;
    for (const auto& x : vec)
    {
        if (!first)
            os << ",";
        first = false;
        os << x;
    }
    return os << ")";
}

template <class T>
std::ostream& operator << (std::ostream& os, const std::set<T>& s) {
    os << "{";
    bool first = true;
    for (const auto& x : s) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << x;
    }
    return os << "}";
}

template <class K, class V>
std::ostream& operator << (std::ostream& os, const std::map<K, V>& m) {
    os << "{";
    bool first = true;
    for (const auto& kv : m) {
        if (!first) {
            os << ", ";
        }
        first = false;
        os << kv.first << ": " << kv.second;
    }
    return os << "}";
}

template<class K, class V>
std::ostream& operator << (std::ostream& os, const std::unordered_map<K, V>& m)
{
    os << "{";
    bool first = true;
    for (const auto& kv : m) {
      if (!first) {
        os << ", ";
      }
      first = false;
      os << kv.first << ": " << kv.second;
    }
    return os << "}";
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::optional<T>& opt)
{
    if(opt.has_value())
    {
        os << *opt;
    }
    else
    {
        os << "std::nullopt";
    }
    return os;
}

template<class T, class U>
void AssertEqual(const T& t, const U& u,
    const std::string& hint)
{
    if (t != u) {
        std::ostringstream os;
        os << "Assertion failed: " << t << " != " << u
            << " hint: " << hint;
        throw std::runtime_error(os.str());
    }
}

inline void Assert(bool b, const std::string& hint) {
    AssertEqual(b, true, hint);
}

class TestRunner {
public:
    template <class TestFunc>
    void RunTest(TestFunc func, const std::string& test_name) const {
        try {
            func();
            std::cerr << test_name << " OK" << std::endl;
        }
        catch (std::runtime_error& e) {
            ++fail_count;
            std::cerr << test_name << " fail: " << e.what() << std::endl;
        }
    }

    ~TestRunner() {
        if (fail_count > 0) {
            std::cerr << fail_count << " unit tests failed. Terminate" << std::endl;
            exit(1);
        }
    }

private:
    mutable int fail_count = 0;
};

#define ASSERT_EQUAL(x, y) {                       \
  std::ostringstream assertEqualOS;                \
  assertEqualOS << #x << " != " << #y << ", "      \
    << __FILE__ << ":" << __LINE__;                \
  AssertEqual(x, y, assertEqualOS.str());          \
}

#define ASSERT(x){                                 \
    std::ostringstream assertEqualOS;              \
    assertEqualOS << #x << "is false, "            \
    << __FILE__ << ":" << __LINE__;                \
    Assert(x, assertEqualOS.str());                \
} 

#define RUN_TEST(tr, func) \
    tr.RunTest(func, #func)
