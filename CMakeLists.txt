cmake_minimum_required(VERSION 2.8)

set(CMAKE_USER_MAKE_RULES_OVERRIDE ${CMAKE_CURRENT_LIST_DIR}/DefaultBuildFlags.cmake)

project(Plugin CXX)

#############
#  Sources  #
#############

set(PROJECT_INCLUDE_DIR
    ${PROJECT_SOURCE_DIR}/include
)

set(${PROJECT_NAME}_INCLUDE_DIR ${PROJECT_INCLUDE_DIR} CACHE INTERNAL "")

set(PROJECT_FILES
    ${PROJECT_INCLUDE_DIR}/${PROJECT_NAME}/ExportAPI.h
    ${PROJECT_INCLUDE_DIR}/${PROJECT_NAME}/IPlugin.h
    ${PROJECT_INCLUDE_DIR}/${PROJECT_NAME}/PluginFactory.h
    ${PROJECT_INCLUDE_DIR}/${PROJECT_NAME}/PluginLoader.h
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

    # Set install path
    include(GNUInstallDirs)
    set(CMAKE_INSTALL_CMAKEDIR ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)
    set(CMAKE_INSTALL_FULL_CMAKEDIR ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_CMAKEDIR})

    # Build output directories
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

    ####################
    #  Subdirectories  #
    ####################

    add_subdirectory(share)
    add_subdirectory(src)

    ###############
    #  Packaging  #
    ###############

    # Package include directory
    install(DIRECTORY
	${PROJECT_INCLUDE_DIR}/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
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
