cmake_minimum_required(VERSION 3.10)

project(filesystem LANGUAGES C CXX)

add_library(filesystem STATIC file.cpp)
set_target_properties(filesystem PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED TRUE
)
target_include_directories(filesystem PRIVATE "${CMAKE_SOURCE_DIR}/stx/include")