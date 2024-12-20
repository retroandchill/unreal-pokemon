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
#include "RetroLib/Ranges/Views/CacheLastView.h"
#include "RetroLib/Ranges/Views/Concat.h"
#include "RetroLib/Ranges/Views/Filter.h"
#include "RetroLib/Ranges/Views/JoinWith.h"
#include "RetroLib/Ranges/Views/Transform.h"
#include "RetroLib/Ranges/Algorithm/To
.h "

#include <array>
#include <string>
#include <string_view>
#include <vector>
#endif

TEST_CASE("Can use the standard range adapters", "[ranges]") {
    constexpr std::array values = {1, 2, 3, 4, 5};
    constexpr auto is_even = [](int i) { return i % 2 == 0; };
    constexpr auto double_value = [](int i, int j) { return i * j; };

    SECTION("Can use the regular functional operators") {
        auto filtered = retro::ranges::views::filter(values, is_even);
        auto transformed = retro::ranges::views::transform(filtered, double_value, 2);

        auto it = transformed.begin();
        CHECK(*it == 4);
        ++it;
        CHECK(*it == 8);
        ++it;
        CHECK(it == transformed.end());
    }

    SECTION("Can use the range pipe syntax") {
        auto transformed =
            values | retro::ranges::views::filter(is_even) | retro::ranges::views::transform(double_value, 2);

        auto it = transformed.begin();
        CHECK(*it == 4);
        ++it;
        CHECK(*it == 8);
        ++it;
        CHECK(it == transformed.end());
    }
}

TEST_CASE("Can use the constexpr range adapters", "[ranges]") {
    constexpr std::array values = {1, 2, 3, 4, 5};
    constexpr auto is_even = [](int i) { return i % 2 == 0; };
    constexpr auto double_value = [](int i, int j) { return i * j; };

    SECTION("Can use the regular functional operators") {
        auto filtered = retro::ranges::views::filter<is_even>(values);
        auto transformed = retro::ranges::views::transform<double_value>(filtered, 2);

        auto it = transformed.begin();
        CHECK(*it == 4);
        ++it;
        CHECK(*it == 8);
        ++it;
        CHECK(it == transformed.end());
    }

    SECTION("Can use the range pipe syntax") {
        auto transformed =
            values | retro::ranges::views::filter<is_even>() | retro::ranges::views::transform<double_value>(2);

        auto it = transformed.begin();
        CHECK(*it == 4);
        ++it;
        CHECK(*it == 8);
        ++it;
        CHECK(it == transformed.end());
    }
}

