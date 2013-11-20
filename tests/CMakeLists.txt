cmake_minimum_required(VERSION 2.8)

include(CMakeDependentOption)

CMAKE_DEPENDENT_OPTION(Plugin_BUILD_TEST
    "Build tests"
    OFF
    "Plugin_BUILD_EXAMPLE"
    OFF
)

if(Plugin_BUILD_TEST)

    project(TestPlugin CXX)

    find_package(Boost REQUIRED unit_test_framework filesystem system)
    if(NOT TARGET Versionning)
        find_package(Versionning REQUIRED)
    endif()

    if(NOT TARGET PluginExample)
        message(FATAL_ERROR "Plugin_BUILD_EXAMPLE must be enabled in order to execute tests.")
    endif()

    get_target_property(
        PluginExample_LOCATION
        PluginExample
        LOCATION
    )

    set(${PROJECT_NAME}_FILES
        ${PROJECT_SOURCE_DIR}/main.cpp
        ${PROJECT_SOURCE_DIR}/test.cpp
    )

    include_directories(
        ${Plugin_INCLUDE_DIR}
        ${Versionning_INCLUDE_DIR}
        ${Boost_INCLUDE_DIR}
    )

    add_definitions(
        -DBOOST_TEST_DYN_LINK
        -DMYPLUGIN_PATH="${PluginExample_LOCATION}"
    )

    add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_FILES})

    target_link_libraries(${PROJECT_NAME}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${CMAKE_DL_LIBS}
    )

    add_dependencies(${PROJECT_NAME} PluginExample)

endif()
