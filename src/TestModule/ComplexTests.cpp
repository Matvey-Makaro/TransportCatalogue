#include <string>
#include <sstream>
#include <fstream>
#include "Program.h"
#include "ComplexTests.h"

namespace
{

void Example1()
{
    std::string input = R"({
    "routing_settings": {
        "bus_wait_time": 2,
        "bus_velocity": 30
    },
    "render_settings": {
        "width": 1200,
        "height": 1200,
        "padding": 50,
        "stop_radius": 5,
        "line_width": 14,
        "bus_label_font_size": 20,
        "bus_label_offset": [
            7,
            15
        ],
        "stop_label_font_size": 20,
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
            "stop_points",
            "bus_labels"
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
    "stat_requests": [
        {
            "id": 826874078,
            "type": "Bus",
            "name": "14"
        },
        {
            "id": 1086967114,
            "type": "Route",
            "from": "Морской вокзал",
            "to": "Параллельная улица"
        },
        {
            "id": 1218663236,
            "type": "Map"
        }
    ]
}
)";
    std::stringstream in(input);
    // std::stringstream out;
    std::fstream out;
    out.open("Example1.out.txt", std::ios_base::out);
    Program program;
    program.Run(in, out);

//     std::string result = out.str();
//     std::string expectedResult = R"([{"request_id": 826874078, "stop_count": 8, "unique_stop_count": 7, "route_length": 11230, "curvature": 1.6048112037594995}, {"request_id": 1086967114, "total_time": 15.959999999999997, "items": [{"type": "Wait", "stop_name": "Морской вокзал", "time": 2}, {"type": "Bus", "bus": "114", "time": 1.7, "span_count": 1}, {"type": "Wait", "stop_name": "Ривьерский мост", "time": 2}, {"type": "Bus", "bus": "14", "time": 6.06, "span_count": 4}, {"type": "Wait", "stop_name": "Улица Докучаева", "time": 2}, {"type": "Bus", "bus": "24", "time": 2.2, "span_count": 1}]}, {"request_id": 1218663236, "map": "<?xml version=\"1.0\" encoding=\"UTF-8\" ?><svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\"><polyline points=\"202.7046433700865,725.1651507897237 99.25163555140999,520.6462422211027 202.7046433700865,725.1651507897237\" fill=\"none\" stroke=\"green\" stroke-width=\"14\" stroke-linecap=\"round\" stroke-linejoin=\"round\" /><polyline points=\"1150.0,432.1126535822507 580.955800223462,137.79639380890507 99.25163555140999,520.6462422211027 491.26376256587264,861.7217169297415 592.7509175043582,846.6267751715245 690.4467847454753,819.3856709748779 695.2561033988173,598.1921174405119 1150.0,432.1126535822507\" fill=\"none\" stroke=\"rgb(255,160,0)\" stroke-width=\"14\" stroke-linecap=\"round\" stroke-linejoin=\"round\" /><polyline points=\"695.2561033988173,598.1921174405119 660.3973192915555,441.8014999201921 580.955800223462,137.79639380890507 50.0,50.0 580.955800223462,137.79639380890507 660.3973192915555,441.8014999201921 695.2561033988173,598.1921174405119\" fill=\"none\" stroke=\"red\" stroke-width=\"14\" stroke-linecap=\"round\" stroke-linejoin=\"round\" /><circle cx=\"491.26376256587264\" cy=\"861.7217169297415\" r=\"5\" fill=\"white\" stroke=\"none\" stroke-width=\"1\" /><circle cx=\"592.7509175043582\" cy=\"846.6267751715245\" r=\"5\" fill=\"white\" stroke=\"none\" stroke-width=\"1\" /><circle cx=\"202.7046433700865\" cy=\"725.1651507897237\" r=\"5\" fill=\"white\" stroke=\"none\" stroke-width=\"1\" /><circle cx=\"660.3973192915555\" cy=\"441.8014999201921\" r=\"5\" fill=\"white\" stroke=\"none\" stroke-width=\"1\" /><circle cx=\"690.4467847454753\" cy=\"819.3856709748779\" r=\"5\" fill=\"white\" stroke=\"none\" stroke-width=\"1\" /><circle cx=\"99.25163555140999\" cy=\"520.6462422211027\" r=\"5\" fill=\"white\" stroke=\"none\" stroke-width=\"1\" /><circle cx=\"50.0\" cy=\"50.0\" r=\"5\" fill=\"white\" stroke=\"none\" stroke-width=\"1\" /><circle cx=\"695.2561033988173\" cy=\"598.1921174405119\" r=\"5\" fill=\"white\" stroke=\"none\" stroke-width=\"1\" /><circle cx=\"1150.0\" cy=\"432.1126535822507\" r=\"5\" fill=\"white\" stroke=\"none\" stroke-width=\"1\" /><circle cx=\"580.955800223462\" cy=\"137.79639380890507\" r=\"5\" fill=\"white\" stroke=\"none\" stroke-width=\"1\" /><text x=\"491.26376256587264\" y=\"861.7217169297415\" dx=\"7\" dy=\"-3\" font-size=\"20\" font-family=\"Verdana\" fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" >Гостиница Сочи</text><text x=\"491.26376256587264\" y=\"861.7217169297415\" dx=\"7\" dy=\"-3\" font-size=\"20\" font-family=\"Verdana\" fill=\"black\" stroke=\"none\" stroke-width=\"1\" >Гостиница Сочи</text><text x=\"592.7509175043582\" y=\"846.6267751715245\" dx=\"7\" dy=\"-3\" font-size=\"20\" font-family=\"Verdana\" fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" >Кубанская улица</text><text x=\"592.7509175043582\" y=\"846.6267751715245\" dx=\"7\" dy=\"-3\" font-size=\"20\" font-family=\"Verdana\" fill=\"black\" stroke=\"none\" stroke-width=\"1\" >Кубанская улица</text><text x=\"202.7046433700865\" y=\"725.1651507897237\" dx=\"7\" dy=\"-3\" font-size=\"20\" font-family=\"Verdana\" fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" >Морской вокзал</text><text x=\"202.7046433700865\" y=\"725.1651507897237\" dx=\"7\" dy=\"-3\" font-size=\"20\" font-family=\"Verdana\" fill=\"black\" stroke=\"none\" stroke-width=\"1\" >Морской вокзал</text><text x=\"660.3973192915555\" y=\"441.8014999201921\" dx=\"7\" dy=\"-3\" font-size=\"20\" font-family=\"Verdana\" fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" >Параллельная улица</text><text x=\"660.3973192915555\" y=\"441.8014999201921\" dx=\"7\" dy=\"-3\" font-size=\"20\" font-family=\"Verdana\" fill=\"black\" stroke=\"none\" stroke-width=\"1\" >Параллельная улица</text><text x=\"690.4467847454753\" y=\"819.3856709748779\" dx=\"7\" dy=\"-3\" font-size=\"20\" font-family=\"Verdana\" fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" >По требованию</text><text x=\"690.4467847454753\" y=\"819.3856709748779\" dx=\"7\" dy=\"-3\" font-size=\"20\" font-family=\"Verdana\" fill=\"black\" stroke=\"none\" stroke-width=\"1\" >По требованию</text><text x=\"99.25163555140999\" y=\"520.6462422211027\" dx=\"7\" dy=\"-3\" font-size=\"20\" font-family=\"Verdana\" fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" >Ривьерский мост</text><text x=\"99.25163555140999\" y=\"520.6462422211027\" dx=\"7\" dy=\"-3\" font-size=\"20\" font-family=\"Verdana\" fill=\"black\" stroke=\"none\" stroke-width=\"1\" >Ривьерский мост</text><text x=\"50.0\" y=\"50.0\" dx=\"7\" dy=\"-3\" font-size=\"20\" font-family=\"Verdana\" fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" >Санаторий Родина</text><text x=\"50.0\" y=\"50.0\" dx=\"7\" dy=\"-3\" font-size=\"20\" font-family=\"Verdana\" fill=\"black\" stroke=\"none\" stroke-width=\"1\" >Санаторий Родина</text><text x=\"695.2561033988173\" y=\"598.1921174405119\" dx=\"7\" dy=\"-3\" font-size=\"20\" font-family=\"Verdana\" fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" >Улица Докучаева</text><text x=\"695.2561033988173\" y=\"598.1921174405119\" dx=\"7\" dy=\"-3\" font-size=\"20\" font-family=\"Verdana\" fill=\"black\" stroke=\"none\" stroke-width=\"1\" >Улица Докучаева</text><text x=\"1150.0\" y=\"432.1126535822507\" dx=\"7\" dy=\"-3\" font-size=\"20\" font-family=\"Verdana\" fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" >Улица Лизы Чайкиной</text><text x=\"1150.0\" y=\"432.1126535822507\" dx=\"7\" dy=\"-3\" font-size=\"20\" font-family=\"Verdana\" fill=\"black\" stroke=\"none\" stroke-width=\"1\" >Улица Лизы Чайкиной</text><text x=\"580.955800223462\" y=\"137.79639380890507\" dx=\"7\" dy=\"-3\" font-size=\"20\" font-family=\"Verdana\" fill=\"rgba(255,255,255,0.85)\" stroke=\"rgba(255,255,255,0.85)\" stroke-width=\"3\" stroke-linecap=\"round\" stroke-linejoin=\"round\" >Электросети</text><text x=\"580.955800223462\" y=\"137.79639380890507\" dx=\"7\" dy=\"-3\" font-size=\"20\" font-family=\"Verdana\" fill=\"black\" stroke=\"none\" stroke-width=\"1\" >Электросети</text></svg>"}]
// )";
//     ASSERT_EQUAL(result, expectedResult);
}

}

