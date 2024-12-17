#pragma once

#include <iostream>
#include <map>
#include "CoordinateMapper.h"
#include "descriptions.h"
#include "RenderSettings.h"
#include "Document.h"

namespace Svg
{
class MapVisualizer
{
public:
    MapVisualizer(const std::vector<const Descriptions::Stop*>& stops,
                  const std::vector<const Descriptions::Bus*>& buses,
                  const RenderSettings& renderSettins);

    void Render(std::ostream& out) const;

private:
    void SetupCoordinateMapper();
    void RenderBuses() const;
    void RenderStopCircles() const;
    void RenderStopNames() const;


private:
    std::map<std::string, const Descriptions::Stop*> _stops;
    std::map<std::string, const Descriptions::Bus*> _buses;
    RenderSettings _renderSettins;
    CoordinateMapper _coordMapper;
    mutable Document _mapDoc;
};

}

