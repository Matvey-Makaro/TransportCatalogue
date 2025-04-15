#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <optional>
#include <string_view>

#include "Rubric.h"
#include "Sphere.h"

namespace YellowPages
{
    namespace BLL
    {
        struct AddressComponent
        {
            enum Type
            {
                Country,
                Region,
                City,
                Street,
                House,
            };

            std::string value;
            Type type = Type::Country;

            static Type NameToType(std::string_view name);
        };
        bool operator==(const AddressComponent& lhs, const AddressComponent& rhs);

        struct Address
        {
            std::string formatted;
            std::vector<AddressComponent> components;
            Sphere::Point coords;
            std::string comment;
        };
        bool operator==(const Address& lhs, const Address& rhs);

        struct Name
        {
            enum Type
            {
                Main,
                Synonym,
                Short
            };
            std::string value;
            Type type = Type::Main;

            static Type NameToType(std::string_view name);
        };
        bool operator==(const Name& lhs, const Name& rhs);

        struct Phone
        {
            enum Type
            {
                PhoneNum,
                Fax
            };
            std::string formatted;
            Type type = Type::PhoneNum;
            std::string countryCode;
            std::string localCode;
            std::string number;
            std::string extension;
            std::string description;

            static Type NameToType(std::string_view name);
        };
        bool operator==(const Phone& lhs, const Phone& rhs);

        struct Url
        {
            std::string value;
        };
        bool operator==(const Url& lhs, const Url& rhs);

        struct WorkingTimeInterval
        {
            enum Day
            {
                Everyday,
                Monday,
                Tuesday,
                Wednesday,
                Thursday,
                Friday,
                Saturday,
                Sunday,
            };

            Day day = Day::Everyday;
            int32_t minutesFrom = 0;
            int32_t minutesTo = 0;

            static Day NameToDay(std::string_view name);
        };
        bool operator==(const WorkingTimeInterval& lhs, const WorkingTimeInterval& rhs);

        struct WorkingTime
        {
            std::string formatted;
            std::vector<WorkingTimeInterval> intervals;
        };
        bool operator==(const WorkingTime& lhs, const WorkingTime& rhs);

        struct NearbyStop
        {
            std::string name;
            uint32_t meters = 0;
        };
        bool operator==(const NearbyStop& lhs, const NearbyStop& rhs);

        struct Company
        {
            Address address;
            std::vector<Name> names;
            std::vector<Phone> phones;
            std::vector<Url> urls;
            std::vector<Rubric::IdType> rubrics;
            WorkingTime workingTime;
            std::vector<NearbyStop> nearbyStops;

            const Name& GetMainName() const;
        };
        bool operator==(const Company& lhs, const Company& rhs);
    }
}