void Example2()
{
    std::string input = R"({
    "stat_requests": [
        {
            "type": "Map",
            "id": 1821171961
        }
    ],
    "render_settings": {
        "bus_label_font_size": 18,
        "padding": 50,
        "height": 950,
        "line_width": 10,
        "underlayer_color": [
            255,
            255,
            255,
            0.85
        ],
        "underlayer_width": 3,
        "stop_label_font_size": 13,
        "stop_radius": 3,
        "width": 1500,
        "bus_label_offset": [
            7,
            15
        ],
        "color_palette": [
            "red",
            "green",
            "blue",
            "brown",
            "orange"
        ],
        "stop_label_offset": [
            7,
            -3
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
            "name": "23",
            "type": "Bus",
            "is_roundtrip": false
        },
        {
            "stops": [
                "Улица Лизы Чайкиной",
                "Пионерская улица, 111",
                "Садовая",
                "Театральная"
            ],
            "name": "13",
            "type": "Bus",
            "is_roundtrip": false
        },
        {
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
            "name": "36",
            "type": "Bus",
            "is_roundtrip": false
        },
        {
            "stops": [
                "Пансионат Светлана",
                "Улица Лысая Гора",
                "Улица В. Лысая Гора"
            ],
            "name": "44к",
            "type": "Bus",
            "is_roundtrip": false
        },
        {
            "stops": [
                "Краево-Греческая улица",
                "Улица Бытха",
                "Санаторий им. Ворошилова",
                "Санаторий Приморье",
                "Санаторий Заря",
                "Мацеста",
                "Мацестинская долина"
            ],
            "name": "90",
            "type": "Bus",
            "is_roundtrip": false
        },
        {
            "name": "Краево-Греческая улица",
            "latitude": 43.565551,
            "type": "Stop",
            "longitude": 39.776858,
            "road_distances": {
                "Улица Бытха": 1780
            }
        },
        {
            "name": "Санаторий им. Ворошилова",
            "latitude": 43.557935,
            "type": "Stop",
            "longitude": 39.764452,
            "road_distances": {
                "Санаторий Приморье": 950
            }
        },
        {
            "name": "Санаторий Приморье",
            "latitude": 43.554202,
            "type": "Stop",
            "longitude": 39.77256,
            "road_distances": {
                "Санаторий Заря": 2350
            }
        },
        {
            "name": "Санаторий Заря",
            "latitude": 43.549618,
            "type": "Stop",
            "longitude": 39.780908,
            "road_distances": {
                "Мацеста": 800
            }
        },
        {
            "name": "Мацеста",
            "latitude": 43.545509,
            "type": "Stop",
            "longitude": 39.788993,
            "road_distances": {
                "Мацестинская долина": 2350
            }
        },
        {
            "name": "Мацестинская долина",
            "latitude": 43.560422,
            "type": "Stop",
            "longitude": 39.798219,
            "road_distances": {}
        },
        {
            "name": "Улица Лысая Гора",
            "latitude": 43.577997,
            "type": "Stop",
            "longitude": 39.741685,
            "road_distances": {
                "Улица В. Лысая Гора": 640
            }
        },
        {
            "name": "Улица В. Лысая Гора",
            "latitude": 43.58092,
            "type": "Stop",
            "longitude": 39.744749,
            "road_distances": {}
        },
        {
            "name": "Морской вокзал",
            "latitude": 43.581969,
            "type": "Stop",
            "longitude": 39.719848,
            "road_distances": {
                "Сбербанк": 870,
                "Органный зал": 570
            }
        },
        {
            "name": "Сбербанк",
            "latitude": 43.585969,
            "type": "Stop",
            "longitude": 39.725175,
            "road_distances": {
                "Автовокзал": 870
            }
        },
        {
            "name": "Автовокзал",
            "latitude": 43.592956,
            "type": "Stop",
            "longitude": 39.727798,
            "road_distances": {
                "Отель Звёздный": 700
            }
        },
        {
            "name": "Отель Звёздный",
            "latitude": 43.596585,
            "type": "Stop",
            "longitude": 39.721151,
            "road_distances": {
                "Магазин Быт": 1000
            }
        },
        {
            "name": "Магазин Быт",
            "latitude": 43.604025,
            "type": "Stop",
            "longitude": 39.724492,
            "road_distances": {
                "Хлебозавод": 420
            }
        },
        {
            "name": "Хлебозавод",
            "latitude": 43.607364,
            "type": "Stop",
            "longitude": 39.726643,
            "road_distances": {
                "Кинотеатр Юбилейный": 2110
            }
        },
        {
            "name": "Кинотеатр Юбилейный",
            "latitude": 43.623382,
            "type": "Stop",
            "longitude": 39.720626,
            "road_distances": {
                "Новая Заря": 450
            }
        },
        {
            "name": "Новая Заря",
            "latitude": 43.626842,
            "type": "Stop",
            "longitude": 39.717802,
            "road_distances": {
                "Деревообр. комбинат": 530
            }
        },
        {
            "name": "Деревообр. комбинат",
            "latitude": 43.631035,
            "type": "Stop",
            "longitude": 39.714624,
            "road_distances": {
                "Целинная улица, 5": 840
            }
        },
        {
            "name": "Целинная улица, 5",
            "latitude": 43.633353,
            "type": "Stop",
            "longitude": 39.710257,
            "road_distances": {
                "Целинная улица, 57": 1270
            }
        },
        {
            "name": "Целинная улица, 57",
            "latitude": 43.640536,
            "type": "Stop",
            "longitude": 39.713253,
            "road_distances": {
                "Целинная улица": 1050
            }
        },
        {
            "name": "Целинная улица",
            "latitude": 43.647968,
            "type": "Stop",
            "longitude": 39.717733,
            "road_distances": {}
        },
        {
            "name": "Санаторий Салют",
            "latitude": 43.623238,
            "type": "Stop",
            "longitude": 39.704646,
            "road_distances": {
                "Санаторная улица": 1500
            }
        },
        {
            "name": "Санаторная улица",
            "latitude": 43.620766,
            "type": "Stop",
            "longitude": 39.719058,
            "road_distances": {
                "Пансионат Нева": 670
            }
        },
        {
            "name": "Пансионат Нева",
            "latitude": 43.614288,
            "type": "Stop",
            "longitude": 39.718674,
            "road_distances": {
                "Санаторий Радуга": 520
            }
        },
        {
            "name": "Санаторий Радуга",
            "latitude": 43.609951,
            "type": "Stop",
            "longitude": 39.72143,
            "road_distances": {
                "Санаторий Родина": 1190
            }
        },
        {
            "name": "Санаторий Родина",
            "latitude": 43.601202,
            "type": "Stop",
            "longitude": 39.715498,
            "road_distances": {
                "Спортивная": 1100
            }
        },
        {
            "name": "Спортивная",
            "latitude": 43.593689,
            "type": "Stop",
            "longitude": 39.717642,
            "road_distances": {
                "Парк Ривьера": 640
            }
        },
        {
            "name": "Парк Ривьера",
            "latitude": 43.588296,
            "type": "Stop",
            "longitude": 39.715956,
            "road_distances": {
                "Морской вокзал": 730
            }
        },
        {
            "name": "Органный зал",
            "latitude": 43.57926,
            "type": "Stop",
            "longitude": 39.725574,
            "road_distances": {
                "Театральная": 770
            }
        },
        {
            "name": "Пансионат Светлана",
            "latitude": 43.571807,
            "type": "Stop",
            "longitude": 39.735866,
            "road_distances": {
                "Цирк": 520,
                "Улица Лысая Гора": 1070
            }
        },
        {
            "name": "Цирк",
            "latitude": 43.569207,
            "type": "Stop",
            "longitude": 39.739869,
            "road_distances": {
                "Стадион": 860
            }
        },
        {
            "name": "Стадион",
            "latitude": 43.565301,
            "type": "Stop",
            "longitude": 39.749485,
            "road_distances": {
                "Санаторий Металлург": 950
            }
        },
        {
            "name": "Санаторий Металлург",
            "latitude": 43.561005,
            "type": "Stop",
            "longitude": 39.760511,
            "road_distances": {
                "Улица Бытха": 900
            }
        },
        {
            "name": "Улица Бытха",
            "latitude": 43.566135,
            "type": "Stop",
            "longitude": 39.762109,
            "road_distances": {
                "Санаторий им. Ворошилова": 1160
            }
        },
        {
            "name": "Улица Лизы Чайкиной",
            "latitude": 43.590317,
            "type": "Stop",
            "longitude": 39.746833,
            "road_distances": {
                "Пионерская улица, 111": 950
            }
        },
        {
            "name": "Пионерская улица, 111",
            "latitude": 43.587257,
            "type": "Stop",
            "longitude": 39.740325,
            "road_distances": {
                "Садовая": 520
            }
        },
        {
            "name": "Садовая",
            "latitude": 43.58395,
            "type": "Stop",
            "longitude": 39.736938,
            "road_distances": {
                "Театральная": 1300
            }
        },
        {
            "name": "Театральная",
            "latitude": 43.57471,
            "type": "Stop",
            "longitude": 39.731954,
            "road_distances": {
                "Пансионат Светлана": 390
            }
        }
    ],
    "routing_settings": {
        "bus_wait_time": 2,
        "bus_velocity": 30
    }
}
)";
    std::stringstream in(input);
    // std::stringstream out;
    std::fstream out;
    out.open("Example2.out.txt", std::ios_base::out);
    Program program;
    program.Run(in, out);
}

void RunComplexTests(const TestRunner& tr)
{
    RUN_TEST(tr, Example1);
    RUN_TEST(tr, Example2);
}
