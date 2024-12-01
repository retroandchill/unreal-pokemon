/**
 * @file RangesToTest.cpp
 * @brief Tested for ranges to.
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
#include "RetroLib/Ranges/Algorithm/To.h"

#include <array>
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