#include <iostream>
#include <sstream>
#include "Objects.h"
#include "utils.h"

using namespace Svg;

ObjectCommon::ObjectCommon() :
    _fillColor(NoneColor),
    _strokeColor(NoneColor),
    _strokeWidth(DefaultStrokeWidth),
    _strokeLineCap(),
    _strokeLineJoin()
{}

ObjectCommon &ObjectCommon::SetFillColor(const Color &fillColor)
{
    _fillColor = fillColor;
    return *this;
}

ObjectCommon &ObjectCommon::SetStrokeColor(const Color &strokeColor)
{
    _strokeColor = strokeColor;
    return *this;
}

ObjectCommon &ObjectCommon::SetStrokeWidth(double strokeWidth)
{
    _strokeWidth = strokeWidth;
    return *this;
}

ObjectCommon &ObjectCommon::SetStrokeLineCap(const std::string &strokeLineCap)
{
    _strokeLineCap = strokeLineCap;
    return *this;
}

ObjectCommon &ObjectCommon::SetStrokeLineJoin(const std::string &strokeLineJoin)
{
    _strokeLineJoin = strokeLineJoin;
    return *this;
}

void ObjectCommon::RenderCommonProperties(std::ostream& out) const
{
    RenderProperty(out, "fill", _fillColor);
    RenderProperty(out, "stroke", _strokeColor);
    RenderProperty(out, "stroke-width", _strokeWidth);
    if(!_strokeLineCap.empty())
    {
        RenderProperty(out, "stroke-linecap", _strokeLineCap);
    }
    if(!_strokeLineJoin.empty())
    {
        RenderProperty(out, "stroke-linejoin", _strokeLineJoin);
    }
}


Circle::Circle() :
    _center(DefaultCenter),
    _radius(DefaultRadius)
{}

Circle& Circle::SetCenter(const Point& center)
{
    _center = center;
    return *this;
}

Circle& Circle::SetRadius(double radius)
{
    _radius = radius;
    return *this;
}

Circle &Circle::SetFillColor(const Color& fillColor)
{
    ObjectCommon::SetFillColor(fillColor);
    return *this;
}

Circle &Circle::SetStrokeColor(const Color &strokeColor)
{
    ObjectCommon::SetStrokeColor(strokeColor);
    return *this;
}

Circle &Circle::SetStrokeWidth(double strokeWidth)
{
    ObjectCommon::SetStrokeWidth(strokeWidth);
    return *this;
}

Circle &Circle::SetStrokeLineCap(const std::string &strokeLineCap)
{
    ObjectCommon::SetStrokeLineCap(strokeLineCap);
    return *this;
}

Circle &Circle::SetStrokeLineJoin(const std::string &strokeLineJoin)
{
    ObjectCommon::SetStrokeLineJoin(strokeLineJoin);
    return *this;
}

void Circle::Render(std::ostream& out) const
{
    RenderOpenTag(out);
    RenderCenter(out);
    RenderRadius(out);
    RenderCommonProperties(out);
    RenderCloseTag(out);
}

void Circle::RenderOpenTag(std::ostream &out) const
{
    out << EscapeSpecialCharacters("<circle ");
}

void Circle::RenderCenter(std::ostream &out) const
{
    RenderProperty(out, "cx", _center.x);
    RenderProperty(out, "cy", _center.y);
}

void Circle::RenderRadius(std::ostream &out) const
{
    RenderProperty(out, "r", _radius);
}

void Circle::RenderCloseTag(std::ostream &out) const
{
    out << EscapeSpecialCharacters("/>");
}

Polyline &Polyline::AddPoint(const Point &point)
{
    _points.push_back(point);
    return *this;
}

Polyline &Polyline::SetFillColor(const Color &fillColor)
{
    ObjectCommon::SetFillColor(fillColor);
    return *this;
}

Polyline &Polyline::SetStrokeColor(const Color &strokeColor)
{
    ObjectCommon::SetStrokeColor(strokeColor);
    return *this;
}

Polyline &Polyline::SetStrokeWidth(double strokeWidth)
{
    ObjectCommon::SetStrokeWidth(strokeWidth);
    return *this;
}

