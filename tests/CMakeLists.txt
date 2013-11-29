cmake_minimum_required(VERSION 2.8)

option(Plugin_BUILD_TEST "Build tests" ${BUILD_ALL})

if(Plugin_BUILD_TEST)

    project(TestPlugin CXX)

    ##################
    #  Dependencies  #
    ##################

    find_package(Boost REQUIRED unit_test_framework filesystem system)
    mark_as_advanced(Boost_DIR)

    if(NOT TARGET PluginExample)
        message(FATAL_ERROR "Target not found: PluginExample")
    endif()

    get_target_property(
        PluginExample_LOCATION
        PluginExample
        LOCATION
    )

    #############
    #  Sources  #
    #############

    set(PROJECT_SRC_DIR
        ${PROJECT_SOURCE_DIR}/src
    )

    set(PROJECT_FILES
        ${PROJECT_SRC_DIR}/main.cpp
        ${PROJECT_SRC_DIR}/test.cpp
    )

    #######################
    #  Compilation flags  #
    #######################

    include_directories(
        ${Plugin_INCLUDE_DIR}
        ${Versionning_INCLUDE_DIR}
        ${Boost_INCLUDE_DIR}
    )

    add_definitions(
        -DBOOST_TEST_DYN_LINK
        -DMYPLUGIN_PATH="${PluginExample_LOCATION}"
    )

    ############
    #  Target  #
    ############

    add_executable(${PROJECT_NAME} ${PROJECT_FILES})

    target_link_libraries(${PROJECT_NAME}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${CMAKE_DL_LIBS}
    )

    add_dependencies(${PROJECT_NAME} PluginExample)

    ###############
    #  Packaging  #
    ###############

    install(
        TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT test
    )

endif()
