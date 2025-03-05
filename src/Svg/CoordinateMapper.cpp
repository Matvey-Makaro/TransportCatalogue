#include <optional>
#include "Utils.h"
#include "CoordinateMapper.h"

using namespace Svg;

CoordinateMapper::CoordinateMapper() :
    _maxWidth(0.0),
    _maxHeight(0.0),
    _padding(0.0),
    _maxLon(0.0),
    _minLon(0.0),
    _maxLat(0.0),
    _minLat(0.0)
{}

void CoordinateMapper::SetMaxWidth(double maxWidth)
{
    _maxWidth = maxWidth;
}

void CoordinateMapper::SetMaxHeight(double maxHeight)
{
    _maxHeight = maxHeight;
}

void CoordinateMapper::SetPadding(double padding)
{
    _padding = padding;
}

void CoordinateMapper::SetMaxLon(double maxLon)
{
    _maxLon = maxLon;
}

void CoordinateMapper::SetMinLon(double minLon)
{
    _minLon = minLon;
}

void CoordinateMapper::SetMaxLat(double maxLat)
{
    _maxLat = maxLat;
}

void CoordinateMapper::SetMinLat(double minLat)
{
    _minLat = minLat;
}

Point CoordinateMapper::Map(const Sphere::Point &geoPoint) const
{
    double zoomCoef = CalculateZoomCoef();
    return Point{
        .x = (geoPoint.longitude - _minLon) * zoomCoef + _padding,
        .y = (_maxLat - geoPoint.latitude) * zoomCoef + _padding
    };
}

double CoordinateMapper::CalculateZoomCoef() const
{
    std::optional<double> widthZoomCoef = std::nullopt;
    if(!IsEqual(_maxLon - _minLon, 0.0))
        widthZoomCoef = (_maxWidth - 2 * _padding) / (_maxLon - _minLon);

    std::optional<double> heightZoomCoef = std::nullopt;
    if(!IsEqual(_maxLat - _minLat, 0.0))
        heightZoomCoef = (_maxHeight - 2 * _padding) / (_maxLat - _minLat);

    double zoomCoef = 0.0;
    if(widthZoomCoef.has_value() && heightZoomCoef.has_value())
    {
        zoomCoef = std::min(*widthZoomCoef, *heightZoomCoef);
    }
    else if(widthZoomCoef.has_value())
    {
        zoomCoef = *widthZoomCoef;
    }
    else if(heightZoomCoef.has_value())
    {
        zoomCoef = *heightZoomCoef;
    }
    return zoomCoef;
}
