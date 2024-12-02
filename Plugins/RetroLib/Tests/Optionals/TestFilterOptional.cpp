/**
 * @file TestFilterOptional.cpp
 * @brief Test for the optional filter operation.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#include <catch2/catch_test_macros.hpp>

#if RETROLIB_WITH_MODULES
import std;
import RetroLib;
#else
#include "RetroLib/Functional/BindBack.h"
#include "RetroLib/Optionals/Filter.h"
#endif

TEST_CASE("Can filter an optional value", "[optionals]") {
    SECTION("Can filter on a constant functor") {
        constexpr auto is_even = [](int x) { return x % 2 == 0; };
        std::optional odd = 3;
        std::optional even = 4;
        std::optional<int> empty;
        auto filtered_odd = odd | retro::optionals::filter<is_even>();
        CHECK_FALSE(filtered_odd.has_value());
        auto filtered_even = even | retro::optionals::filter<is_even>();
        CHECK(filtered_even.has_value());
        auto filtered_empty = empty | retro::optionals::filter<is_even>();
        CHECK_FALSE(filtered_empty.has_value());

        auto filtered_rvalue_odd = std::optional(5) | retro::optionals::filter<is_even>();
        CHECK_FALSE(filtered_rvalue_odd.has_value());
        auto filtered_rvalue_even = std::optional(6) | retro::optionals::filter<is_even>();
        CHECK(filtered_rvalue_even.has_value());
        auto filtered_empty_rvalue = std::optional<int>() | retro::optionals::filter<is_even>();
        CHECK_FALSE(filtered_empty_rvalue.has_value());
    }

    SECTION("Can filter on a constant functor") {
        constexpr auto greater_than = [](int x, int y) { return x > y; };
        std::optional odd = 3;
        std::optional even = 4;
        std::optional<int> empty;
        static_assert(retro::optionals::Optional<decltype(odd)>);
        auto filtered_odd = odd | retro::optionals::filter(retro::bind_back<greater_than>(4));
        CHECK_FALSE(filtered_odd.has_value());
        auto filtered_even = even | retro::optionals::filter(retro::bind_back<greater_than>(3));
        CHECK(filtered_even.has_value());
        auto filtered_empty = empty | retro::optionals::filter(retro::bind_back<greater_than>(3));
        CHECK_FALSE(filtered_empty.has_value());

        auto filtered_rvalue_odd = std::optional(5) | retro::optionals::filter(retro::bind_back<greater_than>(7));
        CHECK_FALSE(filtered_rvalue_odd.has_value());
        auto filtered_rvalue_even = std::optional(6) | retro::optionals::filter(retro::bind_back<greater_than>(4));
        CHECK(filtered_rvalue_even.has_value());
        auto filtered_rvalue_empty = std::optional<int>() | retro::optionals::filter(retro::bind_back<greater_than>(4));
        CHECK_FALSE(filtered_rvalue_empty.has_value());
    }
}