cmake_minimum_required(VERSION 3.20)

PROJECT(Voyage)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/bin")

add_subdirectory(Editor)
add_subdirectory(RenderEngine)
add_subdirectory(Framework)