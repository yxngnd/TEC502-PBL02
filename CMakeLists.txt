cmake_minimum_required(VERSION 3.5)
project(TEC502-PBL02)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(httplib SYSTEM
    GIT_REPOSITORY https://github.com/yhirose/cpp-httplib
    GIT_TAG a609330e4c6374f741d3b369269f7848255e1954 # v0.14.1
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(httplib)
find_package(Crow REQUIRED)

add_executable(server server.cpp)

target_link_libraries(server PRIVATE Crow::Crow httplib)