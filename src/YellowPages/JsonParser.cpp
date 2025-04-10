#include <iostream>
#include <cassert>
#include <optional>
#include <string_view>
#include "JsonParser.h"

using namespace YellowPages::BLL;

namespace
{
    const Json::Node *GetNodeByName(const Json::Dict &dict, const std::string &name)
    {
        auto it = dict.find(name);
        if (it == cend(dict))
        {
            return nullptr;
        }
        return &it->second;
    }
}

JsonParser::RubricMap JsonParser::ParseRubrics(const Json::Dict &yellowPages)
{
    const auto &rubricsJson = yellowPages.at("rubrics").AsMap();
    RubricMap allRubrics;
    for (const auto &[idStr, rubric] : rubricsJson)
    {
        Rubric::IdType id = std::stoull(idStr);
        allRubrics[id] = ParseRubric(rubric.AsMap());
    }
    return allRubrics;
}

std::vector<Company> YellowPages::BLL::JsonParser::ParseCompanies(const Json::Dict &yellowPages)
{
    const auto &companiesJson = yellowPages.at("companies").AsArray();
    std::vector<Company> companies;
    companies.reserve(companiesJson.size());
    for (const auto &c : companiesJson)
    {
        companies.emplace_back(ParseCompany(c.AsMap()));
    }
    return companies;
}

Rubric YellowPages::BLL::JsonParser::ParseRubric(const Json::Dict &json)
{
    Rubric rubric;
    rubric.name = json.at("name").AsString();
    auto keywordsIt = json.find("keywords");
    if (keywordsIt != end(json))
    {
        const auto &keywordsJson = keywordsIt->second.AsArray();
        rubric.keywords.reserve(keywordsJson.size());
        for (const auto &k : keywordsJson)
        {
            rubric.keywords.emplace_back(k.AsString());
        }
    }
    return rubric;
}

Company YellowPages::BLL::JsonParser::ParseCompany(const Json::Dict &json)
{
    Company company;
    const auto* addressNode = GetNodeByName(json, "address");
    if(addressNode)
    {
        company.address = ParseAddress(addressNode->AsMap());
    }
    const auto* namesNode = GetNodeByName(json, "names");
    if(namesNode)
    {
        company.names = ParseNames(namesNode->AsArray());
    }
    const auto* phonesNode = GetNodeByName(json, "phones");
    if(phonesNode)
    {
        company.phones = ParsePhones(phonesNode->AsArray());
    }
    const auto* urlsNode = GetNodeByName(json, "urls");
    if(urlsNode)
    {
        company.urls = ParseUrls(urlsNode->AsArray());
    }
    const auto* rubricsNode = GetNodeByName(json, "rubrics");
    if(rubricsNode)
    {
        company.rubrics = ParseRubricIds(rubricsNode->AsArray());
    }
    const auto* workingTimeNode = GetNodeByName(json, "working_time");
    if(workingTimeNode)
    {
        company.workingTime = ParseWorkingTime(workingTimeNode->AsMap());
    }
    const auto* nearbyStopsNode = GetNodeByName(json, "nearby_stops");
    if(nearbyStopsNode)
    {
        company.nearbyStops = ParseNearbyStops(nearbyStopsNode->AsArray());
    }
    return company;
}

Address YellowPages::BLL::JsonParser::ParseAddress(const Json::Dict &json)
{
    Address address;
    const auto* formattedNode = GetNodeByName(json, "formatted");
    if(formattedNode)
    {
        address.formatted = formattedNode->AsString();
    }
    const auto* addressComponents = GetNodeByName(json, "components");
    if(addressComponents)
    {
        address.components = ParseAddressComponentVec(addressComponents->AsArray());
    }
    const auto* coordsNode = GetNodeByName(json, "coords");
    if(coordsNode)
    {
        address.coords = ParseCoords(coordsNode->AsMap());
    }
    const auto* commentNode = GetNodeByName(json, "comment");
    if(commentNode)
    {
        address.comment = commentNode->AsString();
    }
    return address;
}

std::vector<Name> YellowPages::BLL::JsonParser::ParseNames(const Json::Arr &json)
{
    std::vector<Name> names;
    names.reserve(json.size());
    for(const auto& node : json)
    {
        names.emplace_back(ParseName(node.AsMap()));
    }
    return names;
}

std::vector<Phone> YellowPages::BLL::JsonParser::ParsePhones(const Json::Arr &json)
{
    std::vector<Phone> phones;
    phones.reserve(json.size());
    for(const auto& node : json)
    {
        phones.emplace_back(ParsePhone(node.AsMap()));
    }
    return phones;
}

std::vector<Url> YellowPages::BLL::JsonParser::ParseUrls(const Json::Arr &json)
{
    std::vector<Url> urls;
    urls.reserve(json.size());
    for(const auto& node : json)
    {
        urls.emplace_back(ParseUrl(node.AsMap()));
    }
    return urls;
}

std::vector<Rubric::IdType> YellowPages::BLL::JsonParser::ParseRubricIds(const Json::Arr &json)
{
    std::vector<Rubric::IdType> rubricIds;
    rubricIds.reserve(json.size());
    for(const auto& idNode : json)
    {
        rubricIds.emplace_back(idNode.AsInt());
    }
    return rubricIds;
}

