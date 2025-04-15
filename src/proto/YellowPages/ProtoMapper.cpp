#include "ProtoMapper.h"

#include "database.pb.h"
#include "company.pb.h"
#include "rubric.pb.h"
#include "sphere.pb.h"

#include "YellowPages/YellowPagesDatabase.h"
#include "YellowPages/Company.h"
#include "YellowPages/Rubric.h"
#include "Sphere.h"


YellowPages::Database YellowPages::ProtoMapper::Map(const BLL::YellowPagesDatabase &db)
{
    YellowPages::Database pbDatabase;
    for (const auto &company : db.GetCompanies())
    {
        *pbDatabase.add_companies() = Map(company);
    }
    for(const auto& [id, rubric] : db.GetRubrics())
    {
        pbDatabase.mutable_rubrics()->insert({id, Map(rubric)});
    }
    return pbDatabase;
}

YellowPages::BLL::YellowPagesDatabase YellowPages::ProtoMapper::Map(const Database &pbDb)
{
    std::vector<YellowPages::BLL::Company> companies;
    companies.reserve(pbDb.companies_size());
    for(const auto& company : pbDb.companies())
    {
        companies.emplace_back(Map(company));
    }
    std::unordered_map<BLL::Rubric::IdType, BLL::Rubric> rubrics;
    rubrics.reserve(pbDb.rubrics_size());
    for(const auto& rubric : pbDb.rubrics())
    {
        rubrics.emplace(rubric.first, Map(rubric.second));
    }
    return BLL::YellowPagesDatabase(std::move(companies), std::move(rubrics));
}

YellowPages::Rubric YellowPages::ProtoMapper::Map(const BLL::Rubric &rubric)
{
    YellowPages::Rubric pbRubric;
    pbRubric.set_name(rubric.name);
    for (const auto &w : rubric.keywords)
    {
        pbRubric.add_keywords(w);
    }
    return pbRubric;
}

YellowPages::BLL::Rubric YellowPages::ProtoMapper::Map(const Rubric &pbRubric)
{
    YellowPages::BLL::Rubric rubric;
    rubric.name = pbRubric.name();
    rubric.keywords.reserve(pbRubric.keywords_size());
    for (const auto &w : pbRubric.keywords())
    {
        rubric.keywords.emplace_back(w);
    }
    return rubric;
}

YellowPages::Company YellowPages::ProtoMapper::Map(const BLL::Company &company)
{
    YellowPages::Company pbCompany;
    *pbCompany.mutable_address() = Map(company.address);
    for (const auto &name : company.names)
    {
        *pbCompany.add_names() = Map(name);
    }
    for (const auto &phone : company.phones)
    {
        *pbCompany.add_phones() = Map(phone);
    }
    for (const auto &url : company.urls)
    {
        *pbCompany.add_urls() = Map(url);
    }
    for (const auto &rubric : company.rubrics)
    {
        pbCompany.add_rubrics(rubric);
    }
    *pbCompany.mutable_working_time() = Map(company.workingTime);
    for (const auto &nearbyStop : company.nearbyStops)
    {
        *pbCompany.add_nearby_stops() = Map(nearbyStop);
    }
    return pbCompany;
}

YellowPages::BLL::Company YellowPages::ProtoMapper::Map(const Company &pbCompany)
{
    YellowPages::BLL::Company company;
    company.address = Map(pbCompany.address());
    company.names.reserve(pbCompany.names_size());
    for (const auto &name : pbCompany.names())
    {
        company.names.emplace_back(Map(name));
    }
    company.phones.reserve(pbCompany.phones_size());
    for (const auto &phone : pbCompany.phones())
    {
        company.phones.emplace_back(Map(phone));
    }
    company.urls.reserve(pbCompany.urls_size());
    for (const auto &url : pbCompany.urls())
    {
        company.urls.emplace_back(Map(url));
    }
    company.rubrics.reserve(pbCompany.rubrics_size());
    for (const auto &rubric : pbCompany.rubrics())
    {
        company.rubrics.emplace_back(rubric);
    }
    company.workingTime = Map(pbCompany.working_time());
    company.nearbyStops.reserve(pbCompany.nearby_stops_size());
    for (const auto &nearbyStop : pbCompany.nearby_stops())
    {
        company.nearbyStops.emplace_back(Map(nearbyStop));
    }
    return company;
}

YellowPages::Address YellowPages::ProtoMapper::Map(const BLL::Address &address)
{
    YellowPages::Address pbAddress;
    pbAddress.set_formatted(address.formatted);
    for (const auto &component : address.components)
    {
        *pbAddress.add_components() = Map(component);
    }
    *pbAddress.mutable_coords() = Map(address.coords);
    pbAddress.set_comment(address.comment);
    return pbAddress;
}

YellowPages::BLL::Address YellowPages::ProtoMapper::Map(const Address &pbAddress)
{
    YellowPages::BLL::Address address;
    address.formatted = pbAddress.formatted();
    address.components.reserve(pbAddress.components_size());
    for (const auto &component : pbAddress.components())
    {
        address.components.emplace_back(Map(component));
    }
    address.coords = Map(pbAddress.coords());
    address.comment = pbAddress.comment();
    return address;
}

YellowPages::Name YellowPages::ProtoMapper::Map(const BLL::Name &name)
{
    YellowPages::Name pbName;
    pbName.set_value(name.value);
    pbName.set_type(static_cast<Name_Type>(name.type));
    return pbName;
}

