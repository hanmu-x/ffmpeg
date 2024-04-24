cmake_minimum_required (VERSION 3.15)

project(ffmpeg_test CXX)

add_subdirectory(ffmpeg_tool)

link_libraries(ffmpeg_tool_lib)


add_executable(${PROJECT_NAME}  "main.cpp" )







