/**
 * @file TestBindFront.cpp
 * @brief Test for the constexpr `bind_front` function.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#include <catch2/catch_test_macros.hpp>

#if RETROLIB_WITH_MODULES
import std;
import RetroLib;
#else
#include "RetroLib/Functional/BindFront.h"
#endif

static int add_numbers(int a, int b, int c) {
    return a + b + c;
}

TEST_CASE("Can bind front to a constexpr defined functional type", "[functional]") {
    SECTION("Can bind with one parameter") {
        auto binding = retro::bind_front<&add_numbers>(1);
        CHECK(binding(2, 3) == 6);
        CHECK(std::as_const(binding)(5, 4) == 10);
        CHECK(retro::bind_front<&add_numbers>(3)(5, 4) == 12);
    }

    SECTION("Can bind with two parameters") {
        auto binding = retro::bind_front<&add_numbers>(1, 2);
        CHECK(binding(3) == 6);
        CHECK(std::as_const(binding)(4) == 7);
        CHECK(retro::bind_front<&add_numbers>(3, 6)(5) == 14);
    }

    SECTION("Can bind with three parameters") {
        auto binding = retro::bind_front<&add_numbers>(1, 2, 3);
        CHECK(binding() == 6);
        CHECK(std::as_const(binding)() == 6);
        CHECK(retro::bind_front<&add_numbers>(3, 6, 9)() == 18);
    }
}