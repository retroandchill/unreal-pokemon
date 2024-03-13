cmake_minimum_required(VERSION 3.15)

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../build/generators")

find_package(reflect-cpp REQUIRED)
add_library(ReflectCPP ALIAS reflect-cpp::reflect-cpp)