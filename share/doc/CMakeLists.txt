cmake_minimum_required(VERSION 2.8)

###################
#  Documentation  #
###################

# Generate VERSION.txt
file(WRITE
    ${CMAKE_CURRENT_BINARY_DIR}/VERSION.txt
    "${PROJECT_NAME} v${${PROJECT_NAME}_VERSION}"
)

find_package(Doxygen)
if(DOXYGEN_FOUND)
    option(${PROJECT_NAME}_BUILD_DOC "Generate API documentation with Doxygen" OFF)
    if(${PROJECT_NAME}_BUILD_DOC)
	# Copy license file to test generated documentation
        file(COPY ${PROJECT_SOURCE_DIR}/LICENSE_1_0.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
	# Generate Doxyfile
        configure_file(
            ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in.cmake 
            ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile 
            @ONLY
        )
	# Doxygen target
        add_custom_target(
            ${PROJECT_NAME}_DOCUMENTATION
            ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM
            SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in.cmake
        )
    endif()
endif()

###############
#  Packaging  #
###############

install(FILES
    ${PROJECT_SOURCE_DIR}/LICENSE_1_0.txt
    ${PROJECT_SOURCE_DIR}/INSTALL.txt
    ${PROJECT_SOURCE_DIR}/README.txt
    ${CMAKE_CURRENT_BINARY_DIR}/VERSION.txt
    DESTINATION ${INSTALL_DOC_DIR}
    COMPONENT doc
)
if(${PROJECT_NAME}_BUILD_DOC)
    install(DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR}/html
        DESTINATION ${INSTALL_DOC_DIR}
        COMPONENT doc
    )
endif()
