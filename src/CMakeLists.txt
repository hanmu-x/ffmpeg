cmake_minimum_required (VERSION 3.15)

project(ffmpeg_test CXX)

add_subdirectory(tool_dir_name)

link_libraries(tool_lib_name)


add_executable(${PROJECT_NAME}  "main.cpp" )







