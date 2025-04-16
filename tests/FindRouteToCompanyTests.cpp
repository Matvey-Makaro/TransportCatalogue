#include <gtest/gtest.h>
#include <tuple>
#include <sstream>

#include "TransportDatabase.h"
#include "YellowPages/YellowPagesDatabase.h"
#include "YellowPages/JsonParser.h"
#include "Json.h"

using namespace YellowPages::BLL;
using namespace Router;

namespace
{
    std::pair<TransportDatabase, YellowPages::BLL::YellowPagesDatabase> ParseTransportAndYellowPagesDatabases(const std::string& json)
    {
        std::stringstream in(json);
        const auto inputDoc = Json::Load(in);
        const auto& inputMap = inputDoc.GetRoot().AsMap();

        TransportDatabase transportDb(
            Descriptions::ReadDescriptions(inputMap.at("base_requests").AsArray()),
            Router::RoutingSettings::FromJson(inputMap.at("routing_settings").AsMap()),
            Visualization::RenderSettings::ParseFrom(inputMap.at("render_settings").AsMap()));

        auto yellowPagesDb = YellowPages::BLL::JsonParser::ParseYellowPages(inputMap.at("yellow_pages").AsMap());
        return std::make_pair(std::move(transportDb), std::move(yellowPagesDb));
    }

