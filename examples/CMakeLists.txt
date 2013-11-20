cmake_minimum_required(VERSION 2.8)

option(Plugin_BUILD_EXAMPLE "Build examples" OFF)

if(Plugin_BUILD_EXAMPLE)

    add_subdirectory(PluginExample)
    add_subdirectory(PluginLoaderExample)

    add_dependencies(PluginLoaderExample PluginExample)

endif()
