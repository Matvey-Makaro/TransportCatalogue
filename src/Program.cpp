#include <iomanip>
#include "Descriptions.h"
#include "Json.h"
#include "Requests.h"
#include "Sphere.h"
#include "TransportDatabase.h"
#include "RenderSettings.h"
#include "Utils.h"
#include "Program.h"
#include "Svg/MapVisualizer.h"

using namespace std;

Program::Program() {}

void Program::Run(std::istream& in, std::ostream& out)
{
    const auto input_doc = Json::Load(in);
    const auto& input_map = input_doc.GetRoot().AsMap();

    const TransportDatabase db(
        Descriptions::ReadDescriptions(input_map.at("base_requests").AsArray()),
        input_map.at("routing_settings").AsMap());
    const Svg::MapVisualizer mapVisualizer(db.GetStopsDescriptions(),
        db.GetBusesDescriptions(),
        RenderSettings::ParseFrom(input_map.at("render_settings").AsMap()));


    out << std::fixed << std::setprecision(14);
    Json::PrintValue(
        Requests::ProcessAll(db, mapVisualizer, input_map.at("stat_requests").AsArray()),
        out
    );
    out << endl;
}