    const std::string testData1 = R"({
    "serialization_settings": {
        "file": "/tmp/tmp8h4r0nnk"
    },
    "routing_settings": {
        "bus_wait_time": 2,
        "bus_velocity": 30,
        "pedestrian_velocity": 5
    },
    "render_settings": {
        "width": 1200,
        "height": 500,
        "padding": 50,
        "outer_margin": 200,
        "stop_radius": 5,
        "company_radius": 6,
        "line_width": 14,
        "company_line_width": 4,
        "bus_label_font_size": 20,
        "bus_label_offset": [
            7,
            15
        ],
        "stop_label_font_size": 18,
        "stop_label_offset": [
            7,
            -3
        ],
        "underlayer_color": [
            255,
            255,
            255,
            0.85
        ],
        "underlayer_width": 3,
        "color_palette": [
            "green",
            [
                255,
                160,
                0
            ],
            "red"
        ],
        "layers": [
            "bus_lines",
            "company_lines",
            "bus_labels",
            "stop_points",
            "company_points",
            "stop_labels",
            "company_labels"
        ]
    },
    "base_requests": [
        {
            "type": "Bus",
            "name": "14",
            "stops": [
                "Улица Лизы Чайкиной",
                "Электросети",
                "Ривьерский мост",
                "Гостиница Сочи",
                "Кубанская улица",
                "По требованию",
                "Улица Докучаева",
                "Улица Лизы Чайкиной"
            ],
            "is_roundtrip": true
        },
        {
            "type": "Bus",
            "name": "24",
            "stops": [
                "Улица Докучаева",
                "Параллельная улица",
                "Электросети",
                "Санаторий Родина"
            ],
            "is_roundtrip": false
        },
        {
            "type": "Bus",
            "name": "114",
            "stops": [
                "Морской вокзал",
                "Ривьерский мост"
            ],
            "is_roundtrip": false
        },
        {
            "type": "Stop",
            "name": "Улица Лизы Чайкиной",
            "latitude": 43.590317,
            "longitude": 39.746833,
            "road_distances": {
                "Электросети": 4300,
                "Улица Докучаева": 2000
            }
        },
        {
            "type": "Stop",
            "name": "Морской вокзал",
            "latitude": 43.581969,
            "longitude": 39.719848,
            "road_distances": {
                "Ривьерский мост": 850
            }
        },
        {
            "type": "Stop",
            "name": "Электросети",
            "latitude": 43.598701,
            "longitude": 39.730623,
            "road_distances": {
                "Санаторий Родина": 4500,
                "Параллельная улица": 1200,
                "Ривьерский мост": 1900
            }
        },
        {
            "type": "Stop",
            "name": "Ривьерский мост",
            "latitude": 43.587795,
            "longitude": 39.716901,
            "road_distances": {
                "Морской вокзал": 850,
                "Гостиница Сочи": 1740
            }
        },
        {
            "type": "Stop",
            "name": "Гостиница Сочи",
            "latitude": 43.578079,
            "longitude": 39.728068,
            "road_distances": {
                "Кубанская улица": 320
            }
        },
        {
            "type": "Stop",
            "name": "Кубанская улица",
            "latitude": 43.578509,
            "longitude": 39.730959,
            "road_distances": {
                "По требованию": 370
            }
        },
        {
            "type": "Stop",
            "name": "По требованию",
            "latitude": 43.579285,
            "longitude": 39.733742,
            "road_distances": {
                "Улица Докучаева": 600
            }
        },
        {
            "type": "Stop",
            "name": "Улица Докучаева",
            "latitude": 43.585586,
            "longitude": 39.733879,
            "road_distances": {
                "Параллельная улица": 1100
            }
        },
        {
            "type": "Stop",
            "name": "Параллельная улица",
            "latitude": 43.590041,
            "longitude": 39.732886,
            "road_distances": {}
        },
        {
            "type": "Stop",
            "name": "Санаторий Родина",
            "latitude": 43.601202,
            "longitude": 39.715498,
            "road_distances": {}
        }
    ],
    "yellow_pages": {
        "rubrics": {
            "1": {
                "name": "Сквер"
            },
            "2": {
                "name": "Вокзал"
            }
        },
        "companies": [
            {
                "names": [
                    {
                        "value": "Комсомольский"
                    }
                ],
                "rubrics": [
                    1
                ],
                "address": {
                    "coords": {
                        "lat": "43.585745",
                        "lon": "39.716931"
                    }
                },
                "nearby_stops": [
                    {
                        "name": "Ривьерский мост",
                        "meters": 150
                    }
                ]
            },
            {
                "names": [
                    {
                        "value": "Кооперативный"
                    }
                ],
                "rubrics": [
                    1
                ],
                "address": {
                    "coords": {
                        "lat": "43.581303",
                        "lon": "39.720118"
                    }
                },
                "nearby_stops": [
                    {
                        "name": "Морской вокзал",
                        "meters": 15
                    }
                ]
            }
        ]
    }
})";

    const std::string testData2 = R"({
    "serialization_settings": {
        "file": "/tmp/tmp2f3570x9"
    },
    "routing_settings": {
        "bus_wait_time": 2,
        "bus_velocity": 30,
        "pedestrian_velocity": 4
    },
    "render_settings": {
        "width": 1500,
        "height": 950,
        "padding": 50,
        "outer_margin": 150,
        "stop_radius": 3,
        "company_radius": 5,
        "line_width": 10,
        "company_line_width": 2,
        "bus_label_font_size": 18,
        "bus_label_offset": [
            7,
            15
        ],
        "stop_label_font_size": 13,
        "stop_label_offset": [
            7,
            -3
        ],
        "underlayer_color": [
            255,
            255,
            255,
            0.85
        ],
        "underlayer_width": 3,
        "color_palette": [
            "red",
            "green",
            "blue",
            "brown",
            "orange"
        ],
        "layers": [
            "bus_lines",
            "company_lines",
            "bus_labels",
            "stop_points",
            "company_points",
            "stop_labels",
            "company_labels"
        ]
    },
    "base_requests": [
        {
            "type": "Bus",
            "name": "23",
            "stops": [
                "Санаторий Салют",
                "Санаторная улица",
                "Пансионат Нева",
                "Санаторий Радуга",
                "Санаторий Родина",
                "Спортивная",
                "Парк Ривьера",
                "Морской вокзал",
                "Органный зал",
                "Театральная",
                "Пансионат Светлана",
                "Цирк",
                "Стадион",
                "Санаторий Металлург",
                "Улица Бытха"
            ],
            "is_roundtrip": false
        },
        {
            "type": "Bus",
            "name": "13",
            "stops": [
                "Улица Лизы Чайкиной",
                "Пионерская улица, 111",
                "Садовая",
                "Театральная"
            ],
            "is_roundtrip": false
        },
        {
            "type": "Bus",
            "name": "36",
            "stops": [
                "Морской вокзал",
                "Сбербанк",
                "Автовокзал",
                "Отель Звёздный",
                "Магазин Быт",
                "Хлебозавод",
                "Кинотеатр Юбилейный",
                "Новая Заря",
                "Деревообр. комбинат",
                "Целинная улица, 5",
                "Целинная улица, 57",
                "Целинная улица"
            ],
            "is_roundtrip": false
        },
        {
            "type": "Bus",
            "name": "44к",
            "stops": [
                "Пансионат Светлана",
                "Улица Лысая Гора",
                "Улица В. Лысая Гора"
            ],
            "is_roundtrip": false
        },
        {
            "type": "Bus",
            "name": "90",
            "stops": [
                "Краево-Греческая улица",
                "Улица Бытха",
                "Санаторий им. Ворошилова",
                "Санаторий Приморье",
                "Санаторий Заря",
                "Мацеста",
                "Мацестинская долина"
            ],
            "is_roundtrip": false
        },
        {
            "type": "Stop",
            "name": "Краево-Греческая улица",
            "latitude": 43.565551,
            "longitude": 39.776858,
            "road_distances": {
                "Улица Бытха": 1780
            }
        },
        {
            "type": "Stop",
            "name": "Санаторий им. Ворошилова",
            "latitude": 43.557935,
            "longitude": 39.764452,
            "road_distances": {
                "Санаторий Приморье": 950
            }
        },
        {
            "type": "Stop",
            "name": "Санаторий Приморье",
            "latitude": 43.554202,
            "longitude": 39.77256,
            "road_distances": {
                "Санаторий Заря": 2350
            }
        },
        {
            "type": "Stop",
            "name": "Санаторий Заря",
            "latitude": 43.549618,
            "longitude": 39.780908,
            "road_distances": {
                "Мацеста": 800
            }
        },
        {
            "type": "Stop",
            "name": "Мацеста",
            "latitude": 43.545509,
            "longitude": 39.788993,
            "road_distances": {
                "Мацестинская долина": 2350
            }
        },
        {
            "type": "Stop",
            "name": "Мацестинская долина",
            "latitude": 43.560422,
            "longitude": 39.798219,
            "road_distances": {}
        },
        {
            "type": "Stop",
            "name": "Улица Лысая Гора",
            "latitude": 43.577997,
            "longitude": 39.741685,
            "road_distances": {
                "Улица В. Лысая Гора": 640
            }
        },
        {
            "type": "Stop",
            "name": "Улица В. Лысая Гора",
            "latitude": 43.58092,
            "longitude": 39.744749,
            "road_distances": {}
        },
        {
            "type": "Stop",
            "name": "Морской вокзал",
            "latitude": 43.581969,
            "longitude": 39.719848,
            "road_distances": {
                "Сбербанк": 870,
                "Органный зал": 570
            }
        },
        {
            "type": "Stop",
            "name": "Сбербанк",
            "latitude": 43.585969,
            "longitude": 39.725175,
            "road_distances": {
                "Автовокзал": 870
            }
        },
        {
            "type": "Stop",
            "name": "Автовокзал",
            "latitude": 43.592956,
            "longitude": 39.727798,
            "road_distances": {
                "Отель Звёздный": 700
            }
        },
        {
            "type": "Stop",
            "name": "Отель Звёздный",
            "latitude": 43.596585,
            "longitude": 39.721151,
            "road_distances": {
                "Магазин Быт": 1000
            }
        },
        {
            "type": "Stop",
            "name": "Магазин Быт",
            "latitude": 43.604025,
            "longitude": 39.724492,
            "road_distances": {
                "Хлебозавод": 420
            }
        },
        {
            "type": "Stop",
            "name": "Хлебозавод",
            "latitude": 43.607364,
            "longitude": 39.726643,
            "road_distances": {
                "Кинотеатр Юбилейный": 2110
            }
        },
        {
            "type": "Stop",
            "name": "Кинотеатр Юбилейный",
            "latitude": 43.623382,
            "longitude": 39.720626,
            "road_distances": {
                "Новая Заря": 450
            }
        },
        {
            "type": "Stop",
            "name": "Новая Заря",
            "latitude": 43.626842,
            "longitude": 39.717802,
            "road_distances": {
                "Деревообр. комбинат": 530
            }
        },
        {
            "type": "Stop",
            "name": "Деревообр. комбинат",
            "latitude": 43.631035,
            "longitude": 39.714624,
            "road_distances": {
                "Целинная улица, 5": 840
            }
        },
        {
            "type": "Stop",
            "name": "Целинная улица, 5",
            "latitude": 43.633353,
            "longitude": 39.710257,
            "road_distances": {
                "Целинная улица, 57": 1270
            }
        },
        {
            "type": "Stop",
            "name": "Целинная улица, 57",
            "latitude": 43.640536,
            "longitude": 39.713253,
            "road_distances": {
                "Целинная улица": 1050
            }
        },
        {
            "type": "Stop",
            "name": "Целинная улица",
            "latitude": 43.647968,
            "longitude": 39.717733,
            "road_distances": {}
        },
        {
            "type": "Stop",
            "name": "Санаторий Салют",
            "latitude": 43.623238,
            "longitude": 39.704646,
            "road_distances": {
                "Санаторная улица": 1500
            }
        },
        {
            "type": "Stop",
            "name": "Санаторная улица",
            "latitude": 43.620766,
            "longitude": 39.719058,
            "road_distances": {
                "Пансионат Нева": 670
            }
        },
        {
            "type": "Stop",
            "name": "Пансионат Нева",
            "latitude": 43.614288,
            "longitude": 39.718674,
            "road_distances": {
                "Санаторий Радуга": 520
            }
        },
        {
            "type": "Stop",
            "name": "Санаторий Радуга",
            "latitude": 43.609951,
            "longitude": 39.72143,
            "road_distances": {
                "Санаторий Родина": 1190
            }
        },
        {
            "type": "Stop",
            "name": "Санаторий Родина",
            "latitude": 43.601202,
            "longitude": 39.715498,
            "road_distances": {
                "Спортивная": 1100
            }
        },
        {
            "type": "Stop",
            "name": "Спортивная",
            "latitude": 43.593689,
            "longitude": 39.717642,
            "road_distances": {
                "Парк Ривьера": 640
            }
        },
        {
            "type": "Stop",
            "name": "Парк Ривьера",
            "latitude": 43.588296,
            "longitude": 39.715956,
            "road_distances": {
                "Морской вокзал": 730
            }
        },
        {
            "type": "Stop",
            "name": "Органный зал",
            "latitude": 43.57926,
            "longitude": 39.725574,
            "road_distances": {
                "Театральная": 770
            }
        },
        {
            "type": "Stop",
            "name": "Пансионат Светлана",
            "latitude": 43.571807,
            "longitude": 39.735866,
            "road_distances": {
                "Цирк": 520,
                "Улица Лысая Гора": 1070
            }
        },
        {
            "type": "Stop",
            "name": "Цирк",
            "latitude": 43.569207,
            "longitude": 39.739869,
            "road_distances": {
                "Стадион": 860
            }
        },
        {
            "type": "Stop",
            "name": "Стадион",
            "latitude": 43.565301,
            "longitude": 39.749485,
            "road_distances": {
                "Санаторий Металлург": 950
            }
        },
        {
            "type": "Stop",
            "name": "Санаторий Металлург",
            "latitude": 43.561005,
            "longitude": 39.760511,
            "road_distances": {
                "Улица Бытха": 900
            }
        },
        {
            "type": "Stop",
            "name": "Улица Бытха",
            "latitude": 43.566135,
            "longitude": 39.762109,
            "road_distances": {
                "Санаторий им. Ворошилова": 1160
            }
        },
        {
            "type": "Stop",
            "name": "Улица Лизы Чайкиной",
            "latitude": 43.590317,
            "longitude": 39.746833,
            "road_distances": {
                "Пионерская улица, 111": 950
            }
        },
        {
            "type": "Stop",
            "name": "Пионерская улица, 111",
            "latitude": 43.587257,
            "longitude": 39.740325,
            "road_distances": {
                "Садовая": 520
            }
        },
        {
            "type": "Stop",
            "name": "Садовая",
            "latitude": 43.58395,
            "longitude": 39.736938,
            "road_distances": {
                "Театральная": 1300
            }
        },
        {
            "type": "Stop",
            "name": "Театральная",
            "latitude": 43.57471,
            "longitude": 39.731954,
            "road_distances": {
                "Пансионат Светлана": 390
            }
        }
    ],
    "yellow_pages": {
        "rubrics": {
            "1": {
                "name": "Парк"
            }
        },
        "companies": [
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
                        "lat": "43.573226",
                        "lon": "39.742947"
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
                        "type": "PHONE",
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
                        "lat": "43.567998",
                        "lon": "39.734131"
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
        ]
    }
}
)";
}

