#pragma once

#include <cmath>

namespace Sphere {
  double ConvertDegreesToRadians(double degrees);

  struct Point {
    double latitude;
    double longitude;

    static Point FromDegrees(double latitude, double longitude);
  };

  double Distance(Point lhs, Point rhs);
  bool IsEqual(const Point& lhs, const Point& rhs, double eps = std::numeric_limits<double>::epsilon());
}