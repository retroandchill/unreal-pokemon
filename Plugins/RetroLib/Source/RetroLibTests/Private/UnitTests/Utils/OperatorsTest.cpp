/**
 * @file OperatorsTest.cpp
 * @brief Test for the operator functors.
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
#include <memory>
#endif

TEST_CASE_NAMED(FFunctionalComparisonTest, "RetroLib::Utils::Comparisons", "[utils]") {
    CHECK(Retro::Equals(1, 1));
    CHECK_FALSE(Retro::Equals(1, 2));

    CHECK_FALSE(Retro::NotEqual(1, 1));
    CHECK(Retro::NotEqual(1, 2));

    CHECK_FALSE(Retro::LessThan(1, 0));
    CHECK_FALSE(Retro::LessThan(1, 1));
    CHECK(Retro::LessThan(1, 2));

    CHECK_FALSE(Retro::LessThanOrEqual(1, 0));
    CHECK(Retro::LessThanOrEqual(1, 1));
    CHECK(Retro::LessThanOrEqual(1, 2));

    CHECK(Retro::GreaterThan(1, 0));
    CHECK_FALSE(Retro::GreaterThan(1, 1));
    CHECK_FALSE(Retro::GreaterThan(1, 2));

    CHECK(Retro::GreaterThanOrEqual(1, 0));
    CHECK(Retro::GreaterThanOrEqual(1, 1));
    CHECK_FALSE(Retro::GreaterThanOrEqual(1, 2));

    CHECK(Retro::ThreeWayCompare(1, 0) == std::strong_ordering::greater);
    CHECK(Retro::ThreeWayCompare(1, 1) == std::strong_ordering::equal);
    CHECK(Retro::ThreeWayCompare(1, 2) == std::strong_ordering::less);
}

TEST_CASE_NAMED(FFunctionalArithmeticTest, "RetroLib::Utils::Arithmetic", "[utils]") {
    CHECK(Retro::Add(2, 2) == 4);
    CHECK(Retro::Subtract(6, 2) == 4);
    CHECK(Retro::Multiply(6, 2) == 12);
    CHECK(Retro::Divide(6, 2) == 3);
    CHECK(Retro::Modulo(11, 3) == 2);
}