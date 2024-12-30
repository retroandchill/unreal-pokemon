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
#include "RetroLib/Ranges/Views/NameAliases.h"

#include <array>
#include <map>
#include <string>
#include <string_view>
#include <vector>
#endif

TEST_CASE("Can use the standard range adapters", "[ranges]") {
    constexpr std::array Values = {1, 2, 3, 4, 5};
    constexpr auto IsEven = [](int i) { return i % 2 == 0; };
    constexpr auto DoubleValue = [](int i, int j) { return i * j; };

    SECTION("Can use the regular functional operators") {
        auto Filtered = Retro::Ranges::Views::Filter(Values, IsEven);
        auto Transformed = Retro::Ranges::Views::Transform(Filtered, DoubleValue, 2);

        auto It = Transformed.begin();
        CHECK(*It == 4);
        ++It;
        CHECK(*It == 8);
        ++It;
        CHECK(It == Transformed.end());
    }

    SECTION("Can use the range pipe syntax") {
        auto Transformed =
            Values | Retro::Ranges::Views::Filter(IsEven) | Retro::Ranges::Views::Transform(DoubleValue, 2);

        auto It = Transformed.begin();
        CHECK(*It == 4);
        ++It;
        CHECK(*It == 8);
        ++It;
        CHECK(It == Transformed.end());
    }
}

TEST_CASE("Can use the constexpr range adapters", "[ranges]") {
    constexpr std::array Values = {1, 2, 3, 4, 5};
    constexpr auto IsEven = [](int i) { return i % 2 == 0; };
    constexpr auto DoubleValue = [](int i, int j) { return i * j; };

    SECTION("Can use the regular functional operators") {
        auto Filtered = Retro::Ranges::Views::Filter<IsEven>(Values);
        auto Transformed = Retro::Ranges::Views::Transform<DoubleValue>(Filtered, 2);

        auto It = Transformed.begin();
        CHECK(*It == 4);
        ++It;
        CHECK(*It == 8);
        ++It;
        CHECK(It == Transformed.end());
    }

    SECTION("Can use the range pipe syntax") {
        auto Transformed =
            Values | Retro::Ranges::Views::Filter<IsEven>() | Retro::Ranges::Views::Transform<DoubleValue>(2);

        auto It = Transformed.begin();
        CHECK(*It == 4);
        ++It;
        CHECK(*It == 8);
        ++It;
        CHECK(It == Transformed.end());
    }
}

TEST_CASE("Can concatenate two unlike ranges into a single unified view", "[ranges]") {
    std::array Range1 = {1, 2, 3, 4, 5};
    std::vector Range2 = {6, 7, 8, 9, 10};

    SECTION("Can concatenate and use with a ranged for loop") {
        int Sum = 0;
        for (decltype(auto) i : Retro::Ranges::Views::Concat(Range1, Range2)) {
            Sum += i;
        }
        CHECK(Sum == 55);
    }

    SECTION("Can use an iterator based view setup") {
        auto View = Retro::Ranges::Views::Concat(Range1, Range2);
        int Sum = 0;
        for (auto It = View.begin(); It != View.end(); ++It) {
            Sum += *It;
        }
        CHECK(Sum == 55);
    }

    SECTION("Can use an iterator based view setup, using post-fix") {
        const auto View = Retro::Ranges::Views::Concat(Range1, Range2);
        int Sum = 0;
        for (auto It = View.begin(); It != View.end(); It++) {
            Sum += *It;
        }
        CHECK(Sum == 55);
    }

    SECTION("Can concatenate and use with a range adaptor chain") {
        auto ViewChain = Retro::Ranges::Views::Concat(Range1, Range2) |
                          Retro::Ranges::Views::Filter([](int i) { return i % 2 == 0; });
        int Sum = 0;
        for (int i : ViewChain) {
            Sum += i;
        }
        CHECK(Sum == 30);
    }

    SECTION("Can iterate using a value based for loop") {
        auto View = Retro::Ranges::Views::Concat(Range1, Range2);
        int Sum = 0;
        for (size_t i = 0; i < View.size(); i++) {
            Sum += View[i];
        }
        CHECK(Sum == 55);
    }

    SECTION("Can use an iterator based view setup, skipping numbers") {
        auto View = Retro::Ranges::Views::Concat(Range1, Range2);
        int Sum = 0;
        for (auto It = View.begin(); It != View.end(); It += 2) {
            Sum += *It;
        }
        CHECK(Sum == 25);
    }

    SECTION("Can iterate a collection in reverse") {
        int Sum = 0;
        for (auto i : std::ranges::views::reverse(Retro::Ranges::Views::Concat(Range1, Range2))) {
            Sum += i;
        }
        CHECK(Sum == 55);
    }

    SECTION("Can iterate a collection in reverse, using a post-fix") {
        auto View = Retro::Ranges::Views::Concat(Range1, Range2);
        int Sum = 0;
        for (auto It = View.end() - 1; It != View.begin(); It--) {
            Sum += *It;
        }
        CHECK(Sum == 54);
    }

    SECTION("Can iterate a collection in reverse, skipping steps") {
        auto Reversed = std::ranges::views::reverse(Retro::Ranges::Views::Concat(Range1, Range2));
        int Sum = 0;
        for (auto It = Reversed.begin(); It != Reversed.end(); It += 2) {
            Sum += *It;
        }
        CHECK(Sum == 30);
    }

    SECTION("Can get distance between iterators") {
        auto View = Retro::Ranges::Views::Concat(Range1, Range2);
        auto Iterator1 = View.begin();
        auto Iterator2 = View.begin() + 2;
        CHECK(std::distance(Iterator1, Iterator2) == 2);
        CHECK(std::distance(Iterator2, Iterator1) == -2);

        Iterator2 += 5;
        CHECK(std::distance(Iterator1, Iterator2) == 7);
        CHECK(std::distance(Iterator2, Iterator1) == -7);

        Iterator1 += 6;
        CHECK(std::distance(Iterator1, Iterator2) == 1);
        CHECK(std::distance(Iterator2, Iterator1) == -1);

        Iterator2 -= 5;
        CHECK(std::distance(Iterator1, Iterator2) == -4);
        CHECK(std::distance(Iterator2, Iterator1) == 4);
    }
}

