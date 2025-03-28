#include <iostream>
#include <fstream>
#include "ProtoSerializer.h"
#include "SerializationSettings.h"
#include "TransportDatabase.h"
#include "TransportCatalogProtoMapper.h"

using namespace Serialization;

void Serialization::ProtoSerializer::Serialize(const SerializationSettings& settings, const TransportDatabase& db)
{
    std::ofstream out(settings.file, std::ios::out | std::ios::binary);
    auto catalog = TransportCatalogProtoMapper::Map(db);
    bool isSuccess = catalog.SerializeToOstream(&out);
    if(!isSuccess)
    {
        std::cerr << "ProtoSerializer::Serialize() catalog.SerializeToOstream failed!" << std::endl;
    }
}

TransportDatabase Serialization::ProtoSerializer::Deserialize(const SerializationSettings& settings)
{
    std::ifstream in(settings.file, std::ios::in | std::ios::binary);
    TransportCatalog catalog;
    bool isSuccess = catalog.ParseFromIstream(&in);
    if(!isSuccess)
    {
        std::cerr << "ProtoSerializer::Deserialize() catalog.ParseFromIstream failed!" << std::endl;
    }
    return TransportCatalogProtoMapper::Map(catalog);
}