TEST(FindRouteToCompanyTest, Test1)
{
    const auto [transportDb, yellowPagesDb] = ParseTransportAndYellowPagesDatabases(testData1);
    const std::string stopFrom = "Параллельная улица";
    CompanyRestrictions companyRestrictions;
    companyRestrictions.rubrics.insert("Сквер");
    const auto companies = yellowPagesDb.FindCompanies(companyRestrictions);
    const auto route = transportDb.FindRouteToCompany(stopFrom, companies);
    ASSERT_TRUE(route);

    TransportRouter::RouteInfo expectedRoute = {
        .total_time = 12,
        .items = {
            TransportRouter::RouteInfo::WaitBusItem{.stop_name = "Параллельная улица", .time = 2},
            TransportRouter::RouteInfo::RideBusItem{.bus_name = "24", .time = 2.4, .span_count = 1},
            TransportRouter::RouteInfo::WaitBusItem{.stop_name = "Электросети", .time = 2},
            TransportRouter::RouteInfo::RideBusItem{.bus_name = "14", .time = 3.8, .span_count = 1},
            TransportRouter::RouteInfo::WalkToCompany{.time = 1.8, .stop_name = "Ривьерский мост", .company_name = "Комсомольский"}
        }
    };
    ASSERT_EQ(*route, expectedRoute);
}

