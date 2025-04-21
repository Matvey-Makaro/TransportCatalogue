#include <algorithm>
#include <cassert>
#include "YellowPagesDatabase.h"
#include "Utils.h"
#include "Json.h"
#include "ProgramState.h"

using namespace YellowPages::BLL;

YellowPages::BLL::YellowPagesDatabase::YellowPagesDatabase(std::vector<Company> companies,
    std::unordered_map<Rubric::IdType, Rubric> rubrics) :
    _companies(std::move(companies)),
    _rubrics(std::move(rubrics))
{
    REGISTER_CURR_FUNC();
}

const std::vector<Company>& YellowPages::BLL::YellowPagesDatabase::GetCompanies() const
{
    REGISTER_CURR_FUNC();
    return _companies;
}

const std::vector<const Company*> YellowPages::BLL::YellowPagesDatabase::GetCompaniesPtr() const
{
    REGISTER_CURR_FUNC();
    std::vector<const Company*> companiesPtr(_companies.size(), nullptr);
    for (size_t i = 0; i < _companies.size(); i++)
    {
        companiesPtr[i] = &_companies[i];
    }
    return companiesPtr;
}

const std::unordered_map<Rubric::IdType, Rubric>& YellowPages::BLL::YellowPagesDatabase::GetRubrics() const
{
    REGISTER_CURR_FUNC();
    return _rubrics;
}

std::vector<const Company*> YellowPages::BLL::YellowPagesDatabase::FindCompanies(
    const CompanyRestrictions& companyRestrictions) const
{
    REGISTER_CURR_FUNC();
    auto foundCompanies = GetCompaniesPtr();
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
    REGISTER_CURR_FUNC();
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
    REGISTER_CURR_FUNC();
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
    REGISTER_CURR_FUNC();
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
                if (companyRestrictions.rubrics.count(rubric.name))
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
    REGISTER_CURR_FUNC();
    if (companyRestrictions.phoneTemplates.empty())
    {
        return;
    }
    auto filterFunc = [this, &companyRestrictions](const Company* company)
        {
            for (const auto& phone : company->phones)
            {
                for (const auto& phoneTemplate : companyRestrictions.phoneTemplates)
                {
                    if (IsPhoneMatch(phone, phoneTemplate))
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
    REGISTER_CURR_FUNC();
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

PhoneTemplate YellowPages::BLL::PhoneTemplate::FromJson(const Json::Dict& attrs)
{
    REGISTER_CURR_FUNC();
    using namespace YellowPages::BLL;
    PhoneTemplate phoneTemplate;
    const auto* typeNode = GetNodeByName(attrs, "type");
    if (typeNode != nullptr)
    {
        phoneTemplate.type = Phone::NameToType(typeNode->AsString());
    }
    const auto* countryCodeNode = GetNodeByName(attrs, "country_code");
    if (countryCodeNode != nullptr)
    {
        phoneTemplate.countryCode = countryCodeNode->AsString();
    }
    const auto* localCodeNode = GetNodeByName(attrs, "local_code");
    if (localCodeNode != nullptr)
    {
        phoneTemplate.localCode = localCodeNode->AsString();
    }
    const auto* numberNode = GetNodeByName(attrs, "number");
    if (numberNode != nullptr)
    {
        phoneTemplate.number = numberNode->AsString();
    }
    const auto* extensionNode = GetNodeByName(attrs, "extension");
    if (extensionNode != nullptr)
    {
        phoneTemplate.extension = extensionNode->AsString();
    }
    return phoneTemplate;
}

CompanyRestrictions YellowPages::BLL::CompanyRestrictions::FromJson(const Json::Dict& attrs)
{
    REGISTER_CURR_FUNC();
    YellowPages::BLL::CompanyRestrictions restrictions;
    const auto* namesNode = GetNodeByName(attrs, "names");
    if (namesNode != nullptr)
    {
        for (const auto& name : namesNode->AsArray())
        {
            restrictions.names.insert(name.AsString());
        }
    }
    const auto* urlsNode = GetNodeByName(attrs, "urls");
    if (urlsNode != nullptr)
    {
        for (const auto& url : urlsNode->AsArray())
        {
            restrictions.urls.insert(url.AsString());
        }
    }
    const auto* rubricsNode = GetNodeByName(attrs, "rubrics");
    if (rubricsNode != nullptr)
    {
        for (const auto& rubric : rubricsNode->AsArray())
        {
            restrictions.rubrics.insert(rubric.AsString());
        }
    }
    const auto* phoneTemplatesNode = GetNodeByName(attrs, "phones");
    if (phoneTemplatesNode != nullptr)
    {
        for (const auto& phoneTemplate : phoneTemplatesNode->AsArray())
        {
            restrictions.phoneTemplates.push_back(PhoneTemplate::FromJson(phoneTemplate.AsMap()));
        }
    }
    return restrictions;
}

const Company* YellowPages::BLL::YellowPagesDatabase::GetCompanyByMainName(std::string_view mainName) const
{
    REGISTER_CURR_FUNC();
    auto it = std::find_if(begin(_companies), end(_companies), [mainName](const Company& company)
        {
            return company.GetMainName().value == mainName;
        });
    if (it == end(_companies))
    {
        return nullptr;
    }
    return &(*it);
}

const Rubric* YellowPages::BLL::YellowPagesDatabase::GetRubricById(Rubric::IdType id) const
{
    REGISTER_CURR_FUNC();
    auto it = _rubrics.find(id);
    if (it == _rubrics.end())
    {
        return nullptr;
    }
    return &it->second;
}
