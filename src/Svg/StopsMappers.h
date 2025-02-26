#pragma once

#include <map>
#include <string>
#include <memory>
#include "descriptions.h"
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

    // class ZipWithGluing : public IStopsMapper
    // {
    // public:
    //     ZipWithGluing(const RenderSettings& renderSettings, const std::vector<const Descriptions::Bus *>& buses);
    //     std::map<std::string, Point> Map(const std::vector<const Descriptions::Stop *> &stops) override;

    // private:
    //     bool IsRouteNeighbors(const Descriptions::Stop* lhs, const Descriptions::Stop* rhs) const;

    // private:
    //     RenderSettings _renderSettings;
    // };
}