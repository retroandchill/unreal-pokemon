/**
 * @file CacheLastTest.cpp
 * @brief Test for caching the result of a range pipe.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#include <catch2/catch_test_macros.hpp>

#if RETROLIB_WITH_MODULES
import std;
import RetroLib;
#else
#include "RetroLib/Ranges/Views/CacheLastView.h"

#include <array>
#include <vector>
#endif

TEST_CASE("Can cache a temporary for use later on in the pipe", "[optionals]") {
    std::array values = {1, 2, 3, 4, 5};
    constexpr auto transformer = [](int value) {
        std::vector<int> v;
        for (int i = 0; i < value; i++) {
            v.push_back(i + 1);
        }
        return v;
    };


    SECTION("Iterating through saves the value") {
        auto view = values |
                    std::ranges::views::transform(transformer) |
                    retro::ranges::views::cache_last;

        REQUIRE(view.size() == 5);

        auto it = view.begin();
        CHECK((*it).size() == 1);
        it++;
        CHECK((*it).size() == 2);
        // Verify that a repeat read doesn't update the cache
        CHECK((*it).size() == 2);
        it++;
        CHECK((*it).size() == 3);
        it++;
        CHECK((*it).size() == 4);
        it++;
        CHECK((*it).size() == 5);
        it++;
        CHECK(it == view.end());
    }

    SECTION("Can get the difference between iterators") {
        auto view = values |
                    std::ranges::views::transform(transformer) |
                    retro::ranges::views::cache_last;

        REQUIRE(view.size() == 5);

        CHECK(view.begin() - view.end() == -5);
        CHECK(view.end() - view.begin() == 5);
    }

    SECTION("The value can be accessed in a loop") {
        auto view = values |
                    std::ranges::views::transform(transformer) |
                    retro::ranges::views::cache_last;

        int sum = 0;
        for (auto vec: view) {
            for (int i : vec) {
                sum += i;
            }
        }
        CHECK(sum == 35);
    }

    SECTION("Convert to a vector an cache") {
        // Here we're turning the collection into a span. This would emulate using ranges with a collection
        // type that isn't range-compatible.
        auto view = values |
                    std::ranges::views::transform(transformer) |
                    retro::ranges::views::cache_last |
                    retro::ranges::views::transform([](auto &&vec) { return std::span(vec); }) |
                    std::ranges::views::join;

        int sum = 0;
        for (auto &i : view) {
            sum += i;
        }
        CHECK(sum == 35);
    }
}
