#pragma once

#include "Point.h"
#include "sphere.h"

namespace Svg
{
class CoordinateMapper
{
public:
    CoordinateMapper();

    void SetMaxWidth(double maxWidth);
    void SetMaxHeight(double maxHeight);
    void SetPadding(double padding);
    void SetMaxLon(double maxLon);
    void SetMinLon(double minLon);
    void SetMaxLat(double maxLat);
    void SetMinLat(double minLat);

    Svg::Point Map(const Sphere::Point& geoPoint) const;

private:
    double CalculateZoomCoef() const;

private:
    double _maxWidth;
    double _maxHeight;
    double _padding;
    double _maxLon;
    double _minLon;
    double _maxLat;
    double _minLat;
};

}
