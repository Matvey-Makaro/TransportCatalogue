#include <string>
#include <sstream>
#include <gtest/gtest.h>

#include "YellowPages/JsonParser.h"
#include "YellowPages/Rubric.h"
#include "Json.h"
#include "PrintUtils.h"

using namespace YellowPages::BLL;

namespace
{
    Json::Dict CreateDict(const std::string &json)
    {
        std::stringstream in(json);
        const auto inputDoc = Json::Load(in);
        return inputDoc.GetRoot().AsMap();
    }
    Json::Dict CreateYellowPagesJsonDict(const std::string &json)
    {
        std::stringstream in(json);
        const auto inputDoc = Json::Load(in);
        const auto &inputMap = inputDoc.GetRoot().AsMap();
        return inputMap.at("yellow_pages").AsMap();
    }
}

TEST(YellowPagesJsonParserTest, ParseRubrics1)
{
    std::string json = R"({
"yellow_pages": {
"rubrics": {}
}
})";
    auto rubricsMap = JsonParser::ParseRubrics(CreateYellowPagesJsonDict(json));
    JsonParser::RubricMap expectedRubricsMap;
    EXPECT_EQ(rubricsMap, expectedRubricsMap);
}

TEST(YellowPagesJsonParserTest, ParseRubrics2)
{
    std::string json = R"({
    "yellow_pages": {
        "rubrics": {
            "1": {
                "name": "Парк"
            }
        }
    }
})";
    auto rubricsMap = JsonParser::ParseRubrics(CreateYellowPagesJsonDict(json));
    JsonParser::RubricMap expectedRubricsMap = {
        std::make_pair(1, Rubric{.name = "Парк", .keywords = {}})};
    EXPECT_EQ(rubricsMap, expectedRubricsMap);
}

TEST(YellowPagesJsonParserTest, ParseRubrics3)
{
    std::string json = R"({
    "yellow_pages": {
        "rubrics": {
            "1": {
                "name": "Парк",
                "keywords": [
                    "Гулять",
                    "Природа",
                    "Keyword1"
                ]
            }
        }
    }
})";
    auto rubricsMap = JsonParser::ParseRubrics(CreateYellowPagesJsonDict(json));
    JsonParser::RubricMap expectedRubricsMap = {
        std::make_pair(1, Rubric{.name = "Парк", .keywords = {"Гулять", "Природа", "Keyword1"}})};
    EXPECT_EQ(rubricsMap, expectedRubricsMap);
}

TEST(YellowPagesJsonParserTest, ParseRubrics4)
{
    std::string json = R"({
    "yellow_pages": {
        "rubrics": {
            "1": {
                "name": "Парк",
                "keywords": [
                    "Гулять",
                    "Природа",
                    "Keyword1"
                ]
            },
            "2": {
                "name": "Кафе",
                "keywords": [
                    "Кафе1",
                    "Кафе2"
                ]
            },
            "3": {
                "name": "Аптека",
                "keywords": []
            }
        }
    }
})";
    auto rubricsMap = JsonParser::ParseRubrics(CreateYellowPagesJsonDict(json));
    JsonParser::RubricMap expectedRubricsMap = {
        std::make_pair(1, Rubric{.name = "Парк", .keywords = {"Гулять", "Природа", "Keyword1"}}),
        std::make_pair(2, Rubric{.name = "Кафе", .keywords = {"Кафе1", "Кафе2"}}),
        std::make_pair(3, Rubric{.name = "Аптека", .keywords = {}}),

    };
    EXPECT_EQ(rubricsMap, expectedRubricsMap);
}

