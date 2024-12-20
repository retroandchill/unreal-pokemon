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
#endif

TEST_CASE("Verify that we can iterate over optionals", "[optionals]") {
    SECTION("Can use a ranged for loop") {
        retro::Optional value = 3;

        int sum = 0;
        for (auto i : value) {
            sum += i;
        }
        CHECK(sum == 3);
    }

    SECTION("Can use an iterator based for loop") {
        const retro::Optional value = 3;
        static_assert(std::ranges::sized_range<retro::Optional<int>>);

        int sum = 0;
        for (auto it = value.begin(); it != value.end(); it++) {
            sum += *it;
        }
        CHECK(sum == 3);
    }

    SECTION("Can use as part of a range pipe") {
        std::vector<retro::Optional<int>> values = {1, std::nullopt, 2, 3, std::nullopt, std::nullopt, std::nullopt};
        auto view = values | std::ranges::views::join;

        int sum = 0;
        for (auto i : view) {
            sum += i;
        }
        CHECK(sum == 6);
    }

    SECTION("Can be used to determine size") {
        auto view = retro::ranges::views::concat(retro::Optional(1), retro::Optional<int>(), retro::Optional(2),
                                                 retro::Optional<int>(), retro::Optional<int>(), retro::Optional(3));
        CHECK(view.size() == 3);
    }
}