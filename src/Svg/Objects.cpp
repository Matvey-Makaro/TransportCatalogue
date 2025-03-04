#include <iostream>
#include <sstream>
#include "Objects.h"
#include "utils.h"

using namespace Svg;

Circle::Circle() :
    _center(DefaultCenter),
    _radius(DefaultRadius)
{
}

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

void Circle::Render(std::ostream& out) const
{
    RenderOpenTag(out);
    RenderCenter(out);
    RenderRadius(out);
    RenderCommonProperties(out);
    RenderCloseTag(out);
}

void Circle::RenderOpenTag(std::ostream& out) const
{
    out << EscapeSpecialCharacters("<circle ");
}

void Circle::RenderCenter(std::ostream& out) const
{
    RenderProperty(out, "cx", _center.x);
    RenderProperty(out, "cy", _center.y);
}

void Circle::RenderRadius(std::ostream& out) const
{
    RenderProperty(out, "r", _radius);
}

void Circle::RenderCloseTag(std::ostream& out) const
{
    out << EscapeSpecialCharacters("/>");
}

Polyline& Polyline::AddPoint(const Point& point)
{
    _points.push_back(point);
    return *this;
}

void Polyline::Render(std::ostream& out) const
{
    RenderOpenTag(out);
    RenderPoints(out);
    RenderCommonProperties(out);
    RenderCloseTag(out);
}

void Polyline::RenderOpenTag(std::ostream& out) const
{
    out << EscapeSpecialCharacters("<polyline ");
}

void Polyline::RenderPoints(std::ostream& out) const
{
    RenderProperty(out, "points", FormPointsStr());
}

void Polyline::RenderCloseTag(std::ostream& out) const
{
    out << EscapeSpecialCharacters("/>");
}

std::string Polyline::FormPointsStr() const
{
    std::stringstream ss;
    ss << std::setprecision(_precision);
    for (const auto& p : _points)
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
    _data(),
    _fontWeight()
{
}

Text& Text::SetPoint(const Point& point)
{
    _point = point;
    return *this;
}

Text& Text::SetOffset(const Point& offset)
{
    _offset = offset;
    return *this;
}

Text& Text::SetFontSize(uint32_t fontSize)
{
    _fontSize = fontSize;
    return *this;
}

Text& Text::SetFontFamily(const std::string& fontFamily)
{
    _fontFamily = fontFamily;
    return *this;
}

Text& Text::SetFontWeight(const std::string& fontWeight)
{
    _fontWeight = fontWeight;
    return *this;
}

Text& Text::SetData(const std::string& data)
{
    _data = data;
    return *this;
}

void Text::Render(std::ostream& out) const
{
    RenderOpenTag(out);
    RenderPoint(out);
    RenderOffset(out);
    RenderFontSize(out);
    RenderFontFamily(out);
    RenderFontWeight(out);
    RenderCommonProperties(out);
    RenderClosePropertiesTag(out);
    RenderData(out);
    RenderCloseTag(out);
}

void Text::RenderOpenTag(std::ostream& out) const
{
    out << EscapeSpecialCharacters("<text ");
}

void Text::RenderPoint(std::ostream& out) const
{
    RenderProperty(out, "x", _point.x);
    RenderProperty(out, "y", _point.y);
}

void Text::RenderOffset(std::ostream& out) const
{
    RenderProperty(out, "dx", _offset.x);
    RenderProperty(out, "dy", _offset.y);
}

void Text::RenderFontSize(std::ostream& out) const
{
    RenderProperty(out, "font-size", _fontSize);
}

void Text::RenderFontFamily(std::ostream& out) const
{
    if (!_fontFamily.empty())
        RenderProperty(out, "font-family", _fontFamily);
}

void Text::RenderFontWeight(std::ostream& out) const
{
    if (_fontWeight && !_fontWeight->empty())
        RenderProperty(out, "font-weight", *_fontWeight);
}

void Text::RenderClosePropertiesTag(std::ostream& out) const
{
    out << '>';
}

void Text::RenderData(std::ostream& out) const
{
    out << _data;
}

void Text::RenderCloseTag(std::ostream& out) const
{
    out << EscapeSpecialCharacters("</text>");
}

Svg::Rect::Rect() :
    _leftTop{0., 0.},
    _width(0.),
    _height(0.)
{
}

Rect& Svg::Rect::SetLeftTop(const Point& p)
{
    _leftTop = p;
    return *this;
}

Rect& Svg::Rect::SetWidth(double width)
{
    _width = width;
    return *this;
}

Rect& Svg::Rect::SetHeight(double height)
{
    _height = height;
    return *this;
}

void Svg::Rect::Render(std::ostream& out) const
{
    RenderOpenTag(out);
    RenderPoint(out);
    RenderSize(out);
    RenderCommonProperties(out);
    RenderCloseTag(out);
}

void Svg::Rect::RenderOpenTag(std::ostream& out) const
{
    out << EscapeSpecialCharacters("<rect ");
}

void Svg::Rect::RenderPoint(std::ostream& out) const
{
    RenderProperty(out, "x", _leftTop.x);
    RenderProperty(out, "y", _leftTop.y);
}

void Svg::Rect::RenderSize(std::ostream& out) const
{
    RenderProperty(out, "width", _width);
    RenderProperty(out, "height", _height);
}

void Svg::Rect::RenderCloseTag(std::ostream& out) const
{
    out << EscapeSpecialCharacters("/>");
}