TEST_CASE("Can cache a temporary for use later on in the pipe", "[ranges]") {
    std::array Values = {1, 2, 3, 4, 5};
    constexpr auto Transformer = [](int Value) {
        std::vector<int> v;
        for (int i = 0; i < Value; i++) {
            v.push_back(i + 1);
        }
        return v;
    };

    SECTION("Iterating through saves the value") {
        auto View = Values | std::ranges::views::transform(Transformer) | Retro::Ranges::Views::CacheLast;

        REQUIRE(View.size() == 5);

        auto It = View.begin();
        CHECK((*It).size() == 1);
        It++;
        CHECK((*It).size() == 2);
        // Verify that a repeat read doesn't update the cache
        CHECK((*It).size() == 2);
        It++;
        CHECK((*It).size() == 3);
        It++;
        CHECK((*It).size() == 4);
        It++;
        CHECK((*It).size() == 5);
        It++;
        CHECK(It == View.end());
    }

    SECTION("Can get the difference between iterators") {
        auto View = Values | std::ranges::views::transform(Transformer) | Retro::Ranges::Views::CacheLast;

        REQUIRE(View.size() == 5);

        CHECK(View.begin() - View.end() == -5);
        CHECK(View.end() - View.begin() == 5);
    }

    SECTION("The value can be accessed in a loop") {
        auto View = Values | std::ranges::views::transform(Transformer) | Retro::Ranges::Views::CacheLast;

        int Sum = 0;
        for (auto vec : View) {
            for (int i : vec) {
                Sum += i;
            }
        }
        CHECK(Sum == 35);
    }

    SECTION("Convert to a vector an cache") {
        // Here we're turning the collection into a span. This would emulate using ranges with a collection
        // type that isn't range-compatible.
        auto View = Values | std::ranges::views::transform(Transformer) | Retro::Ranges::Views::CacheLast |
                    Retro::Ranges::Views::Transform([](auto &&vec) { return std::span(vec); }) |
                    Retro::Ranges::Views::Join;

        int Sum = 0;
        for (auto &i : View) {
            Sum += i;
        }
        CHECK(Sum == 35);
    }
}

TEST_CASE("Can join a string together using a view", "[ranges]") {
    SECTION("Can splice a string together with a character") {
        using namespace std::literals;

        std::array Strings = {"This"sv, "is"sv, "a"sv, "test."sv};
        auto Joined = Strings | Retro::Ranges::Views::JoinWith(' ') | Retro::Ranges::To<std::string>();
        CHECK(Joined == "This is a test.");
    }

    SECTION("Can splice a string together with a string literal") {
        using namespace std::literals;

        std::array Strings = {"1"sv, "2"sv, "3"sv, "4"sv};
        auto Constraction = ", "sv;
        auto Joined = Strings | Retro::Ranges::Views::JoinWith(Constraction) | Retro::Ranges::To<std::string>();
        CHECK(Joined == "1, 2, 3, 4");
    }
}

