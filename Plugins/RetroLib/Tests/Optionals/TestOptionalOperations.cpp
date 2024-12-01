/**
 * @file TestOptionalOperations.cpp
 * @brief Test for validation the optional operations work as intended.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#include <catch2/catch_test_macros.hpp>

#if RETROLIB_WITH_MODULES
import std;
import RetroLib;
#else
#include "RetroLib/Optionals/OptionalOperations.h"
#endif

TEST_CASE("Can perform basic operations on optional values", "[optionals]") {
    SECTION("Can get the values out of an optional correctly") {
        std::optional<int> optional1;
        std::optional optional2 = 1;

        CHECK_FALSE(retro::optionals::has_value(optional1));
        CHECK(retro::optionals::has_value(optional2));

        CHECK_THROWS_AS(retro::optionals::get_value(optional1), std::bad_optional_access);
        CHECK(retro::optionals::get_value(optional2) == 1);
        CHECK(retro::optionals::get(optional2) == 1);
    }

    SECTION("Can convert an optional to a reference wrapped value") {
        std::optional optional = 1;
        decltype(auto) wrapped = retro::optionals::make_optional_reference(optional);
        CHECK(optional.value() == wrapped.value());
        CHECK(&optional.value() == &wrapped.value().get());

        decltype(auto) wrapped2 = retro::optionals::make_optional_reference(wrapped);
        CHECK(&wrapped == &wrapped2);

        constexpr std::optional optional2 = 4;
        decltype(auto) wrapped3 = retro::optionals::make_optional_reference(optional2);
        CHECK(optional2.value() == wrapped3.value());
        CHECK(&optional2.value() == &wrapped3.value().get());

        decltype(auto) wrapped4 =
            retro::optionals::make_optional_reference(retro::optionals::make_optional_reference(optional2));
        CHECK(wrapped4.value() == wrapped3.value());

        std::optional<int> optional3;
        decltype(auto) wrapped5 = retro::optionals::make_optional_reference(optional3);
        CHECK_FALSE(wrapped5.has_value());
        decltype(auto) wrapped6 = retro::optionals::make_optional_reference(std::as_const(optional3));
        CHECK_FALSE(wrapped6.has_value());
    }
}