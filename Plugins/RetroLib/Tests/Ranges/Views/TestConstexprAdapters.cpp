/**
 * @file TestConstexprAdapters.cpp
 * @brief Test the two constexpr adapters for transform and filter.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#include <catch2/catch_test_macros.hpp>

#if RETROLIB_WITH_MODULES
import std;
import RetroLib;
#else
#include "RetroLib/Ranges/Views/Filter.h"
#include "RetroLib/Ranges/Views/Transform.h"

#include <vector>
#include <array>
#endif

TEST_CASE("Can use the constexpr range adapters", "[ranges]") {
    constexpr std::array values = {1, 2, 3, 4, 5};
    constexpr auto is_even = [](int i) { return i % 2 == 0; };
    constexpr auto double_value = [](int i) { return i * 2; };

    SECTION("Can use the regular functional operators") {
        auto filtered = retro::ranges::views::filter<is_even>(values);
        auto transformed = retro::ranges::views::transform<double_value>(filtered);

        auto it = transformed.begin();
        CHECK(*it == 4);
        ++it;
        CHECK(*it == 8);
        ++it;
        CHECK(it == transformed.end());
    }

    SECTION("Can use the range pipe syntax") {
        auto transformed = values |
            retro::ranges::views::filter<is_even>() |
            retro::ranges::views::transform<double_value>();

        auto it = transformed.begin();
        CHECK(*it == 4);
        ++it;
        CHECK(*it == 8);
        ++it;
        CHECK(it == transformed.end());
    }
}