#include "gtest/gtest.h"

#include "database.pb.h"
#include "company.pb.h"
#include "rubric.pb.h"
#include "address.pb.h"
#include "name.pb.h"
#include "phone.pb.h"
#include "url.pb.h"
#include "working_time.pb.h"
#include "sphere.pb.h"
#include "proto/YellowPages/ProtoMapper.h"

#include "YellowPages/Company.h"
#include "YellowPages/Rubric.h"
#include "YellowPages/YellowPagesDatabase.h"
#include "Sphere.h"

using namespace YellowPages;

TEST(YellowPagesProtoMapperTests, MapRubric) {
    BLL::Rubric rubric;
    rubric.name = "Test Rubric";
    rubric.keywords = {"keyword1", "keyword2"};

    auto pbRubric = ProtoMapper::Map(rubric);
    EXPECT_EQ(pbRubric.name(), rubric.name);
    ASSERT_EQ(pbRubric.keywords_size(), 2);
    EXPECT_EQ(pbRubric.keywords(0), "keyword1");
    EXPECT_EQ(pbRubric.keywords(1), "keyword2");

    auto mappedBack = ProtoMapper::Map(pbRubric);
    EXPECT_EQ(mappedBack.name, rubric.name);
    EXPECT_EQ(mappedBack.keywords, rubric.keywords);
}
TEST(YellowPagesProtoMapperTests, MapAddress) {
    BLL::Address address;
    address.formatted = "Test Street, 123";
    address.components = {
        {.value = "Test Street", .type = BLL::AddressComponent::Type::Street},
        {.value = "123", .type = BLL::AddressComponent::Type::House},
        {.value = "Moscow", .type = BLL::AddressComponent::Type::City}
    };
    address.coords.latitude = 55.7558;
    address.coords.longitude = 37.6173;
    address.comment = "Test comment";

    auto pbAddress = ProtoMapper::Map(address);
    EXPECT_EQ(pbAddress.formatted(), address.formatted);
    ASSERT_EQ(pbAddress.components_size(), 3);
    EXPECT_EQ(pbAddress.components(0).value(), "Test Street");
    // EXPECT_EQ(pbAddress.components(0).type(), AddressComponent_Type::AddressComponent_Type_STREET);
    EXPECT_EQ(pbAddress.components(1).value(), "123");
    // EXPECT_EQ(pbAddress.components(1).type(), AddressComponent_Type::AddressComponent_Type_HOUSE);
    EXPECT_EQ(pbAddress.components(2).value(), "Moscow");
    // EXPECT_EQ(pbAddress.components(2).type(), AddressComponent_Type::AddressComponent_Type_CITY);
    EXPECT_DOUBLE_EQ(pbAddress.coords().lat(), address.coords.latitude);
    EXPECT_DOUBLE_EQ(pbAddress.coords().lon(), address.coords.longitude);
    EXPECT_EQ(pbAddress.comment(), address.comment);

    auto mappedBack = ProtoMapper::Map(pbAddress);
    EXPECT_EQ(mappedBack.formatted, address.formatted);
    ASSERT_EQ(mappedBack.components.size(), 3);
    EXPECT_EQ(mappedBack.components[0].value, address.components[0].value);
    // EXPECT_EQ(mappedBack.components[0].type, address.components[0].type);
    EXPECT_EQ(mappedBack.components[1].value, address.components[1].value);
    // EXPECT_EQ(mappedBack.components[1].type, address.components[1].type);
    EXPECT_EQ(mappedBack.components[2].value, address.components[2].value);
    // EXPECT_EQ(mappedBack.components[2].type, address.components[2].type);
    EXPECT_DOUBLE_EQ(mappedBack.coords.latitude, address.coords.latitude);
    EXPECT_DOUBLE_EQ(mappedBack.coords.longitude, address.coords.longitude);
    EXPECT_EQ(mappedBack.comment, address.comment);
}

TEST(YellowPagesProtoMapperTests, MapName) {
    BLL::Name name;
    name.value = "Test Name";
    name.type = BLL::Name::Type::Main;

    auto pbName = ProtoMapper::Map(name);
    EXPECT_EQ(pbName.value(), name.value);
    EXPECT_EQ(pbName.type(), static_cast<Name_Type>(name.type));

    auto mappedBack = ProtoMapper::Map(pbName);
    EXPECT_EQ(mappedBack.value, name.value);
    EXPECT_EQ(mappedBack.type, name.type);
}