TEST(FindRouteToCompanyTest, Test2)
{
    const auto [transportDb, yellowPagesDb] = ParseTransportAndYellowPagesDatabases(testData1);
    const std::string stopFrom = "Морской вокзал";
    CompanyRestrictions companyRestrictions;
    companyRestrictions.names = { "Комсомольский", "Кооперативный" };
    companyRestrictions.rubrics = { "Сквер" };
    const auto companies = yellowPagesDb.FindCompanies(companyRestrictions);
    const auto route = transportDb.FindRouteToCompany(stopFrom, companies);
    ASSERT_TRUE(route);

    TransportRouter::RouteInfo expectedRoute = {
        .total_time = 0.18,
        .items = {
            TransportRouter::RouteInfo::WalkToCompany{.time = 0.18, .stop_name = "Морской вокзал", .company_name = "Кооперативный"}
        }
    };
    ASSERT_EQ(*route, expectedRoute);
}

TEST(FindRouteToCompanyTest, Test3)
{
    const auto [transportDb, yellowPagesDb] = ParseTransportAndYellowPagesDatabases(testData2);
    const std::string stopFrom = "Краево-Греческая улица";
    CompanyRestrictions companyRestrictions;
    companyRestrictions.rubrics.insert("Парк");
    const auto companies = yellowPagesDb.FindCompanies(companyRestrictions);
    const auto route = transportDb.FindRouteToCompany(stopFrom, companies);
    ASSERT_TRUE(route);

    TransportRouter::RouteInfo expectedRoute = {
        .total_time = 15.68,
        .items = {
            TransportRouter::RouteInfo::WaitBusItem{.stop_name = "Краево-Греческая улица", .time = 2},
            TransportRouter::RouteInfo::RideBusItem{.bus_name = "90", .time = 3.56, .span_count = 1},
            TransportRouter::RouteInfo::WaitBusItem{.stop_name = "Улица Бытха", .time = 2},
            TransportRouter::RouteInfo::RideBusItem{.bus_name = "23", .time = 5.42, .span_count = 3},
            TransportRouter::RouteInfo::WalkToCompany{.time = 2.7, .stop_name = "Цирк", .company_name = "Дендрарий"}
        }
    };
    ASSERT_EQ(*route, expectedRoute);
}

TEST(FindRouteToCompanyTest, Test4)
{
    const auto [transportDb, yellowPagesDb] = ParseTransportAndYellowPagesDatabases(testData2);
    const std::string stopFrom = "Цирк";
    CompanyRestrictions companyRestrictions;
    companyRestrictions.rubrics.insert("Парк");
    const auto companies = yellowPagesDb.FindCompanies(companyRestrictions);
    const auto route = transportDb.FindRouteToCompany(stopFrom, companies);
    ASSERT_TRUE(route);

    TransportRouter::RouteInfo expectedRoute = {
        .total_time = 2.7,
        .items = {
            TransportRouter::RouteInfo::WalkToCompany{.time = 2.7, .stop_name = "Цирк", .company_name = "Дендрарий"}
        }
    };
    ASSERT_EQ(*route, expectedRoute);
}