project(RetroLibTests)
find_package(Catch2 3 REQUIRED)

add_executable(RetroLibTests Utils/PolymorphicTest.cpp
        Ranges/Views/AnyViewTest.cpp
        Functional/TestExtensionMethods.cpp
        Functional/TestBindings.cpp
        Ranges/Algorithm/RangesTerminalClosureTest.cpp
        Optionals/TestOptionalOperations.cpp
        Optionals/TestOptionalPipes.cpp
        Ranges/Views/TestRangeAdapters.cpp
        Optionals/TestOptional.cpp
        Utils/TestValidPtr.cpp
        Casts/TestCasts.cpp
        Utils/OperatorsTest.cpp
        Optionals/OptionalIteratorTest.cpp
)

target_link_libraries(RetroLibTests
    PRIVATE
        RetroLib
        Catch2::Catch2WithMain)


include(CTest)
include(Catch)
catch_discover_tests(RetroLibTests)