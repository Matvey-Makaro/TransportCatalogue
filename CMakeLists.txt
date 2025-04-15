cmake_minimum_required(VERSION 3.16)

project(TransportCatalogue LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(TestMode "Enable test mode" OFF)
option(ManualTests "Enable manual tests" OFF)
# set(TestMode ON)
if(TestMode)
    add_definitions(-DOnlyMap)
    add_definitions(-DNoEscapeNeeded)    
endif()

if(ManualTests)
    add_definitions(-DRunManualTests)
endif()


set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/")
include_directories(${SOURCE_DIR})

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(ProtoFiles
    src/proto/TransportCatalog/Point.proto
    src/proto/TransportCatalog/Stop.proto
    src/proto/TransportCatalog/Bus.proto
    src/proto/TransportCatalog/RoutingSettings.proto
    src/proto/TransportCatalog/RenderSettings.proto
    src/proto/YellowPages/sphere.proto
    src/proto/YellowPages/url.proto
    src/proto/YellowPages/working_time.proto
    src/proto/YellowPages/rubric.proto
    src/proto/YellowPages/phone.proto
    src/proto/YellowPages/name.proto
    src/proto/YellowPages/address.proto
    src/proto/YellowPages/company.proto
    src/proto/YellowPages/database.proto
    src/proto/TransportCatalog/transport_catalog.proto
    )

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${ProtoFiles})

set(CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})
file(GLOB_RECURSE Sources "${SOURCE_DIR}/*.cpp" "${SOURCE_DIR}/*.h")
list(FILTER Sources EXCLUDE REGEX ".*main\.cpp$")
set(MainCPP src/main.cpp)

add_library(TransportCatalogueCore STATIC ${Sources} ${PROTO_SRCS} ${PROTO_HDRS})
add_executable(TransportCatalogue ${MainCPP})
target_link_libraries(TransportCatalogue ${Protobuf_LIBRARIES} TransportCatalogueCore)



enable_testing()
set(googletest-version "1.16.0")
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY "https://github.com/google/googletest.git"
  GIT_TAG "v${googletest-version}"
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE TestFiles "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.h")
add_executable(TransportCatalogueTests ${TestFiles})
target_include_directories(TransportCatalogueTests PRIVATE ${gtest_SOURCE_DIR}/include ${gtest_BINARY_DIR} ${gmock_SOURCE_DIR}/include)
target_link_libraries(TransportCatalogueTests TransportCatalogueCore ${Protobuf_LIBRARIES} GTest::gtest_main gmock)

message(${gmock_SOURCE_DIR}/include)

include(GoogleTest)
include(GNUInstallDirs)
install(TARGETS TransportCatalogueCore TransportCatalogue TransportCatalogueTests
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
gtest_discover_tests(TransportCatalogueTests)