#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cstdint>
#include <optional>
#include <memory>

#include "Rubric.h"
#include "Company.h"
#include "Json.h"

namespace YellowPages
{
    namespace BLL
    {
        struct PhoneTemplate
        {
            std::optional<Phone::Type> type;
            std::string countryCode;
            std::string localCode;
            std::string number;
            std::string extension;

            static PhoneTemplate FromJson(const Json::Dict& attrs);
        };

        struct CompanyRestrictions
        {
            std::unordered_set<std::string> names;
            std::unordered_set<std::string> urls;
            std::unordered_set<std::string> rubrics;
            std::vector<PhoneTemplate> phoneTemplates;

            static CompanyRestrictions FromJson(const Json::Dict& attrs);
        };

        class YellowPagesDatabase
        {
        public:
            YellowPagesDatabase(std::vector<Company> companies, 
                std::unordered_map<Rubric::IdType, Rubric> rubrics);
            ~YellowPagesDatabase() = default;

            YellowPagesDatabase(const YellowPagesDatabase& other) = default; 
            YellowPagesDatabase& operator=(const YellowPagesDatabase& other) = default;
            YellowPagesDatabase(YellowPagesDatabase&& other) noexcept = default;
            YellowPagesDatabase& operator=(YellowPagesDatabase&& other) noexcept = default;

            const std::vector<Company>& GetCompanies() const;
            const std::unordered_map<Rubric::IdType, Rubric>& GetRubrics() const;

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
        using YellowPagesDatabaseShp = std::shared_ptr<YellowPagesDatabase>;
        using YellowPagesDatabaseUnp = std::unique_ptr<YellowPagesDatabase>;
    }

}
