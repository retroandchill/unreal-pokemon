/**
 * @file RangesTerminalClosureTest.cpp
 * @brief Tests for range terminal pipe operations
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#include "TestAdapter.h"

#if RETROLIB_WITH_MODULES
import std;
import RetroLib;
#else
#include "RetroLib.h"

#include <array>
#include <optional>
#include <vector>
#include <map>
#include <set>
#endif

TEST_CASE_NAMED(FRangeToTest, "Retro::Ranges::Algorithm::To", "[ranges]") {
    static constexpr std::array Values = {1, 2, 3, 4, 5};
    SECTION("Convert to a like range type") {
        auto Vectored = Values | Retro::Ranges::To<std::vector>();
        CHECK(Vectored == std::vector({1, 2, 3, 4, 5}));
        CHECK(Retro::Ranges::ContainerCapacity(Vectored) == 5);
        CHECK(Retro::Ranges::ContainerMaxSize(Vectored) == Vectored.max_size());
    }

    SECTION("Can convert between unlike, but compatible, range types") {
        auto Vectored = Values | Retro::Ranges::To<std::vector<double>>();
        CHECK(Vectored == std::vector({1.0, 2.0, 3.0, 4.0, 5.0}));
        CHECK(Retro::Ranges::ContainerCapacity(Vectored) == 5);
        CHECK(Retro::Ranges::ContainerMaxSize(Vectored) == Vectored.max_size());
    }

    SECTION("Can convert a vector into a set") {
        static constexpr std::array Duplicated = {1, 1, 2, 2, 2, 2, 3, 3, 3, 3};
        auto AsSet = Duplicated | Retro::Ranges::To<std::set>();
        CHECK(AsSet == std::set({1, 2, 3}));
    }

    SECTION("Can convert a range into a map using pairs") {
        static constexpr std::array Pairs = {std::make_pair(1, 2), std::make_pair(3, 4)};
        auto AsMap = Pairs | Retro::Ranges::To<std::map<int, int>>();
        CHECK(AsMap == std::map<int, int>({{1, 2}, {3, 4}}));
    }
}

TEST_CASE_NAMED(FRangeForEachTest, "Retro::Ranges::Algorithm::ForEach", "[ranges]") {
    static constexpr std::array Values = {1, 2, 3, 4, 5};
    SECTION("Can iterate over the values of a collection") {
        std::vector<int> Vectored;
        Values | Retro::Ranges::ForEach([&Vectored](int value) { Vectored.push_back(value); });
        CHECK(Vectored == std::vector({1, 2, 3, 4, 5}));

        constexpr auto AddToVector = [](int value, std::vector<int> &vector) { vector.push_back(value); };
        Vectored.clear();
        Values | Retro::Ranges::ForEach<AddToVector>(std::ref(Vectored));
        CHECK(Vectored == std::vector({1, 2, 3, 4, 5}));
    }

    SECTION("Can iterate over a range of pairs using a two arg functor") {
        static constexpr std::array Pairs = {std::make_pair(1, 2), std::make_pair(3, 4)};
        std::map<int, int> AsMap;
        Pairs | Retro::Ranges::ForEach([&AsMap](int key, int value) { AsMap[key] = value; });
        CHECK(AsMap == std::map<int, int>({{1, 2}, {3, 4}}));
    }
}

TEST_CASE_NAMED(FRangeReduceTest, "Retro::Ranges::Algorithm::Reduce", "[ranges]") {
    static constexpr std::array Values = {1, 2, 3, 4, 5};
    SECTION("Can reduce a range to a single value using a runtime binding") {
        auto Result = Values | Retro::Ranges::Reduce(0, Retro::Add);
        CHECK(Result == 15);
    }

    SECTION("Can reduce a range to a single value using a constexpr binding") {
        auto Result = Values | Retro::Ranges::Reduce<Retro::Add>(0);
        CHECK(Result == 15);
    }
}

TEST_CASE_NAMED(FRangeFindFirstTest, "Retro::Ranges::Algorithm::FindFirst", "[ranges]") {
    static constexpr std::array Values = {1, 2, 3, 4, 5};
    constexpr auto IsMultipleOf = [](int i, int j) { return i % j == 0; };
    SECTION("Can reduce using an inferred optional value") {
        auto ValidResult = Values | Retro::Ranges::Views::Filter<IsMultipleOf>(3) | Retro::Ranges::FindFirst();
        CHECK(Retro::Optionals::Get(ValidResult) == 3);

        auto InvalidResult = Values | Retro::Ranges::Views::Filter<IsMultipleOf>(10) | Retro::Ranges::FindFirst();
        CHECK_FALSE(Retro::Optionals::HasValue(InvalidResult) );
    }

    SECTION("Can reduce using an inferred template parameter") {
        auto ValidResult =
            Values | Retro::Ranges::Views::Filter<IsMultipleOf>(3) | Retro::Ranges::FindFirst<std::optional>();
        REQUIRE(ValidResult.has_value());
        CHECK(ValidResult->get() == 3);

        auto InvalidResult =
            Values | Retro::Ranges::Views::Filter<IsMultipleOf>(10) | Retro::Ranges::FindFirst<std::optional>();
        CHECK_FALSE(InvalidResult.has_value());
    }

    SECTION("Can reduce using an explicit template parameter") {
        auto ValidResult = Values | Retro::Ranges::Views::Filter<IsMultipleOf>(3) |
                            Retro::Ranges::FindFirst<std::optional<int>>();
        CHECK(ValidResult == 3);

        auto InvalidResult =
            Values | Retro::Ranges::Views::Filter<IsMultipleOf>(10) | Retro::Ranges::FindFirst<std::optional<int>>();
        CHECK_FALSE(InvalidResult.has_value());
    }
}

TEST_CASE_NAMED(FRangeAllOf, "Retro::Ranges::Algorithm::AllOf", "[ranges]") {
    static constexpr std::array Values = {1, 2, 3, 4, 5};
    SECTION("Check the all of condition") {
        CHECK(Values | Retro::Ranges::AllOf(Retro::GreaterThan, 0));
        CHECK(Values | Retro::Ranges::AllOf<Retro::GreaterThan>(0));
        CHECK_FALSE(Values | Retro::Ranges::AllOf(Retro::GreaterThan, 10));
        CHECK_FALSE(Values | Retro::Ranges::AllOf<Retro::GreaterThan>(20));
    }

    SECTION("Check the none of condition") {
        CHECK_FALSE(Values | Retro::Ranges::NoneOf(Retro::GreaterThan, 0));
        CHECK_FALSE(Values | Retro::Ranges::NoneOf<Retro::GreaterThan>(0));
        CHECK(Values | Retro::Ranges::NoneOf(Retro::GreaterThan, 10));
        CHECK(Values | Retro::Ranges::NoneOf<Retro::GreaterThan>(20));
    }

    SECTION("Check the any of condition") {
        CHECK(Values | Retro::Ranges::AnyOf(Retro::GreaterThan, 2));
        CHECK(Values | Retro::Ranges::AnyOf<Retro::GreaterThan>(2));
        CHECK_FALSE(Values | Retro::Ranges::AnyOf(Retro::GreaterThan, 10));
        CHECK_FALSE(Values | Retro::Ranges::AnyOf<Retro::GreaterThan>(20));
    }
}