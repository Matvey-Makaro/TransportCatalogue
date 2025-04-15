#pragma once

namespace Sphere
{
    class Point;
}
namespace SphereProto
{
    class Coords;
}

namespace YellowPages
{
    namespace BLL
    {
        class YellowPagesDatabase;
        class Company;
        class Rubric;
        class Address;
        class Name;
        class Phone;
        class Url;
        class WorkingTime;
        class NearbyStop;
        class AddressComponent;
        class WorkingTimeInterval;
    }

    class Database;
    class Company;
    class Rubric;
    class Address;
    class Name;
    class Phone;
    class Url;
    class WorkingTime;
    class NearbyStop;
    class AddressComponent;
    class WorkingTimeInterval;

    class ProtoMapper
    {
    public:
        ProtoMapper() = delete;

        static Database Map(const BLL::YellowPagesDatabase &db);
        static BLL::YellowPagesDatabase Map(const Database &pbDb);

        static Rubric Map(const BLL::Rubric &rubric);
        static BLL::Rubric Map(const Rubric &pbRubric);

        static Company Map(const BLL::Company &company);
        static BLL::Company Map(const Company &pbCompany);
        
        static Address Map(const BLL::Address &address);
        static BLL::Address Map(const Address &pbAddress);

        static Name Map(const BLL::Name &name);
        static BLL::Name Map(const Name &pbName);

        static Phone Map(const BLL::Phone &phone);
        static BLL::Phone Map(const Phone &pbPhone);

        static Url Map(const BLL::Url &url);
        static BLL::Url Map(const Url &pbUrl);

        static WorkingTime Map(const BLL::WorkingTime &workingTime);
        static BLL::WorkingTime Map(const WorkingTime &pbWorkingTime);

        static NearbyStop Map(const BLL::NearbyStop &nearbyStop);
        static BLL::NearbyStop Map(const NearbyStop &pbNearbyStop);
        
        static AddressComponent Map(const BLL::AddressComponent &addressComponent);
        static BLL::AddressComponent Map(const AddressComponent &pbAddressComponent);

        static SphereProto::Coords Map(const Sphere::Point &point);
        static Sphere::Point Map(const SphereProto::Coords &pbCoords);

        static WorkingTimeInterval Map(const BLL::WorkingTimeInterval &workingTimeInterval);
        static BLL::WorkingTimeInterval Map(const WorkingTimeInterval &pbWorkingTimeInterval);
    };
}