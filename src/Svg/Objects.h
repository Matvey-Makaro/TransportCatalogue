#pragma once

#include <variant>
#include <vector>
#include <optional>
#include <iomanip>
#include "Color.h"
#include "Point.h"
#include "Utils.h"

namespace Svg
{
    template <typename SpecificObject>
    class ObjectCommon
    {
    public:
        SpecificObject& SetPrecision(int precision)
        {
            _precision = precision;
            return static_cast<SpecificObject&>(*this);
        }
        SpecificObject& SetFillColor(const Color& fillColor)
        {
            _fillColor = fillColor;
            return static_cast<SpecificObject&>(*this);
        }
        SpecificObject& SetStrokeColor(const Color& strokeColor)
        {
            _strokeColor = strokeColor;
            return static_cast<SpecificObject&>(*this);
        }
        SpecificObject& SetStrokeWidth(double strokeWidth)
        {
            _strokeWidth = strokeWidth;
            return static_cast<SpecificObject&>(*this);
        }
        SpecificObject& SetStrokeLineCap(const std::string& strokeLineCap)
        {
            _strokeLineCap = strokeLineCap;
            return static_cast<SpecificObject&>(*this);
        }
        SpecificObject& SetStrokeLineJoin(const std::string& strokeLineJoin)
        {
            _strokeLineJoin = strokeLineJoin;
            return static_cast<SpecificObject&>(*this);
        }

    protected:
        ObjectCommon() : _fillColor(NoneColor),
            _strokeColor(NoneColor),
            _strokeWidth(DefaultStrokeWidth),
            _strokeLineCap(),
            _strokeLineJoin()
        {
        }

        template <typename K, typename V>
        void RenderProperty(std::ostream& out, const K& name, const V& value) const
        {
            out << std::setprecision(_precision)
                << name
                << EscapeSpecialCharacters("=")
                << EscapeSpecialCharacters("\"")
                << value
                << EscapeSpecialCharacters("\" ");
        }

        void RenderCommonProperties(std::ostream& out) const
        {
            RenderProperty(out, "fill", _fillColor);
            RenderProperty(out, "stroke", _strokeColor);
            RenderProperty(out, "stroke-width", _strokeWidth);
            if (!_strokeLineCap.empty())
            {
                RenderProperty(out, "stroke-linecap", _strokeLineCap);
            }
            if (!_strokeLineJoin.empty())
            {
                RenderProperty(out, "stroke-linejoin", _strokeLineJoin);
            }
        }

    protected:
        static constexpr double DefaultStrokeWidth = 1.0;
        int _precision = 15;

        Color _fillColor;
        Color _strokeColor;
        double _strokeWidth;
        std::string _strokeLineCap;
        std::string _strokeLineJoin;
    };

    class Circle : public ObjectCommon<Circle>
    {
    public:
        Circle();

        Circle& SetCenter(const Point& center);
        Circle& SetRadius(double radius);

        void Render(std::ostream& out) const;

    private:
        void RenderOpenTag(std::ostream& out) const;
        void RenderCenter(std::ostream& out) const;
        void RenderRadius(std::ostream& out) const;
        void RenderCloseTag(std::ostream& out) const;

    private:
        static constexpr Point DefaultCenter{ 0.0, 0.0 };
        static constexpr double DefaultRadius = 1.0;

        Point _center;
        double _radius;
    };

    class Polyline : public ObjectCommon<Polyline>
    {
    public:
        Polyline() = default;
        ~Polyline() = default;

        Polyline(const Polyline& other) = default;
        Polyline(Polyline&& other) = default;

        Polyline& operator=(const Polyline& other) = default;
        Polyline& operator=(Polyline&& other) = default;

        Polyline& AddPoint(const Point& point);
        void Render(std::ostream& out) const;

    private:
        void RenderOpenTag(std::ostream& out) const;
        void RenderPoints(std::ostream& out) const;
        void RenderCloseTag(std::ostream& out) const;

        std::string FormPointsStr() const;

    private:
        std::vector<Point> _points;
    };

    class Text : public ObjectCommon<Text>
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
        static constexpr Point DefaultPoint{ 0.0, 0.0 };
        static constexpr Point DefaultOffset{ 0.0, 0.0 };
        static constexpr uint32_t DefaultFontSize = -1;

        Point _point;
        Point _offset;
        uint32_t _fontSize;
        std::string _fontFamily;
        std::string _data;
        std::optional<std::string> _fontWeight;
    };

    class Rect : public ObjectCommon<Rect>
    {
    public:
        Rect();
        ~Rect() = default;

        Rect(const Rect& other) = default;
        Rect(Rect&& other) = default;

        Rect& operator=(const Rect& other) = default;
        Rect& operator=(Rect&& other) = default;

        Rect& SetLeftTop(const Point& p);
        Rect& SetWidth(double width);
        Rect& SetHeight(double height);

        void Render(std::ostream& out) const;

    private:
        void RenderOpenTag(std::ostream& out) const;
        void RenderPoint(std::ostream& out) const;
        void RenderSize(std::ostream& out) const;
        void RenderCloseTag(std::ostream& out) const;

    private:
        Point _leftTop;
        double _width;
        double _height;
    };

    using Object = std::variant<Circle,
        Polyline,
        Text,
        Rect>;

}
