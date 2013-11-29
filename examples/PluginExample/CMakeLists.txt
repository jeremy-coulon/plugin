cmake_minimum_required(VERSION 2.8)

option(Plugin_BUILD_PLUGIN_EXAMPLE "Build PluginExample" ${BUILD_ALL})

if(Plugin_BUILD_PLUGIN_EXAMPLE)

    project(PluginExample CXX)

    ##################
    #  Dependencies  #
    ##################

    find_package(Boost REQUIRED)
    mark_as_advanced(Boost_DIR)

    if(NOT TARGET Versionning)
        set(Versionning_HINT ${Plugin_SOURCE_DIR}/../Versionning/build/share/cmake)
        find_package(Versionning
            REQUIRED
            PATHS ${Versionning_HINT}
        )
    endif()

    #######################
    #  Compilation flags  #
    #######################

    include_directories(
        ${Plugin_INCLUDE_DIR}
        ${Versionning_INCLUDE_DIR}
        ${Boost_INCLUDE_DIR}
    )

    ############
    #  Target  #
    ############

    add_library(${PROJECT_NAME}
        SHARED
        ${PROJECT_SOURCE_DIR}/src/MyPlugin.cpp
        ${PROJECT_SOURCE_DIR}/src/MyPlugin.h
    )

    ###############
    #  Packaging  #
    ###############

    install(
        TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT test
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT test
    )

endif()