WorkingTime YellowPages::BLL::JsonParser::ParseWorkingTime(const Json::Dict &json)
{
    WorkingTime workingTime;
    const auto* formattedNode = GetNodeByName(json, "formatted");
    if(formattedNode)
    {
        workingTime.formatted = formattedNode->AsString();
    }
    const auto* intervalsNode = GetNodeByName(json, "intervals");
    if(intervalsNode)
    {
        workingTime.intervals = ParseWorkingTimeIntervalVec(intervalsNode->AsArray());
    }
    return workingTime;
}

std::vector<NearbyStop> YellowPages::BLL::JsonParser::ParseNearbyStops(const Json::Arr &json)
{
    std::vector<NearbyStop> nearbyStops;
    nearbyStops.reserve(json.size());
    for(const auto& stopNode : json)
    {
        nearbyStops.emplace_back(ParseNearbyStop(stopNode.AsMap()));
    }
    return nearbyStops;
}

std::vector<AddressComponent> YellowPages::BLL::JsonParser::ParseAddressComponentVec(const Json::Arr &json)
{
    std::vector<AddressComponent> components;
    components.reserve(json.size());
    for(const auto& componentJson : json)
    {
        components.emplace_back(ParseAddressComponent(componentJson.AsMap()));
    }
    return components;
}

AddressComponent YellowPages::BLL::JsonParser::ParseAddressComponent(const Json::Dict &json)
{
    AddressComponent addressComponent;
    const auto* valueNode = GetNodeByName(json, "value");
    if(valueNode)
    {
        addressComponent.value = valueNode->AsString();
    }
    const auto* typeNode = GetNodeByName(json, "type");
    if(typeNode)
    {
        addressComponent.type = AddressComponent::NameToType(typeNode->AsString());
    }
    return addressComponent;
}

Name YellowPages::BLL::JsonParser::ParseName(const Json::Dict &json)
{
    Name name;
    name.type = Name::Main;
    const Json::Node *typeNode = GetNodeByName(json, "type");
    if (typeNode)
    {
        name.type = Name::NameToType(typeNode->AsString());
    }
    const Json::Node *valueNode = GetNodeByName(json, "value");
    if (valueNode)
    {
        name.value = valueNode->AsString();
    }
    return name;
}

Phone YellowPages::BLL::JsonParser::ParsePhone(const Json::Dict &json)
{
    Phone phone;
    phone.type = Phone::PhoneNum;
    const auto *formattedNode = GetNodeByName(json, "formatted");
    if (formattedNode)
    {
        phone.formatted = formattedNode->AsString();
    }
    const auto *typeNode = GetNodeByName(json, "type");
    if (typeNode)
    {
        phone.type = Phone::NameToType(typeNode->AsString());
    }
    const auto *countryCodeNode = GetNodeByName(json, "country_code");
    if (countryCodeNode)
    {
        phone.countryCode = countryCodeNode->AsString();
    }
    const auto *localCodeNode = GetNodeByName(json, "local_code");
    if (localCodeNode)
    {
        phone.localCode = localCodeNode->AsString();
    }
    const auto *numberNode = GetNodeByName(json, "number");
    if (numberNode)
    {
        phone.number = numberNode->AsString();
    }
    const auto *extensionNode = GetNodeByName(json, "extension");
    if (extensionNode)
    {
        phone.extension = extensionNode->AsString();
    }
    const auto *descriptionNode = GetNodeByName(json, "description");
    if (descriptionNode)
    {
        phone.description = descriptionNode->AsString();
    }
    return phone;
}

Url YellowPages::BLL::JsonParser::ParseUrl(const Json::Dict &json)
{
    const auto* valueNode = GetNodeByName(json, "value");
    if(valueNode)
    {
        return Url{valueNode->AsString()};
    }
    return Url{};
}

std::vector<WorkingTimeInterval> YellowPages::BLL::JsonParser::ParseWorkingTimeIntervalVec(const Json::Arr &jsonArr)
{
    std::vector<WorkingTimeInterval> intervals;
    intervals.reserve(jsonArr.size());
    for(const auto& intervalNode : jsonArr)
    {
        intervals.emplace_back(ParseWorkingTimeInterval(intervalNode.AsMap()));
    }
    return intervals;
}

WorkingTimeInterval YellowPages::BLL::JsonParser::ParseWorkingTimeInterval(const Json::Dict &json)
{
    WorkingTimeInterval interval;
    const auto* dayNode = GetNodeByName(json, "day");
    if(dayNode)
    {
        interval.day = WorkingTimeInterval::NameToDay(dayNode->AsString());
    }
    const auto* minutesFromNode = GetNodeByName(json, "minutes_from");
    if(minutesFromNode)
    {
        interval.minutesFrom = minutesFromNode->AsInt();
    }
    const auto* minutesToNode = GetNodeByName(json, "minutes_to");
    if(minutesToNode)
    {
        interval.minutesTo = minutesToNode->AsInt();
    }
    return interval;
}

NearbyStop YellowPages::BLL::JsonParser::ParseNearbyStop(const Json::Dict &json)
{
    NearbyStop nearbyStop;
    const auto* nameNode = GetNodeByName(json, "name");
    if(nameNode)
    {
        nearbyStop.name = nameNode->AsString();
    }
    const auto* metersNode = GetNodeByName(json, "meters");
    if(metersNode)
    {
        nearbyStop.meters = metersNode->AsInt();
    }
    return nearbyStop;
}

Sphere::Point YellowPages::BLL::JsonParser::ParseCoords(const Json::Dict &json)
{
    return Sphere::Point {
        .latitude = json.at("lat").AsDouble(),
        .longitude = json.at("lon").AsDouble()
    };
}
