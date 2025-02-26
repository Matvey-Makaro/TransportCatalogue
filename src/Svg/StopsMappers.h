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
}