TEST(YellowPagesProtoMapperTests, MapPhone) {
    BLL::Phone phone;
    phone.formatted = "+7 (123) 456-78-90";
    phone.type = BLL::Phone::Type::PhoneNum;
    phone.countryCode = "7";
    phone.localCode = "123";
    phone.number = "4567890";
    phone.extension = "123";
    phone.description = "Test phone";

    auto pbPhone = ProtoMapper::Map(phone);
    EXPECT_EQ(pbPhone.formatted(), phone.formatted);
    EXPECT_EQ(pbPhone.type(), static_cast<Phone_Type>(phone.type));
    EXPECT_EQ(pbPhone.country_code(), phone.countryCode);
    EXPECT_EQ(pbPhone.local_code(), phone.localCode);
    EXPECT_EQ(pbPhone.number(), phone.number);
    EXPECT_EQ(pbPhone.extension(), phone.extension);
    EXPECT_EQ(pbPhone.description(), phone.description);

    auto mappedBack = ProtoMapper::Map(pbPhone);
    EXPECT_EQ(mappedBack.formatted, phone.formatted);
    EXPECT_EQ(mappedBack.type, phone.type);
    EXPECT_EQ(mappedBack.countryCode, phone.countryCode);
    EXPECT_EQ(mappedBack.localCode, phone.localCode);
    EXPECT_EQ(mappedBack.number, phone.number);
    EXPECT_EQ(mappedBack.extension, phone.extension);
    EXPECT_EQ(mappedBack.description, phone.description);
}

TEST(YellowPagesProtoMapperTests, MapUrl) {
    BLL::Url url;
    url.value = "http://test.com";

    auto pbUrl = ProtoMapper::Map(url);
    EXPECT_EQ(pbUrl.value(), url.value);

    auto mappedBack = ProtoMapper::Map(pbUrl);
    EXPECT_EQ(mappedBack.value, url.value);
}

TEST(YellowPagesProtoMapperTests, MapWorkingTimeInterval) {
    BLL::WorkingTimeInterval interval;
    interval.day = BLL::WorkingTimeInterval::Day::Monday;
    interval.minutesFrom = 540;  // 9:00
    interval.minutesTo = 1080;   // 18:00

    auto pbInterval = ProtoMapper::Map(interval);
    EXPECT_EQ(pbInterval.day(), WorkingTimeInterval::MONDAY);
    EXPECT_EQ(pbInterval.minutes_from(), interval.minutesFrom);
    EXPECT_EQ(pbInterval.minutes_to(), interval.minutesTo);

    auto mappedBack = ProtoMapper::Map(pbInterval);
    EXPECT_EQ(mappedBack.day, interval.day);
    EXPECT_EQ(mappedBack.minutesFrom, interval.minutesFrom);
    EXPECT_EQ(mappedBack.minutesTo, interval.minutesTo);
}

