project(RetroLibTests)
find_package(Catch2 3 REQUIRED)

add_executable(RetroLibTests Utils/PolymorphicTest.cpp)

target_link_libraries(RetroLibTests
    PRIVATE
        RetroLib
        Catch2::Catch2WithMain)


include(CTest)
include(Catch)
catch_discover_tests(RetroLibTests)