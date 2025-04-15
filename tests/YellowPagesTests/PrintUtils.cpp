#include "PrintUtils.h"

using namespace YellowPages::BLL;

namespace
{
    void PrintTo(const std::vector<AddressComponent> &components, std::ostream *os)
    {
        *os << "{";
        for (const auto &component : components)
        {
            ::PrintTo(component, os);
            *os << ", ";
        }
        *os << "}";
    }

    void PrintTo(const std::vector<WorkingTimeInterval> &intervals, std::ostream *os)
    {
        *os << "{";
        for (const auto &interval : intervals)
        {
            PrintTo(interval, os);
            *os << ", ";
        }
        *os << "}";
    }

    std::ostream& operator<<(std::ostream &os, const std::vector<Name> &names)
    {
        for (const auto &name : names)
        {
            PrintTo(name, &os);
            os << ", ";
        }
        return os;
    }

    std::ostream& operator<<(std::ostream &os, const std::vector<Phone> &phones)
    {
        for (const auto &phone : phones)
        {
            PrintTo(phone, &os);
            os << ", ";
        }
        return os;
    }

    std::ostream& operator<<(std::ostream &os, const std::vector<Url> &urls)
    {
        for (const auto &url : urls)
        {
            PrintTo(url, &os);
            os << ", ";
        }
        return os;
    }

    std::ostream& operator<<(std::ostream &os, const std::vector<NearbyStop> &stops)
    {
        for (const auto &stop : stops)
        {
            PrintTo(stop, &os);
            os << ", ";
        }
        return os;
    }
}

void YellowPages::BLL::PrintTo(const AddressComponent &component, std::ostream *os)
{
    *os << "AddressComponent(" << component.value << ", " << component.type << ")";
}

void YellowPages::BLL::PrintTo(const Address &address, std::ostream *os)
{
    *os << "Address(" << "Formatted: " << address.formatted << ", \n";
    *os << "Components: ";
    ::PrintTo(address.components, os);
    *os << ", \n";
    *os << "Coords: ";
    PrintTo(address.coords, os);
    *os << ", \n";
    *os << "Comment: " << address.comment << ")";
}

void YellowPages::BLL::PrintTo(const Name &name, std::ostream *os)
{
    *os << "Name(" << name.value << ", " << name.type << ")";
}

void YellowPages::BLL::PrintTo(const Phone &phone, std::ostream *os)
{
    *os << "Phone(" << "Formatted: " << phone.formatted << ", \n"
        << "Phone type: " << phone.type << ", \n"
        << "Country code: " << phone.countryCode << ", \n"
        << "Local code: " << phone.localCode << ", \n"
        << "Number: " << phone.number << ", \n"
        << "Extension: " << phone.extension << ", \n"
        << "Description: " << phone.description << ")";
}

void YellowPages::BLL::PrintTo(const Url &url, std::ostream *os)
{
    *os << "Url(" << url.value << ")";
}

void YellowPages::BLL::PrintTo(const WorkingTimeInterval &interval, std::ostream *os)
{
    *os << "WorkingTimeInterval(" << interval.day << ", " << interval.minutesFrom << ", " << interval.minutesTo << ")";
}

void YellowPages::BLL::PrintTo(const WorkingTime &workingTime, std::ostream *os)
{
    *os << "WorkingTime(" << "Formatted: " << workingTime.formatted << ", \n";
    *os << "Intervals: ";
    ::PrintTo(workingTime.intervals, os);
    *os << ")";
}

void YellowPages::BLL::PrintTo(const NearbyStop &stop, std::ostream *os)
{
    *os << "NearbyStop(" << "Name: " << stop.name << ", \n"
        << "Meters: " << stop.meters << ")";
}

void YellowPages::BLL::PrintTo(const Company &company, std::ostream *os)
{
    *os << "Company(" << "Address: ";
    PrintTo(company.address, os);
    *os << ", \n";
    *os << "Names: " << company.names << ", \n";
    *os << "Phones: " << company.phones << ", \n";
    *os << "Urls: " << company.urls << ", \n";
    *os << "Working time: ";
    PrintTo(company.workingTime, os);
    *os << ", \n";
    *os << "Nearby stops: " << company.nearbyStops << ")";
}

void YellowPages::BLL::PrintTo(const Sphere::Point &point, std::ostream *os)
{
    *os << "Point(" << point.latitude << ", " << point.longitude << ")";
}
