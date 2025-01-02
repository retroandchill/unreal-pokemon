/**
 * @file TestExtensionMethods.cpp
 * @brief Test for the extension method functionality.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#include "TestAdapter.h"

#if RETROLIB_WITH_MODULES
import std;
import RetroLib;
#else
#include "RetroLib.h"

#include <string_view>
#include <vector>
#endif

namespace Retro::Testing::Extensions {
    enum class TestEnum { Ordinal1, Ordinal2 };

    std::string_view EnumToString(TestEnum Enum) {
        switch (Enum) {
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
}

TEST_CASE_NAMED(FExtensionMethodTest, "RetroLib::Functional::ExtensionMethods", "[RetroLib][Functional]") {
    using namespace Retro::Testing::Extensions;
    SECTION("Extension method on enum with no arguments") {
        CHECK((TestEnum::Ordinal1 | Retro::Testing::Extensions::ToString()) == "Ordinal1");
        CHECK((TestEnum::Ordinal2 | Retro::Testing::Extensions::ToString()) == "Ordinal2");
    }

    SECTION("Extension method can be invoked with arguments") {
        std::vector<DemoStruct> Structs;
        Structs | Append(3, 6.5);
        REQUIRE(Structs.size() == 1);
        CHECK(Structs[0].Value1 == 3);
        CHECK(Structs[0].Value2 == 6.5);
    }
}