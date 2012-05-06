cmake_minimum_required(VERSION 2.8)

project(Plugin CXX)

#############
#  Sources  #
#############

set(PROJECT_INCLUDE_DIR
    ${PROJECT_SOURCE_DIR}/include
)

set(${PROJECT_NAME}_INCLUDE_DIR ${PROJECT_INCLUDE_DIR} CACHE INTERNAL "")

set(PROJECT_FILES
    ${PROJECT_INCLUDE_DIR}/${PROJECT_NAME}/IPlugin.h
    ${PROJECT_INCLUDE_DIR}/${PROJECT_NAME}/PluginLoader.h
    ${PROJECT_INCLUDE_DIR}/${PROJECT_NAME}/ExportAPI.h
)

add_custom_target(
    ${PROJECT_NAME}
    COMMAND ""
    SOURCES ${PROJECT_FILES}
)

######################
#  Global variables  #
######################

# Set project version
include(version.cmake)
message(STATUS "${PROJECT_NAME} version: ${${PROJECT_NAME}_VERSION}")

set(IS_TOPLEVEL_PROJECT FALSE)
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(IS_TOPLEVEL_PROJECT TRUE)
endif()

# Following code is enabled only if Versionning is the top level project
if(IS_TOPLEVEL_PROJECT)

    # Set install path (relative path only)
    set(INSTALL_INCLUDE_DIR include)
    set(INSTALL_CMAKE_DIR share/${PROJECT_NAME}/cmake)
    set(INSTALL_DOC_DIR share/doc/${PROJECT_NAME})

    ####################
    #  Subdirectories  #
    ####################

    add_subdirectory(share)

    ###############
    #  Packaging  #
    ###############

    # Package include directory
    install(DIRECTORY
	${PROJECT_INCLUDE_DIR}/
	DESTINATION ${INSTALL_INCLUDE_DIR}
	COMPONENT dev
    )

    ###########
    #  CPack  #
    ###########

    if(WIN32)
	set(CPACK_GENERATOR "ZIP")
    elseif(UNIX)
	set(CPACK_GENERATOR "TGZ")
    endif()
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Portable C++ library which can load/unload dynamic libraries at runtime.")
    set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE_1_0.txt")
    set(CPACK_PACKAGE_VERSION_MAJOR ${${PROJECT_NAME}_MAJOR})
    set(CPACK_PACKAGE_VERSION_MINOR ${${PROJECT_NAME}_MINOR})
    set(CPACK_PACKAGE_VERSION_PATCH ${${PROJECT_NAME}_PATCH})
    set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}_v${${PROJECT_NAME}_VERSION}")

    include(CPack)

endif(IS_TOPLEVEL_PROJECT)
