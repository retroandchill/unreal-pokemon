cmake_minimum_required(VERSION 3.24)

project(PokemonStarterKitLibs LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

list(APPEND CMAKE_PROJECT_TOP_LEVEL_INCLUDES ${CMAKE_SOURCE_DIR}/cmake/conan_provider.cmake)

find_package(range-v3 REQUIRED)
add_library(range-v3 ALIAS range-v3::range-v3)