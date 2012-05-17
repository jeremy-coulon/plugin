cmake_minimum_required(VERSION 2.8)

project(MyExe CXX)

include_directories(
    ${Plugin_INCLUDE_DIR}
    ${Versionning_INCLUDE_DIR}
    ${Boost_INCLUDE_DIR}
)

add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCE_DIR}/src/main.cpp)

target_link_libraries(${PROJECT_NAME}
    ${CMAKE_DL_LIBS}
)