TEST(YellowPagesProtoMapperTests, MapWorkingTime) {
    BLL::WorkingTime workingTime;
    workingTime.formatted = "Mon-Fri 9:00-18:00";
    workingTime.intervals.push_back({BLL::WorkingTimeInterval::Day::Monday, 540, 1080});
    workingTime.intervals.push_back({BLL::WorkingTimeInterval::Day::Tuesday, 540, 1080});
    workingTime.intervals.push_back({BLL::WorkingTimeInterval::Day::Wednesday, 540, 1080});
    workingTime.intervals.push_back({BLL::WorkingTimeInterval::Day::Thursday, 540, 1080});
    workingTime.intervals.push_back({BLL::WorkingTimeInterval::Day::Friday, 540, 1080});

    auto pbWorkingTime = ProtoMapper::Map(workingTime);
    EXPECT_EQ(pbWorkingTime.formatted(), workingTime.formatted);
    ASSERT_EQ(pbWorkingTime.intervals_size(), 5);
    for(int i = 0; i < 5; ++i) 
    {
        EXPECT_EQ(pbWorkingTime.intervals(i).day(), workingTime.intervals[i].day) << "I: " << i;
        EXPECT_EQ(pbWorkingTime.intervals(i).minutes_from(), workingTime.intervals[i].minutesFrom) << "I: " << i;
        EXPECT_EQ(pbWorkingTime.intervals(i).minutes_to(), workingTime.intervals[i].minutesTo) << "I: " << i;
    }

    auto mappedBack = ProtoMapper::Map(pbWorkingTime);
    EXPECT_EQ(mappedBack.formatted, workingTime.formatted);
    ASSERT_EQ(mappedBack.intervals.size(), 5);
    EXPECT_EQ(mappedBack.intervals[0], workingTime.intervals[0]);
    EXPECT_EQ(mappedBack.intervals[1], workingTime.intervals[1]);
    EXPECT_EQ(mappedBack.intervals[2], workingTime.intervals[2]);
    EXPECT_EQ(mappedBack.intervals[3], workingTime.intervals[3]);
    EXPECT_EQ(mappedBack.intervals[4], workingTime.intervals[4]);
}

TEST(YellowPagesProtoMapperTests, MapNearbyStop) {
    BLL::NearbyStop stop;
    stop.name = "Test Stop";
    stop.meters = 100;

    auto pbStop = ProtoMapper::Map(stop);
    EXPECT_EQ(pbStop.name(), stop.name);
    EXPECT_EQ(pbStop.meters(), stop.meters);

    auto mappedBack = ProtoMapper::Map(pbStop);
    EXPECT_EQ(mappedBack.name, stop.name);
    EXPECT_EQ(mappedBack.meters, stop.meters);
}

