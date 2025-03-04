#pragma once

#include <iostream>
#include <map>
#include <functional>
#include <string_view>
#include <memory>
#include <string_view>
#include <unordered_map>
#include <optional>
#include "StopsMappers.h"
#include "descriptions.h"
#include "RenderSettings.h"
#include "Document.h"
#include "transport_router.h"

namespace Descriptions
{
    class Bus;
}

namespace Svg
{
    class MapVisualizer
    {
        struct RouteItem
        {
            const Descriptions::Bus* bus;
            std::vector<std::string> stopNames;
        };
        using Route = std::vector<RouteItem>;

    public:
        MapVisualizer(const std::vector<const Descriptions::Stop*>& stops,
            const std::vector<const Descriptions::Bus*>& buses,
            const RenderSettings& renderSettins);

        void Render(std::ostream& out) const;
        void RenderRoute(std::ostream& out, const TransportRouter::RouteInfo& routeInfo, const std::string& finishStopName) const;

    private:
        void RenderWholeMapIfNeeded() const;
        using RenderFunc = std::function<void(const MapVisualizer&)>;
        RenderFunc GetRenderFuncByLayerName(const std::string& layerName) const;
        void RenderAllBusesLines() const;
        void RenderAllBusesNames() const;
        void RenderAllStopPoints() const;
        void RenderAllStopNames() const;

        Route MapRoute(const TransportRouter::RouteInfo& routeInfo, const std::string& finishStopName) const;
        RouteItem MapRouteItem(const TransportRouter::RouteInfo::BusItem* busItem,
            std::string_view firstStopName,
            std::string_view lastStopName) const;

        void RenderTranslucentRect(Document& doc) const;
        void RenderRouteBusesLines(Document& doc, const Route& route) const;
        void RenderRouteBusesNames(Document& doc, const Route& route) const;


        void RenderBusName(Document& doc, const std::string& busName, const std::string& stopName, const Color& busColor) const;


        void CalculateBusColors();
        const Color& GetBusColor(const Descriptions::Bus* bus) const;

        template<typename It>
        void RenderBusLine(Document& doc,
            const Descriptions::Bus* bus,
            It firstStop, It lastStop) const
        {
            Polyline busPath;
            busPath.SetPrecision(Precision);
            for (auto it = firstStop; it != lastStop; it++)
            {
                auto stopIt = _stops.find(*it);
                assert(stopIt != _stops.end());
                if (stopIt == _stops.end())
                    continue;
                const auto& position = stopIt->second;
                busPath.AddPoint(position);
            }
            busPath.SetStrokeColor(GetBusColor(bus));
            busPath.SetStrokeWidth(_renderSettings.busLineWidth);
            busPath.SetStrokeLineCap(DefaultStrokeLineCap);
            busPath.SetStrokeLineJoin(DefaultStrokeLineJoin);
            doc.Add(busPath);
        }

    private:
        static const std::string DefaultFontFamily;
        static const std::string DefaultStrokeLineCap;
        static const std::string DefaultStrokeLineJoin;
        static const int Precision = 15;

        IStopsMapperUnp _stopsMapper;
        std::map<std::string, Point> _stops;
        // TODO: Переделать на string_view из самих buses
        std::map<std::string, const Descriptions::Bus*> _buses;
        RenderSettings _renderSettings;
        mutable Document _mapDoc;
        std::unordered_map<std::string_view, Color> _busNameToColor;
    };

}