TEST(YellowPagesJsonParserTest, ParseCompanies1)
{
    std::string json = R"({"companies": [
            {
                "names": [
                    {
                        "value": "Дендрарий"
                    }
                ],
                "urls": [
                    {
                        "value": "http://dendrarium.ru"
                    }
                ],
                "rubrics": [
                    1
                ],
                "address": {
                    "coords": {
                        "lat": 43.573226,
                        "lon": 39.742947
                    }
                },
                "nearby_stops": [
                    {
                        "name": "Цирк",
                        "meters": 180
                    }
                ],
                "phones": [
                    {
                        "type": "PHONE",
                        "country_code": "7",
                        "local_code": "862",
                        "number": "2671646"
                    },
                    {
                        "type": "FAX",
                        "country_code": "7",
                        "local_code": "862",
                        "number": "2671842"
                    }
                ]
            }
        ]})";
    std::vector<Company> companies = JsonParser::ParseCompanies(CreateDict(json));
    Company expectedCompany;
    {
        Name name1{
            .value = "Дендрарий",
            .type = Name::Main,
        };
        expectedCompany.names.emplace_back(name1);
    }
    {
        expectedCompany.urls.emplace_back(Url{.value = "http://dendrarium.ru"});
    }
    {
        expectedCompany.rubrics.emplace_back(1);
    }
    {
        Address address;
        address.coords = Sphere::Point{.latitude = 43.573226, .longitude = 39.742947};
        expectedCompany.address = address;
    }
    {
        expectedCompany.nearbyStops.emplace_back(NearbyStop{
            .name = "Цирк",
            .meters = 180});
    }
    {
        Phone phone1;
        phone1.type = Phone::PhoneNum;
        phone1.countryCode = "7";
        phone1.localCode = "862";
        phone1.number = "2671646";

        Phone phone2;
        phone2.type = Phone::Fax;
        phone2.countryCode = "7";
        phone2.localCode = "862";
        phone2.number = "2671842";
        expectedCompany.phones = {phone1, phone2};
    }

    std::vector<Company> expectedCompanies = {expectedCompany};
    EXPECT_EQ(companies, expectedCompanies);
}

TEST(YellowPagesJsonParserTest, ParseCompanies2)
{
    std::string json = R"({"companies": [
        {
            "names": [
                {
                    "value": "Дендрарий"
                }
            ],
            "urls": [
                {
                    "value": "http://dendrarium.ru"
                }
            ],
            "rubrics": [
                1
            ],
            "address": {
                "coords": {
                    "lat": 43.573226,
                    "lon": 39.742947
                }
            },
            "nearby_stops": [
                {
                    "name": "Цирк",
                    "meters": 180
                }
            ],
            "phones": [
                {
                    "type": "PHONE",
                    "country_code": "7",
                    "local_code": "862",
                    "number": "2671646"
                },
                {
                    "type": "FAX",
                    "country_code": "7",
                    "local_code": "862",
                    "number": "2671842"
                }
            ]
        },
        {
            "names": [
                {
                    "value": "им. Фрунзе"
                }
            ],
            "rubrics": [
                1
            ],
            "address": {
                "coords": {
                    "lat": 43.567998,
                    "lon": 39.734131
                }
            },
            "nearby_stops": [
                {
                    "name": "Пансионат Светлана",
                    "meters": 580
                },
                {
                    "name": "Цирк",
                    "meters": 700
                },
                {
                    "name": "Театральная",
                    "meters": 1000
                }
            ]
        }
    ])})";

    std::vector<Company> companies = JsonParser::ParseCompanies(CreateDict(json));
    std::vector<Company> expectedCompanies;

    {
        Company expectedCompany;
        {
            Name name1{
                .value = "Дендрарий",
                .type = Name::Main,
            };
            expectedCompany.names.emplace_back(name1);
        }
        {
            expectedCompany.urls.emplace_back(Url{.value = "http://dendrarium.ru"});
        }
        {
            expectedCompany.rubrics.emplace_back(1);
        }
        {
            Address address;
            address.coords = Sphere::Point{.latitude = 43.573226, .longitude = 39.742947};
            expectedCompany.address = address;
        }
        {
            expectedCompany.nearbyStops.emplace_back(NearbyStop{
                .name = "Цирк",
                .meters = 180});
        }
        {
            Phone phone1;
            phone1.type = Phone::PhoneNum;
            phone1.countryCode = "7";
            phone1.localCode = "862";
            phone1.number = "2671646";

            Phone phone2;
            phone2.type = Phone::Fax;
            phone2.countryCode = "7";
            phone2.localCode = "862";
            phone2.number = "2671842";
            expectedCompany.phones = {phone1, phone2};
        }
        expectedCompanies.emplace_back(expectedCompany);
    }

    {
        Company expectedCompany;
        {
            Name name1{
                .value = "им. Фрунзе",
                .type = Name::Main,
            };
            expectedCompany.names.emplace_back(name1);
        }
        {
            expectedCompany.rubrics.emplace_back(1);
        }
        {
            Address address;
            address.coords = Sphere::Point{.latitude = 43.567998, .longitude = 39.734131};
            expectedCompany.address = address;
        }
        {
            expectedCompany.nearbyStops.emplace_back(NearbyStop{
                .name = "Пансионат Светлана",
                .meters = 580});
            expectedCompany.nearbyStops.emplace_back(NearbyStop{
                .name = "Цирк",
                .meters = 700});
            expectedCompany.nearbyStops.emplace_back(NearbyStop{
                .name = "Театральная",
                .meters = 1000});
        }
        expectedCompanies.emplace_back(expectedCompany);
    }

    EXPECT_EQ(companies, expectedCompanies);
}

