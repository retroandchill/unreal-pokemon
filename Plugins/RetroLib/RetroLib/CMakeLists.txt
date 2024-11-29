if (RETROLIB_WITH_MODULES)
    add_library(RetroLib)

    target_sources(RetroLib
        PUBLIC
            FILE_SET retrolib_modules TYPE CXX_MODULES
            FILES
                src/RetroLib.ixx
                src/StlMock.ixx)
    target_compile_definitions(RetroLib
        PUBLIC
            RETROLIB_WITH_MODULES=1)

    target_include_directories(RetroLib
            PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/include)
else()
    add_library(RetroLib INTERFACE)

    target_include_directories(RetroLib
            INTERFACE
            ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_compile_definitions(RetroLib
            INTERFACE
            RETROLIB_WITH_MODULES=0)
endif()

