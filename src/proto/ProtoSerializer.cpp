#include <iostream>
#include <fstream>
#include "ProtoSerializer.h"
#include "SerializationSettings.h"
#include "TransportDatabase.h"
#include "TransportCatalog/TransportCatalogProtoMapper.h"
#include "YellowPages/ProtoMapper.h"
#include "YellowPages/YellowPagesDatabase.h"
#include "database.pb.h"

using namespace Serialization;

void Serialization::ProtoSerializer::Serialize(const SerializationSettings &settings,
                                               const TransportDatabase &transportDb,
                                               const YellowPages::BLL::YellowPagesDatabase &yellowPagesDb)
{
    std::ofstream out(settings.file, std::ios::out | std::ios::binary);

    {
        auto catalog = TransportCatalogProtoMapper::Map(transportDb);
        bool isSuccess = catalog.SerializeToOstream(&out);
        if (!isSuccess)
        {
            std::cerr << "ProtoSerializer::Serialize() catalog.SerializeToOstream failed!" << std::endl;
        }
    }
    {
        auto yellowPages = YellowPages::ProtoMapper::Map(yellowPagesDb);
        bool isSuccess = yellowPages.SerializeToOstream(&out);
        if (!isSuccess)
        {
            std::cerr << "ProtoSerializer::Serialize() yellowPages.SerializeToOstream failed!" << std::endl;
        }
    }
}

std::pair<TransportDatabaseUnp, YellowPages::BLL::YellowPagesDatabaseUnp> Serialization::ProtoSerializer::Deserialize(
    const SerializationSettings &settings)
{
    std::ifstream in(settings.file, std::ios::in | std::ios::binary);
    TransportCatalog catalog;
    {
        bool isSuccess = catalog.ParseFromIstream(&in);
        if (!isSuccess)
        {
            std::cerr << "ProtoSerializer::Deserialize() catalog.ParseFromIstream failed!" << std::endl;
        }
    }

    YellowPages::Database yellowPagesDb;
    {
        bool isSuccess = yellowPagesDb.ParseFromIstream(&in);
        if (!isSuccess)
        {
            std::cerr << "ProtoSerializer::Deserialize() yellowPagesDb.ParseFromIstream failed!" << std::endl;
        }
    }

    return std::make_pair(std::make_unique<TransportDatabase>(TransportCatalogProtoMapper::Map(catalog)),
                          std::make_unique<YellowPages::BLL::YellowPagesDatabase>(YellowPages::ProtoMapper::Map(yellowPagesDb)));
}
