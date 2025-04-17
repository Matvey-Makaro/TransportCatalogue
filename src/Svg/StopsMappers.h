#pragma once

#include <map>
#include <string>
#include <memory>
#include <unordered_set>
#include <tuple>
#include "Descriptions.h"
#include "Point.h"
#include "RenderSettings.h"
#include "CoordinateMapper.h"

namespace YellowPages::BLL
{
    class Company;
}

namespace Svg
{
    class IStopsMapper
    {
    public:
        IStopsMapper() = default;
        virtual ~IStopsMapper() = default;

        using StopNameToCoords = std::map<std::string, Svg::Point>;
        using CompanyNameToCoords = std::map<std::string, Svg::Point>;
        using CoordsPair = std::pair<StopNameToCoords, CompanyNameToCoords>;
        virtual std::pair<StopNameToCoords, CompanyNameToCoords> Map(
            const std::vector<const Descriptions::Stop*>& stops,
            const std::vector<const YellowPages::BLL::Company*>& companies) = 0;
    };
    using IStopsMapperUnp = std::unique_ptr<IStopsMapper>;

    class GeoStopMapper : public IStopsMapper
    {
    public:
        GeoStopMapper(const Visualization::RenderSettings& renderSettings);
        std::pair<StopNameToCoords, CompanyNameToCoords> Map(
            const std::vector<const Descriptions::Stop*>& stops,
            const std::vector<const YellowPages::BLL::Company*>& companies) override;

    private:
        void SetupCoordinateMapper(const std::vector<const Descriptions::Stop*>& stops);

    private:
        Visualization::RenderSettings _renderSettings;
        CoordinateMapper _coordMapper;
    };

    class ZipStopMapper : public IStopsMapper
    {
    public:
        ZipStopMapper(const Visualization::RenderSettings& renderSettings);
        std::pair<StopNameToCoords, CompanyNameToCoords> Map(
            const std::vector<const Descriptions::Stop*>& stops,
            const std::vector<const YellowPages::BLL::Company*>& companies) override;

    private:
        Visualization::RenderSettings _renderSettings;
    };

    class ZipWithGluingStopMapper : public IStopsMapper
    {
    public:
        ZipWithGluingStopMapper(const Visualization::RenderSettings& renderSettings, const std::vector<const Descriptions::Bus*>& buses);
        // TODO: Change to common interface
        CoordsPair Map(const std::vector<const Descriptions::Stop*>& stops,
            const std::vector<const YellowPages::BLL::Company*>& companies) override;

    protected:
        class TransportPoint
        {
        public:
            using EntityType = std::variant<const Descriptions::Stop*, const YellowPages::BLL::Company*>;
            TransportPoint(const EntityType& entity);
            const EntityType GetEntity() const;
            const Sphere::Point& GetCoords() const;
            const std::string GetName() const;

        private:
            EntityType _entity;
        };
        enum class Dimension
        {
            x,
            y
        };

        void FillStopsPositions(const std::vector<const Descriptions::Stop*>& stops);
        void FillCompaniesPositions(const std::vector<const YellowPages::BLL::Company*>& companies);
        void FillSortedTransportPoints(const std::vector<const Descriptions::Stop*>& stops,
            const std::vector<const YellowPages::BLL::Company*>& companies);
        void SortStopsByLongitude();
        void SortStopsByLatitude();
        virtual std::vector<std::vector<TransportPoint*>> CalculateIndexToStops(Dimension dimension);
        void CalculateX(const std::vector<std::vector<TransportPoint*>>& indexToStops);
        void CalculateY(const std::vector<std::vector<TransportPoint*>>& indexToStops);
        bool IsNeighbors(const TransportPoint& lhs, const TransportPoint& rhs) const;
        bool IsNeighbors(const Descriptions::Stop* lhs, const Descriptions::Stop* rhs) const;
        bool IsNeighbors(const YellowPages::BLL::Company* lhs, const YellowPages::BLL::Company* rhs) const;
        bool IsNeighbors(const Descriptions::Stop* lhs, const YellowPages::BLL::Company* rhs) const;
        bool IsNeighbors(const YellowPages::BLL::Company* lhs, const Descriptions::Stop* rhs) const;
        void ClearCache();

    protected:
        Visualization::RenderSettings _renderSettings;
        const std::vector<const Descriptions::Bus*>& _buses;
        StopNameToCoords _stopsPositions;
        CompanyNameToCoords _companiesPositions;
        std::vector<TransportPoint> _sortedTransportPoints;
    };

    class ImprovedZipWithGluingStopMapper : public ZipWithGluingStopMapper
    {
    public:
        using ZipWithGluingStopMapper::ZipWithGluingStopMapper;

    protected:
        std::vector<std::vector<TransportPoint*>> CalculateIndexToStops(Dimension dimension) override;
    };


    class InterpolationZipWithGluingStopMapper : public IStopsMapper
    {
    public:
        InterpolationZipWithGluingStopMapper(const Visualization::RenderSettings& renderSettings,
            const std::vector<const Descriptions::Bus*>& buses);
        // TODO: Change to common interface
        CoordsPair Map(const std::vector<const Descriptions::Stop*>& stops,
            const std::vector<const YellowPages::BLL::Company*>& companies) override;

    private:
        void FillStops(const std::vector<const Descriptions::Stop*>& stops);
        std::unordered_set<std::string> CalculatePivotStops();
        void RecalculateNonPivotStopsPosition(const std::unordered_set<std::string>& pivotStops);
        CoordsPair ZipWithGluing(const std::vector<const YellowPages::BLL::Company*>& companies);
        void ClearCache();

    private:
        Visualization::RenderSettings _renderSettings;
        const std::vector<const Descriptions::Bus*>& _buses;
        std::unordered_map<std::string, Descriptions::Stop> _stops;
    };
}