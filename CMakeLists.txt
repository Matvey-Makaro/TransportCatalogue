cmake_minimum_required(VERSION 3.16)

project(TransportCatalogue LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(TestMode "Enable test mode" OFF)
set(TestMode ON)
if(TestMode)
    add_definitions(-DRunTests)
    add_definitions(-DOnlyMap)
    add_definitions(-DNoEscapeNeeded)    
endif()

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/")
include_directories(${SOURCE_DIR})

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(ProtoFiles
    src/proto/Point.proto
    src/proto/Stop.proto
    src/proto/Bus.proto
    src/proto/transport_catalog.proto
)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${ProtoFiles})

set(CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})
file(GLOB_RECURSE Source "${SOURCE_DIR}/*.cpp" "${SOURCE_DIR}/*.h")
add_executable(TransportCatalogue ${Source} ${PROTO_SRCS} ${PROTO_HDRS})

target_link_libraries(TransportCatalogue ${Protobuf_LIBRARIES})

include(GNUInstallDirs)
install(TARGETS TransportCatalogue
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
