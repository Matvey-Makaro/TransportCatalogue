#pragma once

#include <map>
#include <string>
#include <memory>
#include <unordered_set>
#include "Descriptions.h"
#include "Point.h"
#include "RenderSettings.h"
#include "CoordinateMapper.h"

namespace Svg
{
    class IStopsMapper
    {
    public:
        IStopsMapper() = default;
        virtual ~IStopsMapper() = default;

        virtual std::map<std::string, Point> Map(const std::vector<const Descriptions::Stop *> &stops) = 0;
    };
    using IStopsMapperUnp = std::unique_ptr<IStopsMapper>;

    class GeoStopMapper : public IStopsMapper
    {
    public:
        GeoStopMapper(const RenderSettings &renderSettings);
        std::map<std::string, Point> Map(const std::vector<const Descriptions::Stop *> &stops) override;

    private:
        void SetupCoordinateMapper(const std::vector<const Descriptions::Stop *> &stops);

    private:
        RenderSettings _renderSettings;
        CoordinateMapper _coordMapper;
    };

    class ZipStopMapper : public IStopsMapper
    {
    public:
        ZipStopMapper(const RenderSettings &renderSettings);
        std::map<std::string, Point> Map(const std::vector<const Descriptions::Stop *> &stops) override;

    private:
        RenderSettings _renderSettings;
    };

    class ZipWithGluingStopMapper : public IStopsMapper
    {
    public:
        ZipWithGluingStopMapper(const RenderSettings &renderSettings, const std::vector<const Descriptions::Bus *> &buses);
        std::map<std::string, Point> Map(const std::vector<const Descriptions::Stop *> &stops) override;

    protected:
        void FillStopsPositions(const std::vector<const Descriptions::Stop *> &stops);
        void FillSortedStops(const std::vector<const Descriptions::Stop *> &stops);
        void SortStopsByLongitude();
        void SortStopsByLatitude();
        virtual std::vector<std::vector<const Descriptions::Stop *>> CalculateIndexToStops();
        void CalculateX(const std::vector<std::vector<const Descriptions::Stop *>> &indexToStops);
        void CalculateY(const std::vector<std::vector<const Descriptions::Stop *>> &indexToStops);
        bool IsRouteNeighbors(const Descriptions::Stop *lhs, const Descriptions::Stop *rhs) const;
        void ClearCache();

    protected:
        RenderSettings _renderSettings;
        const std::vector<const Descriptions::Bus *> &_buses;
        std::map<std::string, Point> _stopsPositions;
        std::vector<const Descriptions::Stop *> _sortedStops;
    };

    class ImprovedZipWithGluingStopMapper : public ZipWithGluingStopMapper
    {
    public:
        using ZipWithGluingStopMapper::ZipWithGluingStopMapper;

    protected:
        std::vector<std::vector<const Descriptions::Stop *>> CalculateIndexToStops() override;
    };

    
    class InterpolationZipWithGluingStopMapper : public IStopsMapper
    {
    public:
        InterpolationZipWithGluingStopMapper(const RenderSettings &renderSettings, const std::vector<const Descriptions::Bus *> &buses);
        std::map<std::string, Point> Map(const std::vector<const Descriptions::Stop *> &stops) override;

    private:
        void FillStops(const std::vector<const Descriptions::Stop *> &stops);
        std::unordered_set<std::string> CalculatePivotStops();
        void RecalculateNonPivotStopsPosition(const std::unordered_set<std::string>& pivotStops);
        std::map<std::string, Point> ZipWithGluing();
        void ClearCache();
        
    private:
        RenderSettings _renderSettings;
        const std::vector<const Descriptions::Bus *> &_buses;
        std::unordered_map<std::string, Descriptions::Stop> _stops;
    };
}