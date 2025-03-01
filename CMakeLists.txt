cmake_minimum_required(VERSION 3.16)

project(TransportCatalogue LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(TestMode "Enable test mode" OFF)
# set(TestMode ON)
if(TestMode)
    add_definitions(-DRunTests)
    add_definitions(-DOnlyMap)
    add_definitions(-DNoEscapeNeeded)    
    set(EscapeNeeded OFF)
endif()


set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/")
include_directories(${SOURCE_DIR})

set(CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR})
file(GLOB_RECURSE Source "${SOURCE_DIR}/*.cpp" "${SOURCE_DIR}/*.h")
add_executable(TransportCatalogue ${Source})

include(GNUInstallDirs)
install(TARGETS TransportCatalogue
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