TEST_CASE("Can concatenate two unlike ranges into a single unified view", "[ranges]") {
    std::array range1 = {1, 2, 3, 4, 5};
    std::vector range2 = {6, 7, 8, 9, 10};

    SECTION("Can concatenate and use with a ranged for loop") {
        int sum = 0;
        for (decltype(auto) i : retro::ranges::views::concat(range1, range2)) {
            sum += i;
        }
        CHECK(sum == 55);
    }

    SECTION("Can use an iterator based view setup") {
        auto view = retro::ranges::views::concat(range1, range2);
        int sum = 0;
        for (auto it = view.begin(); it != view.end(); ++it) {
            sum += *it;
        }
        CHECK(sum == 55);
    }

    SECTION("Can use an iterator based view setup, using post-fix") {
        const auto view = retro::ranges::views::concat(range1, range2);
        int sum = 0;
        for (auto it = view.begin(); it != view.end(); it++) {
            sum += *it;
        }
        CHECK(sum == 55);
    }

    SECTION("Can concatenate and use with a range adaptor chain") {
        auto view_chain = retro::ranges::views::concat(range1, range2) |
                          retro::ranges::views::filter([](int i) { return i % 2 == 0; });
        int sum = 0;
        for (int i : view_chain) {
            sum += i;
        }
        CHECK(sum == 30);
    }

    SECTION("Can iterate using a value based for loop") {
        auto view = retro::ranges::views::concat(range1, range2);
        int sum = 0;
        for (size_t i = 0; i < view.size(); i++) {
            sum += view[i];
        }
        CHECK(sum == 55);
    }

    SECTION("Can use an iterator based view setup, skipping numbers") {
        auto view = retro::ranges::views::concat(range1, range2);
        int sum = 0;
        for (auto it = view.begin(); it != view.end(); it += 2) {
            sum += *it;
        }
        CHECK(sum == 25);
    }

    SECTION("Can iterate a collection in reverse") {
        int sum = 0;
        for (auto i : std::ranges::views::reverse(retro::ranges::views::concat(range1, range2))) {
            sum += i;
        }
        CHECK(sum == 55);
    }

    SECTION("Can iterate a collection in reverse, using a post-fix") {
        auto view = retro::ranges::views::concat(range1, range2);
        int sum = 0;
        for (auto it = view.end() - 1; it != view.begin(); it--) {
            sum += *it;
        }
        CHECK(sum == 54);
    }

    SECTION("Can iterate a collection in reverse, skipping steps") {
        auto reversed = std::ranges::views::reverse(retro::ranges::views::concat(range1, range2));
        int sum = 0;
        for (auto it = reversed.begin(); it != reversed.end(); it += 2) {
            sum += *it;
        }
        CHECK(sum == 30);
    }

    SECTION("Can get distance between iterators") {
        auto view = retro::ranges::views::concat(range1, range2);
        auto iterator1 = view.begin();
        auto iterator2 = view.begin() + 2;
        CHECK(std::distance(iterator1, iterator2) == 2);
        CHECK(std::distance(iterator2, iterator1) == -2);

        iterator2 += 5;
        CHECK(std::distance(iterator1, iterator2) == 7);
        CHECK(std::distance(iterator2, iterator1) == -7);

        iterator1 += 6;
        CHECK(std::distance(iterator1, iterator2) == 1);
        CHECK(std::distance(iterator2, iterator1) == -1);

        iterator2 -= 5;
        CHECK(std::distance(iterator1, iterator2) == -4);
        CHECK(std::distance(iterator2, iterator1) == 4);
    }
}

TEST_CASE("Can cache a temporary for use later on in the pipe", "[ranges]") {
    std::array values = {1, 2, 3, 4, 5};
    constexpr auto transformer = [](int value) {
        std::vector<int> v;
        for (int i = 0; i < value; i++) {
            v.push_back(i + 1);
        }
        return v;
    };

    SECTION("Iterating through saves the value") {
        auto view = values | std::ranges::views::transform(transformer) | retro::ranges::views::cache_last;

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
        auto view = values | std::ranges::views::transform(transformer) | retro::ranges::views::cache_last;

        REQUIRE(view.size() == 5);

        CHECK(view.begin() - view.end() == -5);
        CHECK(view.end() - view.begin() == 5);
    }

    SECTION("The value can be accessed in a loop") {
        auto view = values | std::ranges::views::transform(transformer) | retro::ranges::views::cache_last;

        int sum = 0;
        for (auto vec : view) {
            for (int i : vec) {
                sum += i;
            }
        }
        CHECK(sum == 35);
    }

    SECTION("Convert to a vector an cache") {
        // Here we're turning the collection into a span. This would emulate using ranges with a collection
        // type that isn't range-compatible.
        auto view = values | std::ranges::views::transform(transformer) | retro::ranges::views::cache_last |
                    retro::ranges::views::transform([](auto &&vec) { return std::span(vec); }) |
                    std::ranges::views::join;

        int sum = 0;
        for (auto &i : view) {
            sum += i;
        }
        CHECK(sum == 35);
    }
}

TEST_CASE("Can join a string together using a view", "[ranges]") {
    SECTION("Can splice a string together with a character") {
        using namespace std::literals;

        std::array strings = {"This"sv, "is"sv, "a"sv, "test."sv};
        auto joined = strings | retro::ranges::views::join_with(' ') | retro::ranges::to<std::string>();
        CHECK(joined == "This is a test.");
    }

    SECTION("Can splice a string together with a string literal") {
        using namespace std::literals;

        std::array strings = {"1"sv, "2"sv, "3"sv, "4"sv};
        auto constraction = ", "sv;
        auto joined = strings | retro::ranges::views::join_with(constraction) | retro::ranges::to<std::string>();
        CHECK(joined == "1, 2, 3, 4");
    }
}
