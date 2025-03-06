#pragma once

class TransportDatabase;
class SerializationSettings;

namespace Serialization
{
    class ProtoSerializer
    {
    public:
        ProtoSerializer() = delete;
    
        static void Serialize(const SerializationSettings& settings, const TransportDatabase& db);
        static TransportDatabase Deserialize(const SerializationSettings& settings);
    };
}