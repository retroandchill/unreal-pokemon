project(RetroLibTests)
find_package(Catch2 3 REQUIRED)

add_executable(RetroLibTests
        Private/Utils/PolymorphicTest.cpp
        Private/Ranges/Views/AnyViewTest.cpp
        Private/Functional/TestExtensionMethods.cpp
        Private/Functional/TestBindings.cpp
        Private/Ranges/Algorithm/RangesTerminalClosureTest.cpp
        Private/Optionals/TestOptionalOperations.cpp
        Private/Optionals/TestOptionalPipes.cpp
        Private/Ranges/Views/TestRangeAdapters.cpp
        Private/Utils/TestValidPtr.cpp
        Private/Casts/TestCasts.cpp
        Private/Utils/OperatorsTest.cpp
        Private/Optionals/OptionalIteratorTest.cpp
        Private/Utils/UniqueAnyTest.cpp
        Private/Ranges/Views/GeneratorTest.cpp
)

target_link_libraries(RetroLibTests
    PRIVATE
        RetroLib
        Catch2::Catch2WithMain)

target_include_directories(RetroLibTests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Public
        ${CMAKE_CURRENT_SOURCE_DIR}/Private)


include(CTest)
include(Catch)
catch_discover_tests(RetroLibTests)