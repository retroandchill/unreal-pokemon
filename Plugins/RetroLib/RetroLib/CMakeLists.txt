
add_library(RetroLib)
target_sources(RetroLib
    PUBLIC
    FILE_SET retrolib_modules TYPE CXX_MODULES
    FILES
        src/RetroLib.ixx
        src/StlMock.ixx)