TEST(YellowPagesProtoMapperTests, MapCompany) {
    BLL::Company company;
    company.names.emplace_back(BLL::Name{.value = "Test Company", .type = BLL::Name::Type::Main});
    company.names.emplace_back(BLL::Name{.value = "Test SYNONYM", .type = BLL::Name::Type::Synonym}); 
    company.names.emplace_back(BLL::Name{.value = "Another Name", .type = BLL::Name::Type::Short});
    
    company.phones.push_back({.formatted = "+7 (123) 456-78-90", .type = BLL::Phone::Type::PhoneNum, .countryCode = "7", .localCode = "123", .number = "4567890", .extension = "123", .description = "Main phone"});
    company.phones.push_back({.formatted = "+7 (123) 456-78-91", .type = BLL::Phone::Type::Fax, .countryCode = "7", .localCode = "123", .number = "4567891", .extension = "", .description = "Fax"});
    company.phones.push_back({.formatted = "+7 (123) 456-78-92", .type = BLL::Phone::Type::PhoneNum, .countryCode = "7", .localCode = "123", .number = "4567892", .extension = "", .description = "Support"});
    
    company.urls.push_back({.value = "http://test.com"});
    company.urls.push_back({.value = "http://test.ru"});
    company.urls.push_back({.value = "https://test.org"});
    
    company.rubrics = {1, 2, 3, 4, 5};
    
    company.workingTime.formatted = "Mon-Fri 9:00-18:00";
    
    company.nearbyStops.push_back({.name = "Test Stop 1", .meters = 100});
    company.nearbyStops.push_back({.name = "Test Stop 2", .meters = 200});
    company.nearbyStops.push_back({.name = "Test Stop 3", .meters = 300});
    company.nearbyStops.push_back({.name = "Test Stop 4", .meters = 400});

    auto pbCompany = ProtoMapper::Map(company);
    ASSERT_EQ(pbCompany.names_size(), 3);
    EXPECT_EQ(pbCompany.names(0).value(), "Test Company");
    EXPECT_EQ(pbCompany.names(0).type(), Name_Type::Name_Type_MAIN);
    EXPECT_EQ(pbCompany.names(1).value(), "Test SYNONYM");
    EXPECT_EQ(pbCompany.names(1).type(), Name_Type::Name_Type_SYNONYM);
    EXPECT_EQ(pbCompany.names(2).value(), "Another Name");
    EXPECT_EQ(pbCompany.names(2).type(), Name_Type::Name_Type_SHORT);

    ASSERT_EQ(pbCompany.phones_size(), 3);
    EXPECT_EQ(pbCompany.phones(0).formatted(), "+7 (123) 456-78-90");
    EXPECT_EQ(pbCompany.phones(0).type(), Phone_Type::Phone_Type_PHONE);
    EXPECT_EQ(pbCompany.phones(0).country_code(), "7");
    EXPECT_EQ(pbCompany.phones(0).local_code(), "123");
    EXPECT_EQ(pbCompany.phones(0).number(), "4567890");
    EXPECT_EQ(pbCompany.phones(0).extension(), "123");
    EXPECT_EQ(pbCompany.phones(0).description(), "Main phone");

    ASSERT_EQ(pbCompany.urls_size(), 3);
    EXPECT_EQ(pbCompany.urls(0).value(), "http://test.com");
    EXPECT_EQ(pbCompany.urls(1).value(), "http://test.ru");
    EXPECT_EQ(pbCompany.urls(2).value(), "https://test.org");

    ASSERT_EQ(pbCompany.rubrics_size(), 5);
    EXPECT_EQ(pbCompany.rubrics(0), 1);
    EXPECT_EQ(pbCompany.rubrics(1), 2);
    EXPECT_EQ(pbCompany.rubrics(2), 3);
    EXPECT_EQ(pbCompany.rubrics(3), 4);
    EXPECT_EQ(pbCompany.rubrics(4), 5);

    EXPECT_EQ(pbCompany.working_time().formatted(), "Mon-Fri 9:00-18:00");

    ASSERT_EQ(pbCompany.nearby_stops_size(), 4);
    EXPECT_EQ(pbCompany.nearby_stops(0).name(), "Test Stop 1");
    EXPECT_EQ(pbCompany.nearby_stops(0).meters(), 100);
    EXPECT_EQ(pbCompany.nearby_stops(1).name(), "Test Stop 2");
    EXPECT_EQ(pbCompany.nearby_stops(1).meters(), 200);
    EXPECT_EQ(pbCompany.nearby_stops(2).name(), "Test Stop 3");
    EXPECT_EQ(pbCompany.nearby_stops(2).meters(), 300);
    EXPECT_EQ(pbCompany.nearby_stops(3).name(), "Test Stop 4");
    EXPECT_EQ(pbCompany.nearby_stops(3).meters(), 400);

    auto mappedBack = ProtoMapper::Map(pbCompany);
    ASSERT_EQ(mappedBack.names.size(), 3);
    EXPECT_EQ(mappedBack.names[0].value, "Test Company");
    EXPECT_EQ(mappedBack.names[0].type, BLL::Name::Type::Main);
    EXPECT_EQ(mappedBack.names[1].value, "Test SYNONYM");
    EXPECT_EQ(mappedBack.names[1].type, BLL::Name::Type::Synonym);
    EXPECT_EQ(mappedBack.names[2].value, "Another Name");
    EXPECT_EQ(mappedBack.names[2].type, BLL::Name::Type::Short);

    ASSERT_EQ(mappedBack.phones.size(), 3);
    EXPECT_EQ(mappedBack.phones[0].formatted, "+7 (123) 456-78-90");
    EXPECT_EQ(mappedBack.phones[0].type, BLL::Phone::Type::PhoneNum);
    EXPECT_EQ(mappedBack.phones[0].countryCode, "7");
    EXPECT_EQ(mappedBack.phones[0].localCode, "123");
    EXPECT_EQ(mappedBack.phones[0].number, "4567890");
    EXPECT_EQ(mappedBack.phones[0].extension, "123");
    EXPECT_EQ(mappedBack.phones[0].description, "Main phone");

    ASSERT_EQ(mappedBack.urls.size(), 3);
    EXPECT_EQ(mappedBack.urls[0].value, "http://test.com");
    EXPECT_EQ(mappedBack.urls[1].value, "http://test.ru");
    EXPECT_EQ(mappedBack.urls[2].value, "https://test.org");

    ASSERT_EQ(mappedBack.rubrics.size(), 5);
    EXPECT_EQ(mappedBack.rubrics[0], 1);
    EXPECT_EQ(mappedBack.rubrics[1], 2);
    EXPECT_EQ(mappedBack.rubrics[2], 3);
    EXPECT_EQ(mappedBack.rubrics[3], 4);
    EXPECT_EQ(mappedBack.rubrics[4], 5);

    EXPECT_EQ(mappedBack.workingTime.formatted, "Mon-Fri 9:00-18:00");

    ASSERT_EQ(mappedBack.nearbyStops.size(), 4);
    EXPECT_EQ(mappedBack.nearbyStops[0].name, "Test Stop 1");
    EXPECT_EQ(mappedBack.nearbyStops[0].meters, 100);
    EXPECT_EQ(mappedBack.nearbyStops[1].name, "Test Stop 2");
    EXPECT_EQ(mappedBack.nearbyStops[1].meters, 200);
    EXPECT_EQ(mappedBack.nearbyStops[2].name, "Test Stop 3");
    EXPECT_EQ(mappedBack.nearbyStops[2].meters, 300);
    EXPECT_EQ(mappedBack.nearbyStops[3].name, "Test Stop 4");
    EXPECT_EQ(mappedBack.nearbyStops[3].meters, 400);
}

