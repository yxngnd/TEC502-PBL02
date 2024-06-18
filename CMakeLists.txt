cmake_minimum_required(VERSION 3.5)
project(TEC502-PBL02)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Crow REQUIRED)

add_executable(server server.cpp)

target_link_libraries(server Crow::Crow)