YellowPages::BLL::Name YellowPages::ProtoMapper::Map(const Name &pbName)
{
    YellowPages::BLL::Name name;
    name.value = pbName.value();
    name.type = static_cast<YellowPages::BLL::Name::Type>(pbName.type());
    return name;
}

YellowPages::Phone YellowPages::ProtoMapper::Map(const BLL::Phone &phone)
{
    YellowPages::Phone pbPhone;
    pbPhone.set_formatted(phone.formatted);
    pbPhone.set_type(static_cast<Phone_Type>(phone.type));
    pbPhone.set_country_code(phone.countryCode);
    pbPhone.set_local_code(phone.localCode);
    pbPhone.set_number(phone.number);
    pbPhone.set_extension(phone.extension);
    pbPhone.set_description(phone.description);
    return pbPhone;
}

YellowPages::BLL::Phone YellowPages::ProtoMapper::Map(const Phone &pbPhone)
{
    YellowPages::BLL::Phone phone;
    phone.formatted = pbPhone.formatted();
    phone.type = static_cast<YellowPages::BLL::Phone::Type>(pbPhone.type());
    phone.countryCode = pbPhone.country_code();
    phone.localCode = pbPhone.local_code();
    phone.number = pbPhone.number();
    phone.extension = pbPhone.extension();
    phone.description = pbPhone.description();
    return phone;
}

YellowPages::Url YellowPages::ProtoMapper::Map(const BLL::Url &url)
{
    YellowPages::Url pbUrl;
    pbUrl.set_value(url.value);
    return pbUrl;
}

YellowPages::BLL::Url YellowPages::ProtoMapper::Map(const Url &pbUrl)
{
    YellowPages::BLL::Url url;
    url.value = pbUrl.value();
    return url;
}

YellowPages::WorkingTime YellowPages::ProtoMapper::Map(const BLL::WorkingTime &workingTime)
{
    YellowPages::WorkingTime pbWorkingTime;
    pbWorkingTime.set_formatted(workingTime.formatted);
    for (const auto &interval : workingTime.intervals)
    {
        *pbWorkingTime.add_intervals() = Map(interval);
    }
    return pbWorkingTime;
}

YellowPages::BLL::WorkingTime YellowPages::ProtoMapper::Map(const WorkingTime &pbWorkingTime)
{
    YellowPages::BLL::WorkingTime workingTime;
    workingTime.formatted = pbWorkingTime.formatted();
    workingTime.intervals.reserve(pbWorkingTime.intervals_size());
    for (const auto &interval : pbWorkingTime.intervals())
    {
        workingTime.intervals.emplace_back(Map(interval));
    }
    return workingTime;
}

YellowPages::NearbyStop YellowPages::ProtoMapper::Map(const BLL::NearbyStop &nearbyStop)
{
    YellowPages::NearbyStop pbNearbyStop;
    pbNearbyStop.set_name(nearbyStop.name);
    pbNearbyStop.set_meters(nearbyStop.meters);
    return pbNearbyStop;
}

YellowPages::BLL::NearbyStop YellowPages::ProtoMapper::Map(const NearbyStop &pbNearbyStop)
{
    YellowPages::BLL::NearbyStop nearbyStop;
    nearbyStop.name = pbNearbyStop.name();
    nearbyStop.meters = pbNearbyStop.meters();
    return nearbyStop;
}

YellowPages::AddressComponent YellowPages::ProtoMapper::Map(const BLL::AddressComponent &addressComponent)
{
    YellowPages::AddressComponent pbAddressComponent;
    pbAddressComponent.set_value(addressComponent.value);
    return pbAddressComponent;
}

YellowPages::BLL::AddressComponent YellowPages::ProtoMapper::Map(const AddressComponent &pbAddressComponent)
{
    YellowPages::BLL::AddressComponent addressComponent;
    addressComponent.value = pbAddressComponent.value();
    return addressComponent;
}

SphereProto::Coords YellowPages::ProtoMapper::Map(const Sphere::Point &point)
{
    SphereProto::Coords pbCoords;
    pbCoords.set_lat(point.latitude);
    pbCoords.set_lon(point.longitude);
    return pbCoords;
}

Sphere::Point YellowPages::ProtoMapper::Map(const SphereProto::Coords &pbCoords)
{
    Sphere::Point point;
    point.latitude = pbCoords.lat();
    point.longitude = pbCoords.lon();
    return point;
}

YellowPages::WorkingTimeInterval YellowPages::ProtoMapper::Map(const BLL::WorkingTimeInterval &workingTimeInterval)
{
    YellowPages::WorkingTimeInterval pbWorkingTimeInterval;
    pbWorkingTimeInterval.set_day(static_cast<WorkingTimeInterval_Day>(workingTimeInterval.day));
    pbWorkingTimeInterval.set_minutes_from(workingTimeInterval.minutesFrom);
    pbWorkingTimeInterval.set_minutes_to(workingTimeInterval.minutesTo);
    return pbWorkingTimeInterval;
}

YellowPages::BLL::WorkingTimeInterval YellowPages::ProtoMapper::Map(const WorkingTimeInterval &pbWorkingTimeInterval)
{
    YellowPages::BLL::WorkingTimeInterval workingTimeInterval;
    workingTimeInterval.day = static_cast<YellowPages::BLL::WorkingTimeInterval::Day>(pbWorkingTimeInterval.day());
    workingTimeInterval.minutesFrom = pbWorkingTimeInterval.minutes_from();
    workingTimeInterval.minutesTo = pbWorkingTimeInterval.minutes_to();
    return workingTimeInterval;
}