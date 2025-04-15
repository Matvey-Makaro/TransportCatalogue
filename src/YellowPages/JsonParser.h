#pragma once

#include <unordered_map>

#include "Json.h"
#include "Rubric.h"
#include "Company.h"
#include "YellowPagesDatabase.h"

namespace YellowPages
{
    namespace BLL
    {
        class JsonParser
        {
        public:
            static YellowPagesDatabase ParseYellowPages(const Json::Dict &yellowPages);

            using RubricMap = std::unordered_map<Rubric::IdType, Rubric>;
            static RubricMap ParseRubrics(const Json::Dict &yellowPages);
            static std::vector<Company> ParseCompanies(const Json::Dict &yellowPages);

        private:
            static Rubric ParseRubric(const Json::Dict& json);
            static Company ParseCompany(const Json::Dict& json);

            static Address ParseAddress(const Json::Dict& json);
            static std::vector<Name> ParseNames(const Json::Arr& json);
            static std::vector<Phone> ParsePhones(const Json::Arr& json);
            static std::vector<Url> ParseUrls(const Json::Arr& json);
            static std::vector<Rubric::IdType> ParseRubricIds(const Json::Arr& json);
            static WorkingTime ParseWorkingTime(const Json::Dict& json);
            static std::vector<NearbyStop> ParseNearbyStops(const Json::Arr& json);

            static std::vector<AddressComponent> ParseAddressComponentVec(const Json::Arr& json);
            static AddressComponent ParseAddressComponent(const Json::Dict& json);
            static Name ParseName(const Json::Dict& json);
            static Phone ParsePhone(const Json::Dict& json);
            static Url ParseUrl(const Json::Dict& json);

            static std::vector<WorkingTimeInterval> ParseWorkingTimeIntervalVec(const Json::Arr& json);
            static WorkingTimeInterval ParseWorkingTimeInterval(const Json::Dict& json);
            static NearbyStop ParseNearbyStop(const Json::Dict& json);
            static Sphere::Point ParseCoords(const Json::Dict& json);
        };
    }
}