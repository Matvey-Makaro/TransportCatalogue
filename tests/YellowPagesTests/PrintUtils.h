#pragma once

#include <iostream>
#include "YellowPages/Company.h"
#include "Sphere.h"

namespace YellowPages::BLL
{
    void PrintTo(const YellowPages::BLL::AddressComponent &component, std::ostream *os);
    void PrintTo(const YellowPages::BLL::Address &address, std::ostream *os);
    void PrintTo(const YellowPages::BLL::Name &name, std::ostream *os);
    void PrintTo(const YellowPages::BLL::Phone &phone, std::ostream *os);
    void PrintTo(const YellowPages::BLL::Url &url, std::ostream *os);
    void PrintTo(const YellowPages::BLL::WorkingTimeInterval &interval, std::ostream *os);
    void PrintTo(const YellowPages::BLL::WorkingTime &workingTime, std::ostream *os);
    void PrintTo(const YellowPages::BLL::NearbyStop &stop, std::ostream *os);
    void PrintTo(const YellowPages::BLL::Company &company, std::ostream *os);
    void PrintTo(const Sphere::Point &point, std::ostream *os);
}
