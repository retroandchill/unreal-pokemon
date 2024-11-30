/**
 * @file TestExtensionMethods.cpp
 * @brief Test for the extension method functionality.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#include <catch2/catch_test_macros.hpp>

#if RETROLIB_WITH_MODULES
import std;
import RetroLib;
#else
#include "RetroLib/Functional/ExtensionMethods.h"
#include <vector>
#include <string_view>
#endif

enum class TestEnum {
    Ordinal1,
    Ordinal2
};

std::string_view enum_to_string(TestEnum test_enum) {
    switch (test_enum) {
    case TestEnum::Ordinal1:
        return "Ordinal1";
    case TestEnum::Ordinal2:
        return "Ordinal2";
    default:
        return "<Invalid>";
    }
}
constexpr retro::ExtensionMethodBinder<&enum_to_string> to_string;

struct VectorAppender {
    template <typename T, typename... A>
        requires std::constructible_from<T, A...>
    constexpr void operator()(std::vector<T>& vec, A&&... args) const {
        vec.emplace_back(std::forward<A>(args)...);
    }
};

struct DemoStruct {

    DemoStruct(int value1, float value2) : value1(value1), value2(value2) {}

    int value1;
    float value2;
};

constexpr VectorAppender vector_appender;
constexpr retro::ExtensionMethodBinder<vector_appender> append;

TEST_CASE("Test that extension methods can be used") {
    SECTION("Extension method on enum with no arguments") {
        CHECK((TestEnum::Ordinal1 | to_string()) == "Ordinal1");
        CHECK((TestEnum::Ordinal2 | to_string()) == "Ordinal2");
    }

    SECTION("Extension method can be invoked with arguments") {
        std::vector<DemoStruct> structs;
        structs | append(3, 6.5);
        REQUIRE(structs.size() == 1);
        CHECK(structs[0].value1 == 3);
        CHECK(structs[0].value2 == 6.5);
    }
}