Polyline &Polyline::SetStrokeLineCap(const std::string &strokeLineCap)
{
    ObjectCommon::SetStrokeLineCap(strokeLineCap);
    return *this;
}

Polyline &Polyline::SetStrokeLineJoin(const std::string &strokeLineJoin)
{
    ObjectCommon::SetStrokeLineJoin(strokeLineJoin);
    return *this;
}

void Polyline::Render(std::ostream &out) const
{
    RenderOpenTag(out);
    RenderPoints(out);
    RenderCommonProperties(out);
    RenderCloseTag(out);
}

void Polyline::RenderOpenTag(std::ostream &out) const
{
    out << EscapeSpecialCharacters("<polyline ");
}

void Polyline::RenderPoints(std::ostream &out) const
{
    RenderProperty(out, "points", FormPointsStr());
}

void Polyline::RenderCloseTag(std::ostream &out) const
{
    out << EscapeSpecialCharacters("/>");
}

std::string Polyline::FormPointsStr() const
{
    std::stringstream ss;
    for(const auto& p : _points)
    {
        ss << p.x << "," << p.y << ' ';
    }
    return ss.str();
}

Text::Text() :
    _point(DefaultPoint),
    _offset(DefaultOffset),
    _fontSize(DefaultFontSize),
    _fontFamily(),
    _data()
{}

Text &Text::SetPoint(const Point &point)
{
    _point = point;
    return *this;
}

Text &Text::SetOffset(const Point &offset)
{
    _offset = offset;
    return *this;
}

Text &Text::SetFontSize(uint32_t fontSize)
{
    _fontSize = fontSize;
    return *this;
}

Text &Text::SetFontFamily(const std::string &fontFamily)
{
    _fontFamily = fontFamily;
    return *this;
}

Text &Text::SetData(const std::string &data)
{
    _data = data;
    return *this;
}

Text &Text::SetFillColor(const Color &fillColor)
{
    ObjectCommon::SetFillColor(fillColor);
    return *this;
}

Text &Text::SetStrokeColor(const Color &strokeColor)
{
    ObjectCommon::SetStrokeColor(strokeColor);
    return *this;
}

Text &Text::SetStrokeWidth(double strokeWidth)
{
    ObjectCommon::SetStrokeWidth(strokeWidth);
    return *this;
}

Text &Text::SetStrokeLineCap(const std::string &strokeLineCap)
{
    ObjectCommon::SetStrokeLineCap(strokeLineCap);
    return *this;
}

Text &Text::SetStrokeLineJoin(const std::string &strokeLineJoin)
{
    ObjectCommon::SetStrokeLineJoin(strokeLineJoin);
    return *this;
}

void Text::Render(std::ostream &out) const
{
    RenderOpenTag(out);
    RenderPoint(out);
    RenderOffset(out);
    RenderFontSize(out);
    RenderFontFamily(out);
    RenderCommonProperties(out);
    RenderClosePropertiesTag(out);
    RenderData(out);
    RenderCloseTag(out);
}

void Text::RenderOpenTag(std::ostream &out) const
{
    out << EscapeSpecialCharacters("<text ");
}

void Text::RenderPoint(std::ostream &out) const
{
    RenderProperty(out, "x", _point.x);
    RenderProperty(out, "y", _point.y);
}

void Text::RenderOffset(std::ostream &out) const
{
    RenderProperty(out, "dx", _offset.x);
    RenderProperty(out, "dy", _offset.y);
}

void Text::RenderFontSize(std::ostream &out) const
{
    RenderProperty(out, "font-size", _fontSize);
}

void Text::RenderFontFamily(std::ostream &out) const
{
    if(!_fontFamily.empty())
        RenderProperty(out, "font-family", _fontFamily);
}

void Text::RenderClosePropertiesTag(std::ostream &out) const
{
    out << '>';
}

void Text::RenderData(std::ostream &out) const
{
    out << _data;
}

void Text::RenderCloseTag(std::ostream &out) const
{
    out << EscapeSpecialCharacters("</text>");
}
