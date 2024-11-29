
add_library(RetroLib)



target_include_directories(RetroLib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include)

if (RETROLIB_WITH_MODULES)
    target_sources(RetroLib
        PUBLIC
            FILE_SET retrolib_modules TYPE CXX_MODULES
            FILES
                src/RetroLib.ixx
                src/StlMock.ixx)
    target_compile_definitions(RetroLib
        PUBLIC
            RETROLIB_WITH_MODULES)
endif()
