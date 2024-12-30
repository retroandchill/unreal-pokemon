/**
 * @file TestOptional.cpp
 * @brief Test for the custom optional type.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#include <catch2/catch_test_macros.hpp>

#if RETROLIB_WITH_MODULES
import std;
import RetroLib;
#else
#include "RetroLib/Optionals/Optional.h"

#include <string>
#endif

TEST_CASE("Can use the new optional type", "[optionals]") {
    SECTION("Can create and assign optionals") {
        Retro::Optional<int> Optional1;
        CHECK_FALSE(Optional1.HasValue());
        CHECK(Optional1 == std::nullopt);
        CHECK(std::nullopt == Optional1);
        CHECK(Optional1 <= std::nullopt);
        CHECK(std::nullopt <= Optional1);
        CHECK(Optional1 >= std::nullopt);
        CHECK(std::nullopt >= Optional1);
        CHECK_THROWS_AS(Optional1.Value(), std::bad_optional_access);
        CHECK_THROWS_AS(std::as_const(Optional1.Value()), std::bad_optional_access);
        CHECK_THROWS_AS(std::move(Optional1).Value(), std::bad_optional_access);

        Retro::Optional Optional2 = 3;
        CHECK(Optional2.HasValue());
        CHECK(Optional2 != std::nullopt);
        CHECK(std::nullopt != Optional2);
        CHECK(Optional2 > std::nullopt);
        CHECK(std::nullopt < Optional2);
        CHECK(Optional2 == 3);
        CHECK(Optional2.Value() == 3);
        CHECK(*Optional2 == 3);

        CHECK(Optional1.ValueOr(4) == 4);
        CHECK(std::as_const(Optional1).ValueOr(4) == 4);
        CHECK(Optional2.ValueOr(4) == 3);
        CHECK(std::as_const(Optional2).ValueOr(4) == 3);

        Retro::Optional<std::string> Optional3 = "Hello";
        CHECK(Optional3.HasValue());
        CHECK(Optional3 == "Hello");
        CHECK(std::as_const(Optional3).Value() == "Hello");
        CHECK(std::move(Optional3).Value() == "Hello");
        CHECK(*std::as_const(Optional3) == "Hello");
        CHECK(*std::move(Optional3) == "Hello");

        Optional3.Reset();
        CHECK_FALSE(Optional3.HasValue());

        Optional3.Emplace("New string");
        CHECK(Optional3.HasValue());
        CHECK(Optional3 != "Hello");
        CHECK(Optional3 == "New string");
        CHECK(Optional3->length() == 10);
        CHECK(std::as_const(Optional3)->length() == 10);

        auto Optional4 = std::move(Optional3);
        REQUIRE(Optional3.HasValue());
        CHECK(Optional3->empty());

        auto Optional5 = Optional4;
        REQUIRE(Optional4.HasValue());
        CHECK(Optional4 == "New string");

        Optional3 = std::nullopt;
        CHECK_FALSE(Optional3.HasValue());

        auto Optional6 = Optional3;
        CHECK_FALSE(Optional6.HasValue());
        auto Optional7 = std::move(Optional3);
        CHECK_FALSE(Optional3.HasValue());

        Optional4 = Optional3;
        CHECK_FALSE(Optional3.HasValue());
        Optional5 = std::move(Optional3);
        CHECK_FALSE(Optional3.HasValue());

        Optional3.Emplace("New value");
        Optional6 = Optional3;
        CHECK(Optional6.HasValue());
        Optional5 = "Test string";
        Optional5 = std::move(Optional3);
        CHECK(Optional5.HasValue());
    }

    SECTION("Can compare optionals between each other") {
        Retro::Optional<int> Optional1 = std::nullopt;
        Retro::Optional Optional2 = 3;
        static_assert(Retro::EqualityComparable<int, int>);
        static_assert(Retro::EqualityComparable<decltype(Optional1)::ValueType, decltype(Optional2)::ValueType>);
        CHECK_FALSE(Optional1 == Optional2);
        CHECK(Optional1 < Optional2);
        CHECK(Optional2 > Optional1);

        Retro::Optional Optional3 = 7.0;
        CHECK(Optional1 != Optional3);
        CHECK(Optional2 != Optional3);
        CHECK(Optional1 < Optional3);
        CHECK(Optional3 > Optional1);
        CHECK(Optional2 < Optional3);
        Optional3 = 3.0;
        CHECK(Optional2 == Optional3);
    }

    SECTION("Swap operation works") {
        Retro::Optional Optional1 = 3;
        Retro::Optional Optional2 = 7;
        swap(Optional1, Optional2);
        CHECK(Optional1 == 7);
        CHECK(Optional2 == 3);

        Retro::Optional<std::string> Optional3 = "Hello";
        Retro::Optional<std::string> Optional4 = "World";
        swap(Optional3, Optional4);
        CHECK(Optional3 == "World");
        CHECK(Optional4 == "Hello");

        Optional3.Reset();
        swap(Optional3, Optional4);
        CHECK_FALSE(Optional4.HasValue());
        CHECK(Optional3 == "Hello");

        Optional3.Reset();
        swap(Optional3, Optional4);
        CHECK_FALSE(Optional3.HasValue());
        CHECK_FALSE(Optional4.HasValue());

        Optional3 = "Hello world";
        std::string Value = *std::move(Optional3);
        CHECK(Value == "Hello world");
    }

    SECTION("Can have an optional of a reference") {
        Retro::Optional Optional1 = 3;
        auto Optional2 = Retro::Optionals::MakeOptionalReference(Optional1);
        CHECK(Optional2.HasValue());
        CHECK(*Optional2 == 3);
        CHECK(std::addressof(std::as_const(*Optional2)) == std::addressof(*Optional1));

        Optional2.Reset();
        CHECK_FALSE(Optional2.HasValue());

        int FreeVariable = 7;
        Optional2 = FreeVariable;
        CHECK(Optional2.HasValue());
        CHECK(*Optional2 == 7);

        FreeVariable++;
        CHECK(*Optional2 == 8);

        int OtherVariable = 4;
        Retro::Optional<int &> Optional3 = OtherVariable;
        CHECK(Optional3.HasValue());
        swap(Optional2, Optional3);
        CHECK(*Optional3 == 8);
        CHECK(FreeVariable == 4);
        CHECK(OtherVariable == 8);

        Optional3.Reset();
        swap(Optional3, Optional2);
        CHECK(*Optional3 == 4);
        CHECK_FALSE(Optional2.HasValue());
        CHECK(FreeVariable == 4);

        std::string TestString = "Hello world";
        Retro::Optional<std::string &> Optional4 = TestString;
        CHECK(Optional4.HasValue());
        CHECK(Optional4->length() == 11);
        CHECK(std::as_const(Optional4)->length() == 11);
    }
}