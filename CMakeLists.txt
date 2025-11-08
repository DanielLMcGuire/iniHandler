cmake_minimum_required(VERSION 3.20)
project (iniHandler LANGUAGES CXX)

file(GLOB SOURCES
    "${CMAKE_CURRENT_LIST_DIR}/*.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/*.h"
)

add_library(DConsole STATIC ${SOURCES})

target_include_directories(iniHandler PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_compile_features (iniHandler PUBLIC cxx_std_20)

if(NOT SOURCES)
    message(WARNING "No sources found in iniHandler blob!")
endif()
