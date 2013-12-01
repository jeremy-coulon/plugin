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
        plugin_path
        PluginExample
        LOCATION
    )
    get_target_property(
        plugin_dir
        PluginExample
        LIBRARY_OUTPUT_DIRECTORY
    )
    file(RELATIVE_PATH plugin_filename ${plugin_dir} ${plugin_path})

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
        -DMYPLUGIN_PATH="${plugin_filename}"
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

    set_target_properties(${PROJECT_NAME}
        PROPERTIES
        BUILD_WITH_INSTALL_RPATH ON
    )

    add_dependencies(${PROJECT_NAME} PluginExample)

    #############
    #  Testing  #
    #############

    add_test(${PROJECT_NAME} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME})

    ###############
    #  Packaging  #
    ###############

    install(
        TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT test
    )

endif()
