cmake_minimum_required(VERSION 2.8)

list(APPEND CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_LIST_DIR}/cmake/Modules
    ${CMAKE_CURRENT_LIST_DIR}/share/doc/quickbook/cmake/Modules
)
set(CMAKE_USER_MAKE_RULES_OVERRIDE ${CMAKE_CURRENT_LIST_DIR}/cmake/cmake-tools/DefaultBuildFlags.cmake)

project(Plugin NONE)

# Set project version
include(version.cmake)

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

# Set install path
include(GNUInstallDirs)
set(CMAKE_INSTALL_CMAKEDIR ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)
set(CMAKE_INSTALL_FULL_CMAKEDIR ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_CMAKEDIR})

# Build output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(${PROJECT_NAME}_PACKAGING_ENABLED TRUE CACHE INTERNAL "")
else()
    set(${PROJECT_NAME}_PACKAGING_ENABLED FALSE CACHE INTERNAL "")
endif()

####################
#  Subdirectories  #
####################

add_subdirectory(share)
add_subdirectory(examples)
add_subdirectory(tests)

###############
#  Packaging  #
###############

# Following code is enabled only if Plugin is the top level project
if(${PROJECT_NAME}_PACKAGING_ENABLED)

    # Package include directory
    install(DIRECTORY
        ${PROJECT_INCLUDE_DIR}/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        COMPONENT dev
        PATTERN "*~" EXCLUDE
    )

    ###########
    #  CPack  #
    ###########

    if(WIN32)
        set(CPACK_GENERATOR "ZIP")
        set(CPACK_SOURCE_GENERATOR "ZIP")
    elseif(UNIX)
        set(CPACK_GENERATOR "TGZ")
        set(CPACK_SOURCE_GENERATOR "TGZ")
    endif()
    set(CPACK_COMPONENTS_ALL dev doc)
    set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Portable C++ library which can load/unload dynamic libraries at runtime.")
    set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE_1_0.txt")
    set(CPACK_PACKAGE_VERSION_MAJOR ${${PROJECT_NAME}_MAJOR})
    set(CPACK_PACKAGE_VERSION_MINOR ${${PROJECT_NAME}_MINOR})
    set(CPACK_PACKAGE_VERSION_PATCH ${${PROJECT_NAME}_PATCH})
    set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}_v${${PROJECT_NAME}_VERSION}")
    set(CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT_NAME}-src_v${${PROJECT_NAME}_VERSION}")
    set(CPACK_SOURCE_IGNORE_FILES "/build/;/install/;CMakeLists.txt.user;.git;.gitignore;.gitmodules;~$")

    include(CPack)

endif()

mark_as_advanced(
    CMAKE_CODEBLOCKS_EXECUTABLE
    CMAKE_CONFIGURATION_TYPES
    CMAKE_INSTALL_PREFIX
)
