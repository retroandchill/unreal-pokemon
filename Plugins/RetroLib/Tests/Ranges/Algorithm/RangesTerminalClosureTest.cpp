/**
 * @file RangesTerminalClosureTest.cpp
 * @brief Tests for range terminal pipe operations
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#include <catch2/catch_test_macros.hpp>
#include <map>
#include <set>

#if RETROLIB_WITH_MODULES
import std;
import RetroLib;
#else
#include "RetroLib/Ranges/Algorithm/ForEach.h"
#include "RetroLib/Ranges/Algorithm/Reduce.h"
#include "RetroLib/Ranges/Algorithm/To.h"
#include "RetroLib/Ranges/Views/Filter.h"
#include "RetroLib/Utils/Operators.h"

#include <array>
#include <optional>
#include <vector>
#endif

TEST_CASE("Ranges can be converted into a collection type", "[ranges]") {
    static constexpr std::array values = {1, 2, 3, 4, 5};
    SECTION("Convert to a like range type") {
        auto vectored = values | retro::ranges::to<std::vector>();
        CHECK(vectored == std::vector({1, 2, 3, 4, 5}));
        CHECK(retro::ranges::container_capacity(vectored) == 5);
        CHECK(retro::ranges::container_max_size(vectored) == vectored.max_size());
    }

    SECTION("Can convert between unlike, but compatible, range types") {
        auto vectored = values | retro::ranges::to<std::vector<double>>();
        CHECK(vectored == std::vector({1.0, 2.0, 3.0, 4.0, 5.0}));
        CHECK(retro::ranges::container_capacity(vectored) == 5);
        CHECK(retro::ranges::container_max_size(vectored) == vectored.max_size());
    }

    SECTION("Can convert a vector into a set") {
        static constexpr std::array duplicated = {1, 1, 2, 2, 2, 2, 3, 3, 3, 3};
        auto as_set = duplicated | retro::ranges::to<std::set>();
        CHECK(as_set == std::set({1, 2, 3}));
    }

    SECTION("Can convert a range into a map using pairs") {
        static constexpr std::array pairs = {std::make_pair(1, 2), std::make_pair(3, 4)};
        auto as_map = pairs | retro::ranges::to<std::map<int, int>>();
        CHECK(as_map == std::map<int, int>({{1, 2}, {3, 4}}));
    }
}

TEST_CASE("Can iterate over a range using a functor", "[ranges]") {
    static constexpr std::array values = {1, 2, 3, 4, 5};
    SECTION("Can iterate over the values of a collection") {
        std::vector<int> vectored;
        values | retro::ranges::for_each([&vectored](int value) { vectored.push_back(value); });
        CHECK(vectored == std::vector({1, 2, 3, 4, 5}));

        constexpr auto add_to_vector = [](int value, std::vector<int> &vector) { vector.push_back(value); };
        vectored.clear();
        values | retro::ranges::for_each<add_to_vector>(std::ref(vectored));
        CHECK(vectored == std::vector({1, 2, 3, 4, 5}));
    }

    SECTION("Can iterate over a range of pairs using a two arg functor") {
        static constexpr std::array pairs = {std::make_pair(1, 2), std::make_pair(3, 4)};
        std::map<int, int> as_map;
        pairs | retro::ranges::for_each([&as_map](int key, int value) { as_map[key] = value; });
        CHECK(as_map == std::map<int, int>({{1, 2}, {3, 4}}));
    }
}

TEST_CASE("Can reduce a range to a single value", "[ranges]") {
    static constexpr std::array values = {1, 2, 3, 4, 5};
    SECTION("Can reduce a range to a single value using a runtime binding") {
        auto result = values | retro::ranges::reduce(0, retro::add);
        CHECK(result == 15);
    }

    SECTION("Can reduce a range to a single value using a constexpr binding") {
        auto result = values | retro::ranges::reduce<retro::add>(0);
        CHECK(result == 15);
    }
}

TEST_CASE("Can find the first value in a range", "[ranges]") {
    static constexpr std::array values = {1, 2, 3, 4, 5};
    constexpr auto is_multiple_of = [](int i, int j) { return i % j == 0; };
    SECTION("Can reduce using an inferred optional value") {
        auto valid_result = values | retro::ranges::views::filter<is_multiple_of>(3) | retro::ranges::find_first();
        CHECK(valid_result == 3);

        auto invalid_result = values | retro::ranges::views::filter<is_multiple_of>(10) | retro::ranges::find_first();
        CHECK_FALSE(invalid_result.has_value());
    }

    SECTION("Can reduce using an inferred template parameter") {
        auto valid_result =
            values | retro::ranges::views::filter<is_multiple_of>(3) | retro::ranges::find_first<std::optional>();
        REQUIRE(valid_result.has_value());
        CHECK(valid_result->get() == 3);

        auto invalid_result =
            values | retro::ranges::views::filter<is_multiple_of>(10) | retro::ranges::find_first<std::optional>();
        CHECK_FALSE(invalid_result.has_value());
    }

    SECTION("Can reduce using an explicit template parameter") {
        auto valid_result = values | retro::ranges::views::filter<is_multiple_of>(3) |
                            retro::ranges::find_first<retro::Optional<int>>();
        CHECK(valid_result == 3);

        auto invalid_result =
            values | retro::ranges::views::filter<is_multiple_of>(10) | retro::ranges::find_first<std::optional<int>>();
        CHECK_FALSE(invalid_result.has_value());
    }
}

TEST_CASE("Can check a collection for all of, any of, and none of", "[ranges]") {
    static constexpr std::array values = {1, 2, 3, 4, 5};
    SECTION("Check the all of condition") {
        CHECK(values | retro::ranges::all_of(retro::greater_than, 0));
        CHECK(values | retro::ranges::all_of<retro::greater_than>(0));
        CHECK_FALSE(values | retro::ranges::all_of(retro::greater_than, 10));
        CHECK_FALSE(values | retro::ranges::all_of<retro::greater_than>(20));
    }

    SECTION("Check the none of condition") {
        CHECK_FALSE(values | retro::ranges::none_of(retro::greater_than, 0));
        CHECK_FALSE(values | retro::ranges::none_of<retro::greater_than>(0));
        CHECK(values | retro::ranges::none_of(retro::greater_than, 10));
        CHECK(values | retro::ranges::none_of<retro::greater_than>(20));
    }

    SECTION("Check the any of condition") {
        CHECK(values | retro::ranges::any_of(retro::greater_than, 2));
        CHECK(values | retro::ranges::any_of<retro::greater_than>(2));
        CHECK_FALSE(values | retro::ranges::any_of(retro::greater_than, 10));
        CHECK_FALSE(values | retro::ranges::any_of<retro::greater_than>(20));
    }
}