TEST_CASE("Can get the elements of a tupled collection", "[ranges]") {
    SECTION("Can get the value of any tuple based collection") {
        std::array<std::tuple<int, int, int>, 3> Tuples = {
            {std::make_tuple(1, 2, 3), std::make_tuple(4, 5, 6), std::make_tuple(7, 8, 9)}};

        auto ValueView = Tuples | Retro::Ranges::Views::Elements<2>;
        int Sum = 0;
        for (auto Value : ValueView) {
            Sum += Value;
        }
        CHECK(Sum == 18);

        static_assert(std::ranges::sized_range<decltype(ValueView)>);
        CHECK(std::ranges::size(Tuples) == std::ranges::size(ValueView));

        static_assert(std::ranges::random_access_range<decltype(ValueView)>);
        auto Reversed = ValueView | std::ranges::views::reverse;
        Sum = 0;
        for (auto Value : Reversed) {
            Sum += Value;
        }
        CHECK(Sum == 18);

        CHECK(ValueView[1] == 6);
        CHECK((ValueView.end() - ValueView.begin()) == ValueView.size());
    }

    SECTION("Can get the keys of a map") {
        std::map<int, std::string> Map = {{1, "One"}, {2, "Two"}, {3, "Three"}};

        auto KeysView = Map | Retro::Ranges::Views::Keys;
        int Sum = 0;
        for (auto Key : KeysView) {
            Sum += Key;
        }
        CHECK(Sum == 6);
    }

    SECTION("Can get the values of a map") {
        std::map<int, std::string> Map = {{1, "One"}, {2, "Two"}, {3, "Three"}};
        auto ValuesView = Map | Retro::Ranges::Views::Values | Retro::Ranges::Views::JoinWith(", ") |
                           Retro::Ranges::To<std::string>();

        CHECK(ValuesView == "One, Two, Three");
    }
}

TEST_CASE("Can enumerate over a collection with its index", "[ranges]") {
    std::array Input = {'A', 'B', 'C', 'D'};

    SECTION("Can enumerate using a function call") {
        std::vector<std::tuple<size_t, char>> Values;
        for (const auto [index, letter] : Retro::Ranges::Views::Enumerate(Input)) {
            Values.emplace_back(index, letter);
        }
        CHECK(Values == std::vector<std::tuple<size_t, char>>{{0, 'A'}, {1, 'B'}, {2, 'C'}, {3, 'D'}});
    }

    SECTION("Can enumerate into a map") {
        auto AsMap = Retro::Ranges::To<std::map>(Input | Retro::Ranges::Views::Enumerate);
        CHECK(AsMap ==
              std::map<std::ranges::range_difference_t<decltype(Input)>, char>{{0, 'A'}, {1, 'B'}, {2, 'C'}, {3, 'D'}});
    }

    SECTION("Can propagate a constant reference and increment the values accordingly") {
        std::vector Numbers = {1, 3, 5, 7};

        std::vector<int> Output;
        for (auto [index, num] : Retro::Ranges::Views::Enumerate(Numbers)) {
            ++num;
            Output.emplace_back(Numbers[index]);
        }
        CHECK(Output == std::vector<int>{2, 4, 6, 8});
    }
}

TEST_CASE("Can enumerate in reverse mapping indices to elements", "[ranges]") {
    constexpr static std::array Input = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L'};

    SECTION("Can reverse enumerate using some generated indices") {
        auto Pairs = std::ranges::views::iota(static_cast<size_t>(6), Input.size()) |
                     Retro::Ranges::Views::ReverseEnumerate(Input);

        int Count = 0;
        for (auto [index, letter] : Pairs) {
            CHECK(letter == Input[index]);
            Count++;
        }
        CHECK(Count == 6);
    }

    SECTION("Can reverse enumerate into a map") {
        auto Pairs = std::ranges::views::iota(2, 5) | Retro::Ranges::Views::ReverseEnumerate(Input) |
                     Retro::Ranges::Views::Transform(Retro::ConvertTuple<std::pair>) | Retro::Ranges::To<std::map>();

        CHECK(Pairs == std::map<int, char>{{2, 'C'}, {3, 'D'}, {4, 'E'}});
    }
}