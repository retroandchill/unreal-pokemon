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
        std::optional<int> Optional1;
        std::optional Optional2 = 1;

        CHECK_FALSE(Retro::Optionals::HasValue(Optional1));
        CHECK(Retro::Optionals::HasValue(Optional2));

        CHECK_THROWS_AS(Retro::Optionals::GetValue(Optional1), std::bad_optional_access);
        CHECK(Retro::Optionals::GetValue(Optional2) == 1);
        CHECK(Retro::Optionals::Get(Optional2) == 1);
    }

    SECTION("Can convert an optional to a reference wrapped value") {
        std::optional Optional = 1;
        decltype(auto) Wrapped = Retro::Optionals::MakeOptionalReference(Optional);
        CHECK(Optional.value() == Wrapped.value());
        CHECK(&Optional.value() == &Wrapped.value().get());

        decltype(auto) Wrapped2 = Retro::Optionals::MakeOptionalReference(Wrapped);
        CHECK(&Wrapped == &Wrapped2);

        constexpr std::optional Optional2 = 4;
        decltype(auto) Wrapped3 = Retro::Optionals::MakeOptionalReference(Optional2);
        CHECK(Optional2.value() == Wrapped3.value());
        CHECK(&Optional2.value() == &Wrapped3.value().get());

        decltype(auto) Wrapped4 =
            Retro::Optionals::MakeOptionalReference(Retro::Optionals::MakeOptionalReference(Optional2));
        CHECK(Wrapped4.value() == Wrapped3.value());

        std::optional<int> Optional3;
        decltype(auto) Wrapped5 = Retro::Optionals::MakeOptionalReference(Optional3);
        CHECK_FALSE(Wrapped5.has_value());
        decltype(auto) Wrapped6 = Retro::Optionals::MakeOptionalReference(std::as_const(Optional3));
        CHECK_FALSE(Wrapped6.has_value());
    }
}