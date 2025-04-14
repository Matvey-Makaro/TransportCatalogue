#pragma once

#include <memory>
#include <iostream>
#include <tuple>

namespace YellowPages::BLL
{
    class YellowPagesDatabase;
    using YellowPagesDatabaseUnp = std::unique_ptr<YellowPagesDatabase>;
}

class TransportDatabase;
using TransportDatabaseUnp = std::unique_ptr<TransportDatabase>;
class SerializationSettings;

namespace Serialization
{
    class ProtoSerializer
    {
    public:
        ProtoSerializer() = delete;
        ~ProtoSerializer() = default;

        static void Serialize(const SerializationSettings &settings, 
            const TransportDatabase &transportDb,
            const YellowPages::BLL::YellowPagesDatabase &yellowPagesDb);

        static std::pair<TransportDatabaseUnp, YellowPages::BLL::YellowPagesDatabaseUnp> Deserialize(const SerializationSettings &settings);
    };
}