TEST(YellowPagesProtoMapperTests, MapDatabase) {
    std::vector<BLL::Company> companies;
    std::unordered_map<BLL::Rubric::IdType, BLL::Rubric> rubrics;

    // Create rubrics
    BLL::Rubric rubric1;
    rubric1.name = "Cafe";
    rubric1.keywords = {"coffee", "food", "drinks"};
    rubrics[1] = rubric1;

    BLL::Rubric rubric2;
    rubric2.name = "Restaurant"; 
    rubric2.keywords = {"dinner", "food", "cuisine"};
    rubrics[2] = rubric2;

    BLL::Rubric rubric3;
    rubric3.name = "Bar";
    rubric3.keywords = {"drinks", "cocktails", "beer"};
    rubrics[3] = rubric3;

    // Create companies
    BLL::Company company1;
    company1.names = {{.value = "Coffee House", .type = BLL::Name::Type::Main}};
    company1.rubrics = {1, 2};

    BLL::Company company2;
    company2.names = {{.value = "Good Restaurant", .type = BLL::Name::Type::Main}};
    company2.rubrics = {2};

    BLL::Company company3;
    company3.names = {{.value = "Night Bar", .type = BLL::Name::Type::Main}};
    company3.rubrics = {2, 3};

    companies = {company1, company2, company3};

    BLL::YellowPagesDatabase db(std::move(companies), std::move(rubrics));

    // Map to protobuf
    auto pbDb = ProtoMapper::Map(db);

    // Verify companies
    ASSERT_EQ(pbDb.companies_size(), 3);
    EXPECT_EQ(pbDb.companies(0).names(0).value(), "Coffee House");
    EXPECT_EQ(pbDb.companies(1).names(0).value(), "Good Restaurant");
    EXPECT_EQ(pbDb.companies(2).names(0).value(), "Night Bar");

    // Verify rubrics
    ASSERT_EQ(pbDb.rubrics_size(), 3);
    EXPECT_EQ(pbDb.rubrics().at(1).name(), "Cafe");
    EXPECT_EQ(pbDb.rubrics().at(2).name(), "Restaurant");
    EXPECT_EQ(pbDb.rubrics().at(3).name(), "Bar");

    // Map back and verify
    auto mappedBack = ProtoMapper::Map(pbDb);
    ASSERT_EQ(mappedBack.GetCompanies().size(), 3);
    ASSERT_EQ(mappedBack.GetRubrics().size(), 3);

    const auto& companies2 = mappedBack.GetCompanies();
    EXPECT_EQ(companies2[0].names[0].value, "Coffee House");
    EXPECT_EQ(companies2[1].names[0].value, "Good Restaurant");
    EXPECT_EQ(companies2[2].names[0].value, "Night Bar");

    const auto& rubrics2 = mappedBack.GetRubrics();
    EXPECT_EQ(rubrics2.at(1).name, "Cafe");
    EXPECT_EQ(rubrics2.at(2).name, "Restaurant");
    EXPECT_EQ(rubrics2.at(3).name, "Bar");
}
