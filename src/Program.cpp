#include "descriptions.h"
#include "json.h"
#include "requests.h"
#include "sphere.h"
#include "transport_catalog.h"
#include "RenderSettings.h"
#include "utils.h"
#include "Program.h"

using namespace std;

Program::Program() {}

void Program::Run(std::istream &in, std::ostream &out)
{
    const auto input_doc = Json::Load(in);
    const auto& input_map = input_doc.GetRoot().AsMap();

    const TransportCatalog db(
        Descriptions::ReadDescriptions(input_map.at("base_requests").AsArray()),
        input_map.at("routing_settings").AsMap(),
        RenderSettings::ParseFrom(input_map.at("render_settings").AsMap())
        );

    Json::PrintValue(
        Requests::ProcessAll(db, input_map.at("stat_requests").AsArray()),
        out
        );
    out << endl;
}
