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

    const TransportDatabase db(
        Descriptions::ReadDescriptions(inputMap.at("base_requests").AsArray()),
        Router::RoutingSettings::FromJson(inputMap.at("routing_settings").AsMap()));

    auto serializationSettings = SerializationSettings::ParseFrom(inputMap.at("serialization_settings").AsMap());
    Serialization::ProtoSerializer::Serialize(serializationSettings, db);
}

void Program::ProcessRequests(std::istream& in, std::ostream& out)
{
    const auto inputDoc = Json::Load(in);
    const auto& inputMap = inputDoc.GetRoot().AsMap();
    auto serializationSettings = SerializationSettings::ParseFrom(inputMap.at("serialization_settings").AsMap());
    auto db = Serialization::ProtoSerializer::Deserialize(serializationSettings);

    // TODO: Пока что реализация без mapVisualizer
    // const Svg::MapVisualizer mapVisualizer(db.GetStopsDescriptions(),
    //     db.GetBusesDescriptions(),
    //     RenderSettings::ParseFrom(input_map.at("render_settings").AsMap()));

    const Svg::MapVisualizer mapVisualizer(db.GetStopsDescriptions(),
        db.GetBusesDescriptions(),
        Visualization::RenderSettings{});

    out << std::fixed << std::setprecision(14);
    Json::PrintValue(
        Requests::ProcessAll(db, mapVisualizer, inputMap.at("stat_requests").AsArray()),
        out
    );
    out << endl;
}
