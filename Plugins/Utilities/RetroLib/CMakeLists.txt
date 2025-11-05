cmake_minimum_required(VERSION 3.30)
list(APPEND CMAKE_PROJECT_TOP_LEVEL_INCLUDES ${CMAKE_SOURCE_DIR}/CMake/conan_provider.cmake)
project(RetroLib)

option(RETROLIB_WITH_TESTS "Should this project be compiled with the tests?" ON)
option(RETROLIB_WITH_MODULES "Should this project be compiled with the modules?" ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 20)
if (RETROLIB_WITH_MODULES)
    if (CMAKE_VERSION VERSION_LESS "3.28.0")
        if(CMAKE_VERSION VERSION_LESS "3.27.0")
            set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "2182bf5c-ef0d-489a-91da-49dbc3090d2a")
        else()
            set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "aa1f7df0-828a-4fcd-9afc-2dc80491aca7")
        endif()
        set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP 1)
    else()
        cmake_policy(VERSION 3.28)
    endif()

    if (MSVC)
        set(VCTOOLS_INSTALL_DIR $ENV{VCToolsInstallDir})
        message("Set MSVC tools directory: ${VCTOOLS_INSTALL_DIR}")
        include(EnableStandardLibraryModule)
    else()
        message("${CMAKE_CURRENT_SOURCE_DIR}")
        include(FetchContent)
        FetchContent_Declare(
                std
                URL "file://${LIBCXX_BUILD}/share/libc++/v1/"
                DOWNLOAD_EXTRACT_TIMESTAMP TRUE
                SYSTEM
        )
        FetchContent_MakeAvailable(std)

        add_library(std)
        target_sources(std PUBLIC
                FILE_SET CXX_MODULES
                BASE_DIRS ${std_SOURCE_DIR}
                FILES
                ${std_SOURCE_DIR}/std.cppm
                ${std_SOURCE_DIR}/std.compat.cppm
        )

        link_libraries(std)
    endif()

    set(CMAKE_CXX_SCAN_FOR_MODULES ON)
endif()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


if (RETROLIB_WITH_TESTS)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-instr-generate -fcoverage-mapping")
    endif()
endif()



add_subdirectory(Source/RetroLib)

if (RETROLIB_WITH_TESTS)
    enable_testing()
    add_subdirectory(Source/RetroLibTests)
endif()
