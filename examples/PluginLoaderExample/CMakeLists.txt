cmake_minimum_required(VERSION 2.8)

option(Plugin_BUILD_PLUGINLOADER_EXAMPLE "Build PluginLoaderExample" ${BUILD_ALL})

if(Plugin_BUILD_PLUGINLOADER_EXAMPLE)

    project(PluginLoaderExample CXX)

    ##################
    #  Dependencies  #
    ##################

    if(NOT TARGET PluginExample)
        message(FATAL_ERROR "Target not found: PluginExample")
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

    add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCE_DIR}/src/main.cpp)

    target_link_libraries(${PROJECT_NAME}
        ${CMAKE_DL_LIBS}
    )

    add_dependencies(${PROJECT_NAME} PluginExample)

endif()
