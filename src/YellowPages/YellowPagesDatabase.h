#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cstdint>
#include <optional>

namespace YellowPages
{
    namespace BLL
    {
        struct Rubric
        {
            using IdType = uint64_t;
            std::string name;
            std::vector<std::string> keywords;
        };

        struct Address
        {
            // TODO:
        };

        struct Name
        {
            enum Type
            {
                Main,
                Synonym,
                Short
            };
            std::string value;
            Type type;
        };

        struct Phone
        {
            enum Type
            {
                PhoneNum,
                Fax
            };
            std::string formatted;
            Type type;
            std::string countryCode;
            std::string localCode;
            std::string number;
            std::string extension;
            std::string description;
        };

        struct Url
        {
            std::string value;
        };

        struct WorkingTime
        {

        };

        struct NearbyStop
        {

        };

        struct Company
        {
            Address address;
            std::vector<Name> names;
            std::vector<Phone> phones;
            std::vector<Url> urls;
            std::vector<Rubric::IdType> rubrics;
            WorkingTime workingTime;
            std::vector<NearbyStop> nearbyStops;
        };

        struct PhoneTemplate
        {
            std::optional<Phone::Type> type;
            std::string countryCode;
            std::string localCode;
            std::string number;
            std::string extension;
        };

        struct CompanyRestrictions
        {
            std::unordered_set<std::string> names;
            std::unordered_set<std::string> urls;
            std::unordered_set<std::string> rubrics;
            std::vector<PhoneTemplate> phoneTemplates;

        };

        class YellowPagesDatabase
        {
        public:
            YellowPagesDatabase(std::vector<Company> companies, 
                std::unordered_map<Rubric::IdType, Rubric> rubrics);
            ~YellowPagesDatabase() = default;

            // TODO: Change to list?
            std::vector<const Company*> FindCompanies(const CompanyRestrictions& companyRestrictions) const;

        private:
            void FilterByNames(std::vector<const Company*>& foundCompanies, const CompanyRestrictions& companyRestrictions) const;
            void FilterByUrls(std::vector<const Company*>& foundCompanies, const CompanyRestrictions& companyRestrictions) const;
            void FilterByRubrics(std::vector<const Company*>& foundCompanies, const CompanyRestrictions& companyRestrictions) const;
            void FilterByPhoneTemplates(std::vector<const Company*>& foundCompanies, const CompanyRestrictions& companyRestrictions) const;

            bool IsPhoneMatch(const Phone& phone, const PhoneTemplate& phoneTemplate) const;

        private:
            std::vector<Company> _companies;
            std::unordered_map<Rubric::IdType, Rubric> _rubrics;
        };

    }

}
