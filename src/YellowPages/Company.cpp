#include <iostream>
#include <cassert>

#include "Company.h"
#include "Utils.h"


YellowPages::BLL::Name::Type YellowPages::BLL::Name::NameToType(std::string_view name)
{
    using namespace YellowPages::BLL;
    if (name == "MAIN")
    {
        return Type::Main;
    }
    else if (name == "SYNONYM")
    {
        return Type::Synonym;
    }
    else if (name == "SHORT")
    {
        return Type::Short;
    }
    else
    {
        std::cerr << __FILE__ << ' ' << __LINE__ << ": no Name::Type with name: " << name;
        assert(false);
    }
    return Name::Type();
}

YellowPages::BLL::Phone::Type YellowPages::BLL::Phone::NameToType(std::string_view name)
{
    using namespace YellowPages::BLL;
    if (name == "PHONE")
    {
        return Type::PhoneNum;
    }
    else if (name == "FAX")
    {
        return Type::Fax;
    }
    else
    {
        std::cerr << __FILE__ << ' ' << __LINE__ << ": no Phone::Type with name: " << name;
        assert(false);
    }
    return Type();
}

YellowPages::BLL::AddressComponent::Type YellowPages::BLL::AddressComponent::NameToType(std::string_view name)
{
    using namespace YellowPages::BLL;
    if (name == "COUNTRY")
    {
        return Type::Country;
    }
    else if (name == "REGION")
    {
        return Type::Region;
    }
    else if (name == "CITY")
    {
        return Type::City;
    }
    else if (name == "STREET")
    {
        return Type::Street;
    }
    else if (name == "HOUSE")
    {
        return Type::House;
    }
    else
    {
        std::cerr << __FILE__ << ' ' << __LINE__ << ": no AddressComponent::Type with name: " << name;
        assert(false);
    }
    return Type();
}

YellowPages::BLL::WorkingTimeInterval::Day YellowPages::BLL::WorkingTimeInterval::NameToDay(std::string_view name)
{
    using namespace YellowPages::BLL;
    if (name == "EVERYDAY")
    {
        return Day::Everyday;
    }
    else if (name == "MONDAY")
    {
        return Day::Monday;
    }
    else if (name == "TUESDAY")
    {
        return Day::Tuesday;
    }
    else if (name == "WEDNESDAY")
    {
        return Day::Wednesday;
    }
    else if (name == "THURSDAY")
    {
        return Day::Thursday;
    }
    else if (name == "FRIDAY")
    {
        return Day::Friday;
    }
    else if (name == "SATURDAY")
    {
        return Day::Saturday;
    }
    else if (name == "SUNDAY")
    {
        return Day::Sunday;
    }
    else
    {
        std::cerr << __FILE__ << ' ' << __LINE__ << ": no WorkingTimeInterval::Day with name: " << name;
        assert(false);
    }
    return Day();
}

bool YellowPages::BLL::operator==(const AddressComponent &lhs, const AddressComponent &rhs)
{
    return lhs.value == rhs.value && lhs.type == rhs.type;
}

bool YellowPages::BLL::operator==(const Address &lhs, const Address &rhs)
{
    return lhs.formatted == rhs.formatted &&
           std::equal(cbegin(lhs.components), cend(lhs.components), cbegin(rhs.components), cend(rhs.components)) &&
           IsEqual(lhs.coords, rhs.coords) &&
           lhs.comment == rhs.comment;
}

bool YellowPages::BLL::operator==(const Name &lhs, const Name &rhs)
{
    return lhs.value == rhs.value &&
           lhs.type == rhs.type;
}

bool YellowPages::BLL::operator==(const Phone &lhs, const Phone &rhs)
{
    return lhs.formatted == rhs.formatted &&
           lhs.type == rhs.type &&
           lhs.countryCode == rhs.countryCode &&
           lhs.localCode == rhs.localCode &&
           lhs.number == rhs.number &&
           lhs.extension == rhs.extension &&
           lhs.description == rhs.description;
}

bool YellowPages::BLL::operator==(const Url &lhs, const Url &rhs)
{
    return lhs.value == rhs.value;
}

bool YellowPages::BLL::operator==(const WorkingTimeInterval &lhs, const WorkingTimeInterval &rhs)
{
    return lhs.day == rhs.day &&
           lhs.minutesFrom == rhs.minutesFrom &&
           lhs.minutesTo == rhs.minutesTo;
}

bool YellowPages::BLL::operator==(const WorkingTime &lhs, const WorkingTime &rhs)
{
    return lhs.formatted == rhs.formatted &&
           std::equal(cbegin(lhs.intervals), cend(lhs.intervals), cbegin(rhs.intervals), cend(rhs.intervals));
}

bool YellowPages::BLL::operator==(const NearbyStop &lhs, const NearbyStop &rhs)
{
    return lhs.name == rhs.name && 
        lhs.meters == rhs.meters;
}

bool YellowPages::BLL::operator==(const Company &lhs, const Company &rhs)
{
    return lhs.address == rhs.address &&
    std::equal(cbegin(lhs.names), cend(lhs.names), cbegin(rhs.names), cend(rhs.names)) &&
    std::equal(cbegin(lhs.phones), cend(lhs.phones), cbegin(rhs.phones), cend(rhs.phones)) &&
    std::equal(cbegin(lhs.urls), cend(lhs.urls), cbegin(rhs.urls), cend(rhs.urls)) &&
    std::equal(cbegin(lhs.rubrics), cend(lhs.rubrics), cbegin(rhs.rubrics), cend(rhs.rubrics)) &&
    lhs.workingTime == rhs.workingTime &&
    std::equal(cbegin(lhs.nearbyStops), cend(lhs.nearbyStops), cbegin(rhs.nearbyStops), cend(rhs.nearbyStops));
}
