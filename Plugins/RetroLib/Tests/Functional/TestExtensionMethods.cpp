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

#include <string_view>
#include <vector>
#endif

enum class TestEnum { Ordinal1, Ordinal2 };

std::string_view EnumToString(TestEnum test_enum) {
    switch (test_enum) {
    case TestEnum::Ordinal1:
        return "Ordinal1";
    case TestEnum::Ordinal2:
        return "Ordinal2";
    default:
        return "<Invalid>";
    }
}
constexpr auto ToString = Retro::ExtensionMethod<&EnumToString>;

struct VectorAppender {
    template <typename T, typename... A>
        requires std::constructible_from<T, A...>
    constexpr void operator()(std::vector<T> &Vec, A &&...Args) const {
        Vec.emplace_back(std::forward<A>(Args)...);
    }
};

struct DemoStruct {

    DemoStruct(int Value1, float Value2) : Value1(Value1), Value2(Value2) {
    }

    int Value1;
    float Value2;
};

constexpr VectorAppender VectorAppenderCaller;
constexpr auto Append = Retro::ExtensionMethod<VectorAppenderCaller>;

TEST_CASE("Test that extension methods can be used") {
    SECTION("Extension method on enum with no arguments") {
        CHECK((TestEnum::Ordinal1 | ToString()) == "Ordinal1");
        CHECK((TestEnum::Ordinal2 | ToString()) == "Ordinal2");
    }

    SECTION("Extension method can be invoked with arguments") {
        std::vector<DemoStruct> Structs;
        Structs | Append(3, 6.5);
        REQUIRE(Structs.size() == 1);
        CHECK(Structs[0].Value1 == 3);
        CHECK(Structs[0].Value2 == 6.5);
    }
}