cmake_minimum_required(VERSION 3.10)

project(parser LANGUAGES C CXX)

set(PARSER_SOURCE fs/file.cpp)
add_executable(parser "main.cpp" ${PARSER_SOURCE})
set_target_properties(parser PROPERTIES
    CXX_STANDARD_REQUIRED TRUE
    CXX_STANDARD 17
)
add_subdirectory(stx)
target_link_libraries(parser stx)
target_include_directories(parser PRIVATE stx/include)
