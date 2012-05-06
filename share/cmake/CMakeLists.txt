cmake_minimum_required(VERSION 2.8)

##########################
#  CMake exported files  #
##########################

# Make relative paths absolute (needed later on)
foreach(p INCLUDE CMAKE)
    set(var INSTALL_${p}_DIR)
    if(NOT IS_ABSOLUTE ${${var}})
        set(ABS_${var} ${CMAKE_INSTALL_PREFIX}/${${var}})
    endif()
endforeach()

# Compute relative path between INSTALL_CMAKE_DIR and INSTALL_INCLUDE_DIR
file(RELATIVE_PATH 
    CONF_REL_INCLUDE_DIR 
    ${ABS_INSTALL_CMAKE_DIR}
    ${ABS_INSTALL_INCLUDE_DIR}
)

# Generate PluginConfig.cmake
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/PluginConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/PluginConfig.cmake
    @ONLY
)

# Generate PluginConfigVersion.cmake
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/PluginConfigVersion.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/PluginConfigVersion.cmake
    @ONLY
)

###############
#  Packaging  #
###############

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/PluginConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/PluginConfigVersion.cmake
    DESTINATION ${INSTALL_CMAKE_DIR}
    COMPONENT dev
)
