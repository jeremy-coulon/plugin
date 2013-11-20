cmake_minimum_required(VERSION 2.8)

project(PluginExample CXX)

find_package(Boost REQUIRED)
mark_as_advanced(Boost_DIR)

if(NOT TARGET Versionning)
    find_package(Versionning REQUIRED)
endif()

include_directories(
    ${Plugin_INCLUDE_DIR}
    ${Versionning_INCLUDE_DIR}
    ${Boost_INCLUDE_DIR}
)

add_library(${PROJECT_NAME}
    SHARED
    ${PROJECT_SOURCE_DIR}/src/MyPlugin.cpp
    ${PROJECT_SOURCE_DIR}/src/MyPlugin.h
)
