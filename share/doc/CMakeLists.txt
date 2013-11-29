cmake_minimum_required(VERSION 2.8)

###################
#  Documentation  #
###################

# Generate VERSION.txt
file(WRITE
    ${CMAKE_CURRENT_BINARY_DIR}/VERSION.txt
    "${PROJECT_NAME} v${${PROJECT_NAME}_VERSION}\n"
)

option(${PROJECT_NAME}_BUILD_DOC "Generate API documentation with Doxygen" ${BUILD_ALL})
if(${PROJECT_NAME}_BUILD_DOC)
    find_package(BoostBook REQUIRED)
    find_package(QuickBook REQUIRED)
    find_package(XSLTPROC REQUIRED)
    find_package(Doxygen REQUIRED)
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/quickbook)
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doxygen)
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/boostbook)
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/docbook)
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html)
    # Copy css and image files to test generated documentation
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/quickbook/css/boostbook.css DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/html)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/quickbook/images DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/html FILES_MATCHING PATTERN *.png)
    # Generate Doxyfile
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
        ${CMAKE_CURRENT_BINARY_DIR}/doxygen/Doxyfile
        @ONLY
    )
    # Generate plugin.qbk
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/plugin.qbk
        ${CMAKE_CURRENT_BINARY_DIR}/plugin.qbk
        @ONLY
    )
    # Custom target to generate both Doxygen and QuickBook HTML documentation
    add_custom_target(
        ${PROJECT_NAME}_DOCUMENTATION
        COMMAND ${QUICKBOOK_EXECUTABLE} --output-file ${CMAKE_CURRENT_BINARY_DIR}/quickbook/quickbook.xml ${CMAKE_CURRENT_BINARY_DIR}/plugin.qbk
        COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_CURRENT_BINARY_DIR}/doxygen/xml/*.xml
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doxygen/Doxyfile
        COMMAND ${XSLTPROC_EXECUTABLE} --stringparam doxygen.xml.path ${CMAKE_CURRENT_BINARY_DIR}/doxygen/xml --xinclude -o ${CMAKE_CURRENT_BINARY_DIR}/boostbook/doxygen.xml ${BOOSTBOOK_XSL_DIR}/doxygen/collect.xsl ${CMAKE_CURRENT_BINARY_DIR}/doxygen/xml/index.xml
        COMMAND ${XSLTPROC_EXECUTABLE} --stringparam boost.doxygen.header.prefix "Plugin" --xinclude -o ${CMAKE_CURRENT_BINARY_DIR}/boostbook/boostbook.xml ${BOOSTBOOK_XSL_DIR}/doxygen/doxygen2boostbook.xsl ${CMAKE_CURRENT_BINARY_DIR}/boostbook/doxygen.xml
        COMMAND ${XSLTPROC_EXECUTABLE} --stringparam boost.root "../../../../include" --xinclude -o ${CMAKE_CURRENT_BINARY_DIR}/docbook/docbook.xml ${BOOSTBOOK_XSL_DIR}/docbook.xsl ${CMAKE_CURRENT_BINARY_DIR}/quickbook/quickbook.xml
        COMMAND ${XSLTPROC_EXECUTABLE} --stringparam boost.root "../../../../include" --xinclude -o ${CMAKE_CURRENT_BINARY_DIR}/html/ ${BOOSTBOOK_XSL_DIR}/html.xsl ${CMAKE_CURRENT_BINARY_DIR}/docbook/docbook.xml
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating QuickBook documentation"
        VERBATIM
        SOURCES
            ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile
            ${CMAKE_CURRENT_SOURCE_DIR}/plugin.qbk
    )
endif()

###############
#  Packaging  #
###############
if(${PROJECT_NAME}_PACKAGING_ENABLED)
    set(doc_files
        ${PROJECT_SOURCE_DIR}/LICENSE_1_0.txt
        ${PROJECT_SOURCE_DIR}/README.txt
        ${PROJECT_SOURCE_DIR}/INSTALL.txt
        ${CMAKE_CURRENT_BINARY_DIR}/VERSION.txt
    )
    install(FILES
        ${doc_files}
        DESTINATION ${CMAKE_INSTALL_DOCDIR}
        COMPONENT dev
    )
    install(FILES
        ${doc_files}
        DESTINATION ${CMAKE_INSTALL_DOCDIR}
        COMPONENT doc
    )
    if(${PROJECT_NAME}_BUILD_DOC)
        install(CODE
            "execute_process(COMMAND \"${CMAKE_COMMAND}\" --build ${CMAKE_BINARY_DIR} --target ${PROJECT_NAME}_DOCUMENTATION)"
            COMPONENT doc
        )
        install(DIRECTORY
            ${CMAKE_CURRENT_BINARY_DIR}/html
            DESTINATION ${CMAKE_INSTALL_DOCDIR}
            COMPONENT doc
        )
    endif()
endif()
