cmake_minimum_required (VERSION 3.15)

project(ffmpeg_tool_lib CXX)
foreach(dirname ${CMAKE_CURRENT_SOURCE_DIR})
    file(GLOB_RECURSE SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
            "${dirname}/*.h"
            "${dirname}/*.hpp"
            "${dirname}/*.c"
            "${dirname}/*.cpp"
            )
endforeach()



add_library(${PROJECT_NAME} STATIC ${SRC})


find_package(jsoncpp CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC JsonCpp::JsonCpp)


find_package(FFMPEG REQUIRED)
target_include_directories(${PROJECT_NAME}  PUBLIC ${FFMPEG_INCLUDE_DIRS})
target_link_directories(${PROJECT_NAME}  PUBLIC ${FFMPEG_LIBRARY_DIRS})
target_link_libraries(${PROJECT_NAME}  PUBLIC ${FFMPEG_LIBRARIES})