TEST(JsonParserTests, ParseCompaniesAllFields)
{
    Json::Dict yellowPages;
    Json::Arr companiesJson;
    {
        Json::Dict company;
        
        // Names
        Json::Arr names;
        {
            Json::Dict name;
            name["value"] = "Main Company Name";
            name["type"] = "MAIN";
            names.push_back(name);
        }
        {
            Json::Dict name;
            name["value"] = "Alternative Name";
            name["type"] = "SYNONYM";
            names.push_back(name);
        }
        company["names"] = names;

        // Address
        Json::Dict address;
        {
            address["formatted"] = "Full Address String";
            
            Json::Arr components;
            {
                Json::Dict component;
                component["value"] = "Street Component";
                component["type"] = "STREET";
                components.push_back(component);
            }
            address["components"] = components;
            
            Json::Dict coords;
            coords["lat"] = 55.75;
            coords["lon"] = 37.62;
            address["coords"] = coords;
            
            address["comment"] = "Address comment";
        }
        company["address"] = address;

        // Phones
        Json::Arr phones;
        {
            Json::Dict phone;
            phone["type"] = "PHONE";
            phone["country_code"] = "7";
            phone["local_code"] = "495";
            phone["number"] = "1234567";
            phone["extension"] = "123";
            phone["description"] = "Main office";
            phones.push_back(phone);
        }
        company["phones"] = phones;

        // URLs
        Json::Arr urls;
        {
            Json::Dict url;
            url["value"] = "http://example.com";
            urls.push_back(url);
        }
        company["urls"] = urls;

        // Rubrics
        Json::Arr rubrics;
        rubrics.push_back(Json::Node(1));
        rubrics.push_back(Json::Node(2));
        company["rubrics"] = rubrics;

        // Working Time
        Json::Dict workingTime;
        {
            workingTime["formatted"] = "Mon-Fri 9-18";
            Json::Arr intervals;
            {
                Json::Dict interval;
                interval["day"] = "MONDAY";
                interval["minutes_from"] = 540;  // 9:00
                interval["minutes_to"] = 1080;   // 18:00
                intervals.push_back(interval);
            }
            workingTime["intervals"] = intervals;
        }
        company["working_time"] = workingTime;

        // Nearby Stops
        Json::Arr nearbyStops;
        {
            Json::Dict stop;
            stop["name"] = "Bus Stop";
            stop["meters"] = 100;
            nearbyStops.push_back(stop);
        }
        company["nearby_stops"] = nearbyStops;

        companiesJson.push_back(company);
    }
    yellowPages["companies"] = companiesJson;

    JsonParser parser;
    auto companies = parser.ParseCompanies(yellowPages);
    ASSERT_EQ(companies.size(), 1);

    const auto& company = companies[0];

    // Verify Names
    ASSERT_EQ(company.names.size(), 2);
    EXPECT_EQ(company.names[0].value, "Main Company Name");
    EXPECT_EQ(company.names[0].type, Name::Main);
    EXPECT_EQ(company.names[1].value, "Alternative Name");
    EXPECT_EQ(company.names[1].type, Name::Synonym);

    // Verify Address
    EXPECT_EQ(company.address.formatted, "Full Address String");
    ASSERT_EQ(company.address.components.size(), 1);
    EXPECT_EQ(company.address.components[0].value, "Street Component");
    EXPECT_EQ(company.address.components[0].type, AddressComponent::Street);
    EXPECT_DOUBLE_EQ(company.address.coords.latitude, 55.75);
    EXPECT_DOUBLE_EQ(company.address.coords.longitude, 37.62);
    EXPECT_EQ(company.address.comment, "Address comment");

    // Verify Phones
    ASSERT_EQ(company.phones.size(), 1);
    EXPECT_EQ(company.phones[0].type, Phone::PhoneNum);
    EXPECT_EQ(company.phones[0].countryCode, "7");
    EXPECT_EQ(company.phones[0].localCode, "495");
    EXPECT_EQ(company.phones[0].number, "1234567");
    EXPECT_EQ(company.phones[0].extension, "123");
    EXPECT_EQ(company.phones[0].description, "Main office");

    // Verify URLs
    ASSERT_EQ(company.urls.size(), 1);
    EXPECT_EQ(company.urls[0].value, "http://example.com");

    // Verify Rubrics
    ASSERT_EQ(company.rubrics.size(), 2);
    EXPECT_EQ(company.rubrics[0], 1);
    EXPECT_EQ(company.rubrics[1], 2);

    // Verify Working Time
    EXPECT_STREQ(company.workingTime.formatted.c_str(), "Mon-Fri 9-18");
    ASSERT_EQ(company.workingTime.intervals.size(), 1);
    EXPECT_EQ(company.workingTime.intervals[0].day, WorkingTimeInterval::Monday);
    EXPECT_EQ(company.workingTime.intervals[0].minutesFrom, 540);
    EXPECT_EQ(company.workingTime.intervals[0].minutesTo, 1080);

    // Verify Nearby Stops
    ASSERT_EQ(company.nearbyStops.size(), 1);
    EXPECT_EQ(company.nearbyStops[0].name, "Bus Stop");
    EXPECT_EQ(company.nearbyStops[0].meters, 100);
}

