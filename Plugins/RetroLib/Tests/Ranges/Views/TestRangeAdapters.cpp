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
#include "RetroLib/Ranges/Algorithm/To.h"
#include "RetroLib/Ranges/Views/CacheLast.h"
#include "RetroLib/Ranges/Views/Concat.h"
#include "RetroLib/Ranges/Views/Elements.h"
#include "RetroLib/Ranges/Views/Enumerate.h"
#include "RetroLib/Ranges/Views/Filter.h"
#include "RetroLib/Ranges/Views/JoinWith.h"
#include "RetroLib/Ranges/Views/Transform.h"
#include "RetroLib/Utils/Tuple.h"

#include <array>
#include <map>
#include <string>
#include <string_view>
#include <vector>
#endif

TEST_CASE("Can use the standard range adapters", "[ranges]") {
    constexpr std::array values = {1, 2, 3, 4, 5};
    constexpr auto is_even = [](int i) { return i % 2 == 0; };
    constexpr auto double_value = [](int i, int j) { return i * j; };
    auto filterer = retro::ranges::views::filter(is_even);

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

TEST_CASE("Can get the elements of a tupled collection", "[ranges]") {
    SECTION("Can get the value of any tuple based collection") {
        std::array<std::tuple<int, int, int>, 3> tuples = {
            {std::make_tuple(1, 2, 3), std::make_tuple(4, 5, 6), std::make_tuple(7, 8, 9)}};

        auto value_view = tuples | retro::ranges::views::elements<2>;
        int sum = 0;
        for (auto value : value_view) {
            sum += value;
        }
        CHECK(sum == 18);

        static_assert(std::ranges::sized_range<decltype(value_view)>);
        CHECK(std::ranges::size(tuples) == std::ranges::size(value_view));

        static_assert(std::ranges::random_access_range<decltype(value_view)>);
        auto reversed = value_view | std::ranges::views::reverse;
        sum = 0;
        for (auto value : reversed) {
            sum += value;
        }
        CHECK(sum == 18);

        CHECK(value_view[1] == 6);
        CHECK((value_view.end() - value_view.begin()) == value_view.size());
    }

    SECTION("Can get the keys of a map") {
        std::map<int, std::string> map = {{1, "One"}, {2, "Two"}, {3, "Three"}};

        auto keys_view = map | retro::ranges::views::keys;
        int sum = 0;
        for (auto key : keys_view) {
            sum += key;
        }
        CHECK(sum == 6);
    }

    SECTION("Can get the values of a map") {
        std::map<int, std::string> map = {{1, "One"}, {2, "Two"}, {3, "Three"}};
        auto values_view = map | retro::ranges::views::values | retro::ranges::views::join_with(", ") |
                           retro::ranges::to<std::string>();

        CHECK(values_view == "One, Two, Three");
    }
}

TEST_CASE("Can enumerate over a collection with its index", "[ranges]") {
    constexpr static auto input = {'A', 'B', 'C', 'D'};

    SECTION("Can enumerate using a function call") {
        std::vector<std::tuple<size_t, char>> values;
        for (const auto [index, letter] : retro::ranges::views::enumerate(input)) {
            values.emplace_back(index, letter);
        }
        CHECK(values == std::vector<std::tuple<size_t, char>>{{0, 'A'}, {1, 'B'}, {2, 'C'}, {3, 'D'}});
    }

    SECTION("Can enumerate into a map") {
        auto as_map = input | retro::ranges::views::enumerate |
                      retro::ranges::views::transform(retro::convert_tuple<std::pair>) | retro::ranges::to<std::map>();
        CHECK(as_map ==
              std::map<std::ranges::range_difference_t<decltype(input)>, char>{{0, 'A'}, {1, 'B'}, {2, 'C'}, {3, 'D'}});
    }

    SECTION("Can propagate a constant reference and increment the values accordingly") {
        std::vector numbers = {1, 3, 5, 7};

        std::vector<int> output;
        for (auto [index, num] : retro::ranges::views::enumerate(numbers)) {
            ++num;
            output.emplace_back(numbers[index]);
        }
        CHECK(output == std::vector<int>{2, 4, 6, 8});
    }
}

TEST_CASE("Can enumerate in reverse mapping indices to elements", "[ranges]") {
    constexpr static std::array input = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L'};

    SECTION("Can reverse enumerate using some generated indices") {
        auto pairs = std::ranges::views::iota(static_cast<size_t>(6), input.size()) |
                     retro::ranges::views::reverse_enumerate(input);

        int count = 0;
        for (auto [index, letter] : pairs) {
            CHECK(letter == input[index]);
            count++;
        }
        CHECK(count == 6);
    }

    SECTION("Can reverse enumerate into a map") {
        auto pairs = std::ranges::views::iota(2, 5) | retro::ranges::views::reverse_enumerate(input) |
                     retro::ranges::views::transform(retro::convert_tuple<std::pair>) | retro::ranges::to<std::map>();

        CHECK(pairs == std::map<int, char>{{2, 'C'}, {3, 'D'}, {4, 'E'}});
    }
}