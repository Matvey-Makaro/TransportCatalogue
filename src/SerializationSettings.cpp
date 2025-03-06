#include "SerializationSettings.h"

SerializationSettings SerializationSettings::ParseFrom(const Json::Dict& attrs)
{
    return SerializationSettings {
        .file = attrs.at("file").AsString()
    };
}
