#pragma once

#include <variant>
#include <vector>
#include <optional>
#include <iomanip>
#include "Color.h"
#include "Point.h"
#include "utils.h"

namespace Svg
{

class ObjectCommon
{
public:
    ObjectCommon& SetPrecision(int precision);
protected:
    ObjectCommon();

    ObjectCommon& SetFillColor(const Color& fillColor);
    ObjectCommon& SetStrokeColor(const Color& strokeColor);
    ObjectCommon& SetStrokeWidth(double strokeWidth);
    ObjectCommon& SetStrokeLineCap(const std::string& strokeLineCap);
    ObjectCommon& SetStrokeLineJoin(const std::string& strokeLineJoin);

protected:
    template<typename K, typename V>
    void RenderProperty(std::ostream& out, const K& name, const V& value) const
    {
        out << std::setprecision(_precision)
            << name
            << EscapeSpecialCharacters("=")
            << EscapeSpecialCharacters("\"")
            << value
            << EscapeSpecialCharacters("\" ");
    }

    void RenderCommonProperties(std::ostream& out) const;

protected:
    static constexpr double DefaultStrokeWidth = 1.0;
    int _precision = 15;

    Color _fillColor;
    Color _strokeColor;
    double _strokeWidth;
    std::string _strokeLineCap;
    std::string _strokeLineJoin;
};

class Circle : public ObjectCommon
{
public:
    Circle();

    Circle& SetCenter(const Point& center);
    Circle& SetRadius(double radius);

    Circle& SetFillColor(const Color& fillColor);
    Circle& SetStrokeColor(const Color& strokeColor);
    Circle& SetStrokeWidth(double strokeWidth);
    Circle& SetStrokeLineCap(const std::string& strokeLineCap);
    Circle& SetStrokeLineJoin(const std::string& strokeLineJoin);

    void Render(std::ostream& out) const;

private:
    void RenderOpenTag(std::ostream& out) const;
    void RenderCenter(std::ostream& out) const;
    void RenderRadius(std::ostream& out) const;
    void RenderCloseTag(std::ostream& out) const;

private:
    static constexpr Point DefaultCenter{0.0, 0.0};
    static constexpr double DefaultRadius = 1.0;

    Point _center;
    double _radius;
};


class Polyline : public ObjectCommon
{
public:
    Polyline() = default;
    ~Polyline() = default;

    Polyline(const Polyline& other) = default;
    Polyline(Polyline&& other) = default;

    Polyline& operator=(const Polyline& other) = default;
    Polyline& operator=(Polyline&& other) = default;

    Polyline& AddPoint(const Point& point);
    Polyline& SetFillColor(const Color& fillColor);
    Polyline& SetStrokeColor(const Color& strokeColor);
    Polyline& SetStrokeWidth(double strokeWidth);
    Polyline& SetStrokeLineCap(const std::string& strokeLineCap);
    Polyline& SetStrokeLineJoin(const std::string& strokeLineJoin);

    void Render(std::ostream& out) const;

private:
    void RenderOpenTag(std::ostream& out) const;
    void RenderPoints(std::ostream& out) const;
    void RenderCloseTag(std::ostream& out) const;

    std::string FormPointsStr() const;

private:
    std::vector<Point> _points;
};


class Text : public ObjectCommon
{
public:
    Text();
    ~Text() = default;

    Text(const Text& other) = default;
    Text(Text&& other) = default;

    Text& operator=(const Text& other) = default;
    Text& operator=(Text&& other) = default;

    Text& SetPoint(const Point& point);
    Text& SetOffset(const Point& offset);
    Text& SetFontSize(uint32_t fontSize);
    Text& SetFontFamily(const std::string& fontFamily);
    Text& SetFontWeight(const std::string& fontWeight);
    Text& SetData(const std::string& data);

    Text& SetFillColor(const Color& fillColor);
    Text& SetStrokeColor(const Color& strokeColor);
    Text& SetStrokeWidth(double strokeWidth);
    Text& SetStrokeLineCap(const std::string& strokeLineCap);
    Text& SetStrokeLineJoin(const std::string& strokeLineJoin);

    void Render(std::ostream& out) const;

private:
    void RenderOpenTag(std::ostream& out) const;
    void RenderPoint(std::ostream& out) const;
    void RenderOffset(std::ostream& out) const;
    void RenderFontSize(std::ostream& out) const;
    void RenderFontFamily(std::ostream& out) const;
    void RenderFontWeight(std::ostream& out) const;
    void RenderClosePropertiesTag(std::ostream& out) const;
    void RenderData(std::ostream& out) const;
    void RenderCloseTag(std::ostream& out) const;

private:
    static constexpr Point DefaultPoint{0.0, 0.0};
    static constexpr Point DefaultOffset{0.0, 0.0};
    static constexpr uint32_t DefaultFontSize = -1;

    Point _point;
    Point _offset;
    uint32_t _fontSize;
    std::string _fontFamily;
    std::string _data;
    std::optional<std::string> _fontWeight;
};

using Object = std::variant<Circle,
                            Polyline,
                            Text>;

}

