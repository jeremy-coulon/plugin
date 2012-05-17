cmake_minimum_required(VERSION 2.8.8)

##########################
#  CMake exported files  #
##########################

include(CMakePackageConfigHelpers)

# Generate PluginConfig.cmake
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/PluginConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/PluginConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_CMAKEDIR}
    PATH_VARS CMAKE_INSTALL_FULL_INCLUDEDIR
)

# Generate PluginConfigVersion.cmake
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/PluginConfigVersion.cmake
    VERSION ${Plugin_VERSION}
    COMPATIBILITY ExactVersion
)

###############
#  Packaging  #
###############

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/PluginConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/PluginConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_CMAKEDIR}
    COMPONENT dev
)
