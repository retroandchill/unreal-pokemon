/**
 * @file OperatorsTest.cpp
 * @brief Test for the operator functors.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#include <catch2/catch_test_macros.hpp>

#if RETROLIB_WITH_MODULES
import std;
import RetroLib;
#else
#include "RetroLib/Utils/Operators.h"

#include <array>
#include <memory>
#endif

TEST_CASE("Can use the operators to perform comparisons", "[utils]") {
    CHECK(retro::equals(1, 1));
    CHECK_FALSE(retro::equals(1, 2));

    CHECK_FALSE(retro::not_equal(1, 1));
    CHECK(retro::not_equal(1, 2));

    CHECK_FALSE(retro::less_than(1, 0));
    CHECK_FALSE(retro::less_than(1, 1));
    CHECK(retro::less_than(1, 2));

    CHECK_FALSE(retro::less_than_or_equal(1, 0));
    CHECK(retro::less_than_or_equal(1, 1));
    CHECK(retro::less_than_or_equal(1, 2));

    CHECK(retro::greater_than(1, 0));
    CHECK_FALSE(retro::greater_than(1, 1));
    CHECK_FALSE(retro::greater_than(1, 2));

    CHECK(retro::greater_than_or_equal(1, 0));
    CHECK(retro::greater_than_or_equal(1, 1));
    CHECK_FALSE(retro::greater_than_or_equal(1, 2));

    CHECK(retro::three_way_compare(1, 0) == std::strong_ordering::greater);
    CHECK(retro::three_way_compare(1, 1) == std::strong_ordering::equal);
    CHECK(retro::three_way_compare(1, 2) == std::strong_ordering::less);
}

TEST_CASE("Can perform basic math operations", "[utils]") {
    CHECK(retro::add(2, 2) == 4);
    CHECK(retro::subtract(6, 2) == 4);
    CHECK(retro::multiply(6, 2) == 12);
    CHECK(retro::divide(6, 2) == 3);
    CHECK(retro::modulo(11, 3) == 2);
}