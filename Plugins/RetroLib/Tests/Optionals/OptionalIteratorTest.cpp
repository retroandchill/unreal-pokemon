/**
 * @file OptionalIteratorTest.cpp
 * @brief Test for the optional iterator semantics.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#include <catch2/catch_test_macros.hpp>

#if RETROLIB_WITH_MODULES
import std;
import RetroLib;
#else
#include "RetroLib/Optionals/Optional.h"
#include "RetroLib/Ranges/Views/Concat.h"
#include "RetroLib/Ranges/Views/NameAliases.h"

#include <ranges>
#endif

TEST_CASE("Verify that we can iterate over optionals", "[optionals]") {
    SECTION("Can use a ranged for loop") {
        Retro::Optional Value = 3;

        int Sum = 0;
        for (auto i : Value) {
            Sum += i;
        }
        CHECK(Sum == 3);
    }

    SECTION("Can use an iterator based for loop") {
        const Retro::Optional Value = 3;

        int Sum = 0;
        for (auto It = Value.begin(); It != Value.end(); It++) {
            Sum += *It;
        }
        CHECK(Sum == 3);
    }

    SECTION("Can use as part of a range pipe") {
        std::vector<Retro::Optional<int>> Values = {1, std::nullopt, 2, 3, std::nullopt, std::nullopt, std::nullopt};
        auto View = Values | Retro::Ranges::Views::Join;

        int Sum = 0;
        for (auto i : View) {
            Sum += i;
        }
        CHECK(Sum == 6);
    }

    SECTION("Can be used to determine size") {
        auto View = Retro::Ranges::Views::Concat(Retro::Optional(1), Retro::Optional<int>(), Retro::Optional(2),
                                                 Retro::Optional<int>(), Retro::Optional<int>(), Retro::Optional(3));
        CHECK(View.size() == 3);
    }
}