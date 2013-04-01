cmake_minimum_required(VERSION 2.8)

option(Plugin_BUILD_EXAMPLE "Build examples" OFF)

if(Plugin_BUILD_EXAMPLE)

    project(PluginExample CXX)

    add_subdirectory(MyPlugin)
    add_subdirectory(MyExe)

    add_dependencies(MyExe MyPlugin)
    
endif()
