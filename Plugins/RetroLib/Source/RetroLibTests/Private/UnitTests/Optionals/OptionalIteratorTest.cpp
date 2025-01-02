/**
 * @file OptionalIteratorTest.cpp
 * @brief Test for the optional iterator semantics.
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

#include <ranges>
#endif

#ifdef __UNREAL__
TEST_CASE_NAMED(FOptionalIteratorTest, "RetroLib::Optionals::Iterator", "[optionals]") {
    SECTION("Can use a ranged for loop") {
        TOptional Value = 3;

        int Sum = 0;
        for (auto i : Value) {
            Sum += i;
        }
        CHECK(Sum == 3);
    }

    SECTION("Can use an iterator based for loop") {
        const TOptional Value = 3;

        int Sum = 0;
        for (auto It = begin(Value); It != end(Value); It++) {
            Sum += *It;
        }
        CHECK(Sum == 3);
    }

    SECTION("Can use as part of a range pipe") {
        std::vector<TOptional<int>> Values = {1, {}, 2, 3, {}, {}, {}};
        auto View = Values | Retro::Ranges::Views::Join;

        int Sum = 0;
        for (auto i : View) {
            Sum += i;
        }
        CHECK(Sum == 6);
    }

    SECTION("Can be used to determine size") {
        auto View = Retro::Ranges::Views::Concat(TOptional(1), TOptional<int>(), TOptional(2),
                                                 TOptional<int>(), TOptional<int>(), TOptional(3));
        CHECK(View.size() == 3);
    }
}
#endif