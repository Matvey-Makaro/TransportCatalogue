#include <iostream>
#include <fstream>
#include "ProtoSerializer.h"
#include "SerializationSettings.h"
#include "TransportDatabase.h"
#include "TransportCatalog/TransportCatalogProtoMapper.h"
#include "YellowPages/ProtoMapper.h"
#include "YellowPages/YellowPagesDatabase.h"

using namespace Serialization;

void Serialization::ProtoSerializer::Serialize(const SerializationSettings &settings,
                                               const TransportDatabase &transportDb,
                                               const YellowPages::BLL::YellowPagesDatabase &yellowPagesDb)
{
    std::ofstream out(settings.file, std::ios::out | std::ios::binary);
    auto transportCatalog = TransportCatalogProtoMapper::Map(transportDb);
    transportCatalog.mutable_yellow_pages_database()->CopyFrom(YellowPages::ProtoMapper::Map(yellowPagesDb));
    bool isSuccess = transportCatalog.SerializeToOstream(&out);
    if (!isSuccess)
    {
        std::cerr << "ProtoSerializer::Serialize() transportCatalog.SerializeToOstream failed!" << std::endl;
    }
}

std::pair<TransportDatabaseUnp, YellowPages::BLL::YellowPagesDatabaseUnp> Serialization::ProtoSerializer::Deserialize(
    const SerializationSettings &settings)
{
    std::ifstream in(settings.file, std::ios::in | std::ios::binary);
    TransportCatalog transportCatalog;
    {
        bool isSuccess = transportCatalog.ParseFromIstream(&in);
        if (!isSuccess)
        {
            std::cerr << "ProtoSerializer::Deserialize() transportCatalog.ParseFromIstream failed!" << std::endl;
        }
    }
   
    return std::make_pair(std::make_unique<TransportDatabase>(TransportCatalogProtoMapper::Map(transportCatalog)),
                          std::make_unique<YellowPages::BLL::YellowPagesDatabase>(YellowPages::ProtoMapper::Map(transportCatalog.yellow_pages_database())));
}
