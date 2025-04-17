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
#include "SerializationSettings.h"
#include "proto/ProtoSerializer.h"
#include "TransportRouter.h"
#include "YellowPages/JsonParser.h"

using namespace std;

Program::Program(std::string mode) :
    _mode(std::move(mode))
{
}

void Program::Run(std::istream& in, std::ostream& out)
{
    if (_mode == "make_base")
    {
        MakeBase(in);
    }
    else if (_mode == "process_requests")
    {
        ProcessRequests(in, out);
    }
}

void Program::MakeBase(std::istream& in)
{
    const auto inputDoc = Json::Load(in);
    const auto& inputMap = inputDoc.GetRoot().AsMap();

    const TransportDatabase transportDb(
        Descriptions::ReadDescriptions(inputMap.at("base_requests").AsArray()),
        Router::RoutingSettings::FromJson(inputMap.at("routing_settings").AsMap()),
        Visualization::RenderSettings::ParseFrom(inputMap.at("render_settings").AsMap()));

    const auto yellowPagesDb = YellowPages::BLL::JsonParser::ParseYellowPages(inputMap.at("yellow_pages").AsMap());

    auto serializationSettings = SerializationSettings::ParseFrom(inputMap.at("serialization_settings").AsMap());

    Serialization::ProtoSerializer::Serialize(serializationSettings, transportDb, yellowPagesDb);
}

void Program::ProcessRequests(std::istream& in, std::ostream& out)
{
    const auto inputDoc = Json::Load(in);
    const auto& inputMap = inputDoc.GetRoot().AsMap();
    auto serializationSettings = SerializationSettings::ParseFrom(inputMap.at("serialization_settings").AsMap());

    auto [transportDbDTO, yellowPagesDbDTO] = Serialization::ProtoSerializer::Deserialize(serializationSettings);
    auto transportDb = TransportDatabaseShp(std::move(transportDbDTO));
    auto yellowPagesDb = YellowPages::BLL::YellowPagesDatabaseShp(std::move(yellowPagesDbDTO));

    const auto mapVisualizer = std::make_shared<Svg::MapVisualizer>(
        yellowPagesDb,
        transportDb->GetStopsDescriptions(),
        transportDb->GetBusesDescriptions(),
        transportDb->GetRenderSettings());


    auto context = std::make_shared<Requests::Context>(Requests::Context{
        .transportDb = std::move(transportDb), 
        .yellowPagesDb = std::move(yellowPagesDb), 
        .mapVisualizer = mapVisualizer});

    out << std::fixed << std::setprecision(14);
    Json::PrintValue(
        Requests::ProcessAll(context,inputMap.at("stat_requests").AsArray()),
        out
    );
    out << endl;
}
