#pragma once

#include <string>
#include "Json.h"

struct SerializationSettings
{
    std::string file;

    static SerializationSettings ParseFrom(const Json::Dict &attrs);
};