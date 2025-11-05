if (RETROLIB_WITH_MODULES)
    add_library(RetroLib)

    target_sources(RetroLib
        PUBLIC
            FILE_SET retrolib_modules TYPE CXX_MODULES
            FILES
            Public/RetroLib/RetroLib.ixx)
    target_compile_definitions(RetroLib
        PUBLIC
            RETROLIB_WITH_MODULES=1
            RETROLIB_WITH_COROUTINES=1)

    target_include_directories(RetroLib
            PUBLIC
            Public)
else()
    add_library(RetroLib INTERFACE)

    target_include_directories(RetroLib
            INTERFACE
            Public)
    target_compile_definitions(RetroLib
            INTERFACE
            RETROLIB_WITH_MODULES=0
            RETROLIB_WITH_COROUTINES=1)
endif()

