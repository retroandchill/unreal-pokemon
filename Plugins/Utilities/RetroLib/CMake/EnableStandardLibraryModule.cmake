cmake_minimum_required(VERSION 3.26.0 FATAL_ERROR)

# Check compiler support for C++23 Standard Library Module.
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "17.0.0")
    if (CMAKE_CXX_STANDARD LESS 20)
        message(FATAL_ERROR "C++20 or newer is required.")
    endif()

    set(CMAKE_CXX_STANDARD_REQUIRED YES)
    set(CMAKE_CXX_EXTENSIONS OFF)

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
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "19.36")
    if (CMAKE_CXX_STANDARD VERSION_LESS 20)
        message(FATAL_ERROR "C++20 or newer is required.")
    elseif (CMAKE_CXX_STANDARD VERSION_LESS 23 AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "19.38")
        message(FATAL_ERROR "C++23 Standard library module in C++20 is only supported with MSVC 19.38 or newer.")
    endif()

    file(TO_CMAKE_PATH "${VCTOOLS_INSTALL_DIR}" VCTOOLS_INSTALL_DIR)

    include(FetchContent)
    FetchContent_Declare(
        std
        URL "file://${VCTOOLS_INSTALL_DIR}/modules"
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        SYSTEM
    )
    FetchContent_MakeAvailable(std)

    add_library(std)
    target_sources(std PUBLIC
        FILE_SET CXX_MODULES
        BASE_DIRS ${std_SOURCE_DIR}
        FILES
            ${std_SOURCE_DIR}/std.ixx
            ${std_SOURCE_DIR}/std.compat.ixx
    )

    link_libraries(std)
else()
    message(FATAL_ERROR "C++23 Standard library module is not supported with current compiler.")
endif()
