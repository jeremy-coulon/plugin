project(Plugin CXX)

set(${PROJECT_NAME}_INCLUDE_DIR ${${PROJECT_NAME}_SOURCE_DIR}/include CACHE INTERNAL "")

set(${PROJECT_NAME}_FILES
    ${${PROJECT_NAME}_INCLUDE_DIR}/${PROJECT_NAME}/IPlugin.h
    ${${PROJECT_NAME}_INCLUDE_DIR}/${PROJECT_NAME}/PluginLoader.h
    ${${PROJECT_NAME}_INCLUDE_DIR}/${PROJECT_NAME}/ExportAPI.h
)

include_directories(
    ${${PROJECT_NAME}_INCLUDE_DIR}
    ${Versionning_INCLUDE_DIR}
)

add_custom_target(${PROJECT_NAME}
    COMMAND ""
    SOURCES ${${PROJECT_NAME}_FILES})
