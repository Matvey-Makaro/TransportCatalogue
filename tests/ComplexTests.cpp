#include <gtest/gtest.h>
#include <string>
#include <sstream>
#include <fstream>
#include "Program.h"

TEST(ComplexManualTest, Test1)
{
    // TODO: #if for skip
    // GTEST_SKIP();

    std::string makeBaseInput = R"({
    "serialization_settings": {
        "file": "serialization_settings.bin"
    },
    "routing_settings": {
        "bus_wait_time": 2,
        "bus_velocity": 30
    },
    "render_settings": {
        "width": 1200,
        "height": 500,
        "padding": 50,
        "outer_margin": 150,
        "stop_radius": 5,
        "line_width": 14,
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
            "bus_labels",
            "stop_points",
            "stop_labels"
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
                ]
            }
        ]
    }
}
)";
    std::stringstream makeBastIn(makeBaseInput);
    // std::stringstream out;
    std::fstream out;
    out.open("Example1.svg", std::ios_base::out);

    Program makeBase("make_base");
    makeBase.Run(makeBastIn, out);

    std::string processRequestsInput = R"({
    "serialization_settings": {
        "file": "serialization_settings.bin"
    },
    "stat_requests": [
        {
            "id": 379575119,
            "type": "FindCompanies",
            "rubrics": [
                "Сквер"
            ]
        },
        {
            "id": 702762979,
            "type": "FindCompanies",
            "rubrics": [
                "Вокзал"
            ]
        },
        {
            "id": 7543138,
            "type": "FindCompanies",
            "names": [
                "Комсомольский"
            ]
        },
        {
            "id": 162389115,
            "type": "FindCompanies",
            "names": [
                "Комсомольский"
            ],
            "rubrics": [
                "Вокзал"
            ]
        },
        {
            "id": 279616443,
            "type": "FindCompanies",
            "names": [
                "Комсомольский"
            ],
            "rubrics": [
                "Сквер"
            ]
        },
        {
            "id": 675139528,
            "type": "FindCompanies",
            "names": [
                "Комсомольский",
                "Кооперативный"
            ],
            "rubrics": [
                "Сквер"
            ]
        },
        {
            "id": 1374027080,
            "type": "FindCompanies",
            "names": [
                "Комсомольский",
                "Кооперативный"
            ],
            "rubrics": [
                "Сквер",
                "Вокзал"
            ]
        }
    ]
}
)";
    std::stringstream processRequestsIn(processRequestsInput);
    Program processRequests("process_requests");
    processRequests.Run(processRequestsIn, out);
}

TEST(ComplexManualTest, Test2)
{
    // TODO: #if for skip
    // GTEST_SKIP();

    std::string makeBaseInput = R"({
    "serialization_settings": {
        "file": "serialization_settings.bin"
    },
    "routing_settings": {
        "bus_wait_time": 2,
        "bus_velocity": 30
    },
    "render_settings": {
        "width": 1500,
        "height": 950,
        "padding": 50,
        "outer_margin": 150,
        "stop_radius": 3,
        "line_width": 10,
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
            "bus_labels",
            "stop_points",
            "stop_labels"
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
    std::stringstream makeBastIn(makeBaseInput);
    // std::stringstream out;
    std::fstream out;
    out.open("Example2.svg", std::ios_base::out);

    Program makeBase("make_base");
    makeBase.Run(makeBastIn, out);

    std::string processRequestsInput = R"({
    "serialization_settings": {
        "file": "serialization_settings.bin"
    },
    "stat_requests": [
        {
            "id": 1297727706,
            "type": "Bus",
            "name": "13"
        },
        {
            "id": 749458699,
            "type": "Stop",
            "name": "Морской вокзал"
        },
        {
            "id": 2053647165,
            "type": "FindCompanies",
            "rubrics": [
                "Парк"
            ]
        },
        {
            "id": 866281987,
            "type": "FindCompanies",
            "rubrics": [
                "Парк"
            ],
            "phones": [
                {
                    "number": "2671646"
                }
            ]
        },
        {
            "id": 1306125210,
            "type": "FindCompanies",
            "phones": [
                {
                    "type": "PHONE",
                    "number": "2671646"
                }
            ]
        },
        {
            "id": 589984419,
            "type": "FindCompanies",
            "phones": [
                {
                    "type": "PHONE"
                }
            ]
        }
    ]
}
)";
    std::stringstream processRequestsIn(processRequestsInput);
    Program processRequests("process_requests");
    processRequests.Run(processRequestsIn, out);
}

TEST(ComplexManualTest, Test3)
{
    // TODO: #if for skip
    // GTEST_SKIP();

    std::string makeBaseInput = R"({
    "serialization_settings": {
        "file": "serialization_settings.bin"
    },
    "routing_settings": {
        "bus_wait_time": 2,
        "bus_velocity": 30
    },
    "render_settings": {
        "width": 1200,
        "height": 500,
        "padding": 50,
        "outer_margin": 150,
        "stop_radius": 5,
        "line_width": 14,
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
            "bus_labels",
            "stop_points",
            "stop_labels"
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
                "name": "Park"
            }
        },
        "companies": [
            {
                "names": [
                    {
                        "value": "Dendrariy"
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
                        "name": "Tsirk",
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
                        "value": "im. Frunze"
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
                        "name": "Pansionat Svetlana",
                        "meters": 580
                    },
                    {
                        "name": "Tsirk",
                        "meters": 700
                    },
                    {
                        "name": "Teatral'naya",
                        "meters": 1000
                    }
                ]
            }
        ]
    }
}
)";
    std::stringstream makeBastIn(makeBaseInput);
    std::fstream out;
    out.open("Example3.svg", std::ios_base::out);

    Program makeBase("make_base");
    makeBase.Run(makeBastIn, out);
}
