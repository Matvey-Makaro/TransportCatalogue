#include <algorithm>
#include <cassert>
#include "YellowPagesDatabase.h"

using namespace YellowPages::BLL;

YellowPages::BLL::YellowPagesDatabase::YellowPagesDatabase(std::vector<Company> companies, 
    std::unordered_map<Rubric::IdType, Rubric> rubrics) :
    _companies(std::move(companies)),
    _rubrics(std::move(rubrics))
{}

const std::vector<Company> &YellowPages::BLL::YellowPagesDatabase::GetCompanies() const
{
    return _companies;
}

const std::unordered_map<Rubric::IdType, Rubric> &YellowPages::BLL::YellowPagesDatabase::GetRubrics() const
{
    return _rubrics;
}

std::vector<const Company *> YellowPages::BLL::YellowPagesDatabase::FindCompanies(
    const CompanyRestrictions &companyRestrictions) const
{
    std::vector<const Company*> foundCompanies(_companies.size(), nullptr);
    for (size_t i = 0; i < _companies.size(); i++)
    {
        foundCompanies[i] = &_companies[i];
    }

    FilterByNames(foundCompanies, companyRestrictions);
    FilterByUrls(foundCompanies, companyRestrictions);
    FilterByRubrics(foundCompanies, companyRestrictions);
    FilterByPhoneTemplates(foundCompanies, companyRestrictions);
    foundCompanies.shrink_to_fit();
    return foundCompanies;
}

void YellowPages::BLL::YellowPagesDatabase::FilterByNames(std::vector<const Company*>& foundCompanies,
    const CompanyRestrictions& companyRestrictions) const
{
    if (companyRestrictions.names.empty())
    {
        return;
    }
    auto filterFunc = [&companyRestrictions](const Company* company)
        {
            for (const auto& name : company->names)
            {
                if (companyRestrictions.names.count(name.value))
                {
                    return false;
                }
            }
            return true;
        };
    auto it = std::remove_if(begin(foundCompanies), end(foundCompanies), filterFunc);
    foundCompanies.erase(it, end(foundCompanies));
}

void YellowPages::BLL::YellowPagesDatabase::FilterByUrls(std::vector<const Company*>& foundCompanies,
    const CompanyRestrictions& companyRestrictions) const
{
    if (companyRestrictions.urls.empty())
    {
        return;
    }
    auto filterFunc = [&companyRestrictions](const Company* company)
        {
            for (const auto& url : company->urls)
            {
                if (companyRestrictions.urls.count(url.value))
                {
                    return false;
                }
            }
            return true;
        };
    auto it = std::remove_if(begin(foundCompanies), end(foundCompanies), filterFunc);
    foundCompanies.erase(it, end(foundCompanies));
}

void YellowPages::BLL::YellowPagesDatabase::FilterByRubrics(std::vector<const Company*>& foundCompanies,
    const CompanyRestrictions& companyRestrictions) const
{
    if (companyRestrictions.rubrics.empty())
    {
        return;
    }
    auto filterFunc = [this, &companyRestrictions](const Company* company)
        {
            for (const auto& rubricId : company->rubrics)
            {
                auto rubricIt = _rubrics.find(rubricId);
                assert(rubricIt != end(_rubrics));
                const auto& rubric = rubricIt->second;
                if(companyRestrictions.rubrics.count(rubric.name))
                {
                    return false;
                }
                for (const auto& rubricName : rubric.keywords)
                {
                    if (companyRestrictions.rubrics.count(rubricName))
                    {
                        return false;
                    }

                }
            }
            return true;
        };
    auto it = std::remove_if(begin(foundCompanies), end(foundCompanies), filterFunc);
    foundCompanies.erase(it, end(foundCompanies));
}

void YellowPages::BLL::YellowPagesDatabase::FilterByPhoneTemplates(std::vector<const Company*>& foundCompanies,
    const CompanyRestrictions& companyRestrictions) const
{
    if (companyRestrictions.phoneTemplates.empty())
    {
        return;
    }
    auto filterFunc = [this, &companyRestrictions](const Company* company)
        {
            for (const auto& phone : company->phones)
            {
                for(const auto& phoneTemplate : companyRestrictions.phoneTemplates)
                {
                    if(IsPhoneMatch(phone, phoneTemplate))
                    {
                        return false;
                    }
                }
            }
            return true;
        };
    auto it = std::remove_if(begin(foundCompanies), end(foundCompanies), filterFunc);
    foundCompanies.erase(it, end(foundCompanies));
}

bool YellowPages::BLL::YellowPagesDatabase::IsPhoneMatch(const Phone& phone,
    const PhoneTemplate& phoneTemplate) const
{
    if (phoneTemplate.type.has_value() && phone.type != *phoneTemplate.type)
    {
        return false;
    }
    if (!phoneTemplate.extension.empty() && phone.extension != phoneTemplate.extension)
    {
        return false;
    }
    if (!phoneTemplate.countryCode.empty() && phone.countryCode != phoneTemplate.countryCode)
    {
        return false;
    }
    if ((!phoneTemplate.countryCode.empty() || !phoneTemplate.localCode.empty()) &&
        phone.localCode != phoneTemplate.localCode)
    {
        return false;
    }
    return phone.number == phoneTemplate.number;
}