TEST(JsonParserTests, ParseCompanyEmptyFields) {
    std::string json = R"({
        "companies": [{
            "names": [{
                "value": "Test Company"
            }]
        }]
    })";

    auto companies = JsonParser::ParseCompanies(CreateDict(json));
    ASSERT_EQ(companies.size(), 1);
    
    const auto& company = companies[0];
    
    // Only name should be set, other fields empty
    ASSERT_EQ(company.names.size(), 1);
    EXPECT_EQ(company.names[0].value, "Test Company");
    EXPECT_EQ(company.names[0].type, Name::Main);
    
    EXPECT_TRUE(company.phones.empty());
    EXPECT_TRUE(company.urls.empty());
    EXPECT_TRUE(company.rubrics.empty());
    EXPECT_TRUE(company.nearbyStops.empty());
    EXPECT_TRUE(company.address.formatted.empty());
    EXPECT_TRUE(company.workingTime.formatted.empty());
}

TEST(JsonParserTests, ParseCompanyMultiplePhones) {
    std::string json = R"({
        "companies": [{
            "names": [{"value": "Company"}],
            "phones": [
                {
                    "type": "PHONE",
                    "country_code": "7",
                    "local_code": "495",
                    "number": "1111111"
                },
                {
                    "type": "FAX", 
                    "country_code": "7",
                    "local_code": "495",
                    "number": "2222222"
                },
                {
                    "type": "PHONE",
                    "country_code": "7", 
                    "local_code": "495",
                    "number": "3333333",
                    "extension": "123",
                    "description": "Support"
                }
            ]
        }]
    })";

    auto companies = JsonParser::ParseCompanies(CreateDict(json));
    ASSERT_EQ(companies.size(), 1);
    
    const auto& company = companies[0];
    ASSERT_EQ(company.phones.size(), 3);
    
    EXPECT_EQ(company.phones[0].type, Phone::PhoneNum);
    EXPECT_EQ(company.phones[0].number, "1111111");
    
    EXPECT_EQ(company.phones[1].type, Phone::Fax);
    EXPECT_EQ(company.phones[1].number, "2222222");
    
    EXPECT_EQ(company.phones[2].type, Phone::PhoneNum);
    EXPECT_EQ(company.phones[2].number, "3333333");
    EXPECT_EQ(company.phones[2].extension, "123");
    EXPECT_EQ(company.phones[2].description, "Support");
}

TEST(JsonParserTests, ParseCompanyComplexWorkingTime) {
    std::string json = R"({
        "companies": [{
            "names": [{"value": "Company"}],
            "working_time": {
                "formatted": "Mon-Fri 9-18, Sat 10-16",
                "intervals": [
                    {
                        "day": "MONDAY",
                        "minutes_from": 540,
                        "minutes_to": 1080
                    },
                    {
                        "day": "SATURDAY", 
                        "minutes_from": 600,
                        "minutes_to": 960
                    }
                ]
            }
        }]
    })";

    auto companies = JsonParser::ParseCompanies(CreateDict(json));
    ASSERT_EQ(companies.size(), 1);
    
    const auto& company = companies[0];
    EXPECT_EQ(company.workingTime.formatted, "Mon-Fri 9-18, Sat 10-16");
    ASSERT_EQ(company.workingTime.intervals.size(), 2);
    
    EXPECT_EQ(company.workingTime.intervals[0].day, WorkingTimeInterval::Monday);
    EXPECT_EQ(company.workingTime.intervals[0].minutesFrom, 540);
    EXPECT_EQ(company.workingTime.intervals[0].minutesTo, 1080);
    
    EXPECT_EQ(company.workingTime.intervals[1].day, WorkingTimeInterval::Saturday);
    EXPECT_EQ(company.workingTime.intervals[1].minutesFrom, 600);
    EXPECT_EQ(company.workingTime.intervals[1].minutesTo, 960);
}
