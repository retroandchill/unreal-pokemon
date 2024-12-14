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
        retro::Optional<int> optional1;
        CHECK_FALSE(optional1.has_value());
        CHECK(optional1 == std::nullopt);
        CHECK(std::nullopt == optional1);
        CHECK(optional1 <= std::nullopt);
        CHECK(std::nullopt <= optional1);
        CHECK(optional1 >= std::nullopt);
        CHECK(std::nullopt >= optional1);
        CHECK_THROWS_AS(optional1.value(), std::bad_optional_access);
        CHECK_THROWS_AS(std::as_const(optional1.value()), std::bad_optional_access);
        CHECK_THROWS_AS(std::move(optional1).value(), std::bad_optional_access);

        retro::Optional optional2 = 3;
        CHECK(optional2.has_value());
        CHECK(optional2 != std::nullopt);
        CHECK(std::nullopt != optional2);
        CHECK(optional2 > std::nullopt);
        CHECK(std::nullopt < optional2);
        CHECK(optional2 == 3);
        CHECK(optional2.value() == 3);
        CHECK(*optional2 == 3);

        CHECK(optional1.value_or(4) == 4);
        CHECK(std::as_const(optional1).value_or(4) == 4);
        CHECK(optional2.value_or(4) == 3);
        CHECK(std::as_const(optional2).value_or(4) == 3);

        retro::Optional<std::string> optional3 = "Hello";
        CHECK(optional3.has_value());
        CHECK(optional3 == "Hello");
        CHECK(std::as_const(optional3).value() == "Hello");
        CHECK(std::move(optional3).value() == "Hello");
        CHECK(*std::as_const(optional3) == "Hello");
        CHECK(*std::move(optional3) == "Hello");

        optional3.reset();
        CHECK_FALSE(optional3.has_value());

        optional3.emplace("New string");
        CHECK(optional3.has_value());
        CHECK(optional3 != "Hello");
        CHECK(optional3 == "New string");
        CHECK(optional3->length() == 10);
        CHECK(std::as_const(optional3)->length() == 10);

        auto optional4 = std::move(optional3);
        REQUIRE(optional3.has_value());
        CHECK(optional3->empty());

        auto optional5 = optional4;
        REQUIRE(optional4.has_value());
        CHECK(optional4 == "New string");

        optional3 = std::nullopt;
        CHECK_FALSE(optional3.has_value());

        auto optional6 = optional3;
        CHECK_FALSE(optional6.has_value());
        auto optional7 = std::move(optional3);
        CHECK_FALSE(optional3.has_value());

        optional4 = optional3;
        CHECK_FALSE(optional3.has_value());
        optional5 = std::move(optional3);
        CHECK_FALSE(optional3.has_value());

        optional3.emplace("New value");
        optional6 = optional3;
        CHECK(optional6.has_value());
        optional5 = "Test string";
        optional5 = std::move(optional3);
        CHECK(optional5.has_value());
    }

    SECTION("Can compare optionals between each other") {
        retro::Optional<int> optional1 = std::nullopt;
        retro::Optional optional2 = 3;
        static_assert(retro::EqualityComparable<int, int>);
        static_assert(retro::EqualityComparable<decltype(optional1)::ValueType, decltype(optional2)::ValueType>);
        CHECK_FALSE(optional1 == optional2);
        CHECK(optional1 < optional2);
        CHECK(optional2 > optional1);

        retro::Optional optional3 = 7.0;
        CHECK(optional1 != optional3);
        CHECK(optional2 != optional3);
        CHECK(optional1 < optional3);
        CHECK(optional3 > optional1);
        CHECK(optional2 < optional3);
        optional3 = 3.0;
        CHECK(optional2 == optional3);
    }

    SECTION("Swap operation works") {
        retro::Optional optional1 = 3;
        retro::Optional optional2 = 7;
        swap(optional1, optional2);
        CHECK(optional1 == 7);
        CHECK(optional2 == 3);

        retro::Optional<std::string> optional3 = "Hello";
        retro::Optional<std::string> optional4 = "World";
        swap(optional3, optional4);
        CHECK(optional3 == "World");
        CHECK(optional4 == "Hello");

        optional3.reset();
        swap(optional3, optional4);
        CHECK_FALSE(optional4.has_value());
        CHECK(optional3 == "Hello");

        optional3.reset();
        swap(optional3, optional4);
        CHECK_FALSE(optional3.has_value());
        CHECK_FALSE(optional4.has_value());

        optional3 = "Hello world";
        std::string value = *std::move(optional3);
        CHECK(value == "Hello world");
    }

    SECTION("Can have an optional of a reference") {
        retro::Optional optional1 = 3;
        auto optional2 = retro::optionals::make_optional_reference(optional1);
        CHECK(optional2.has_value());
        CHECK(*optional2 == 3);
        CHECK(std::addressof(std::as_const(*optional2)) == std::addressof(*optional1));

        optional2.reset();
        CHECK_FALSE(optional2.has_value());

        int free_variable = 7;
        optional2 = free_variable;
        CHECK(optional2.has_value());
        CHECK(*optional2 == 7);

        free_variable++;
        CHECK(*optional2 == 8);

        int other_variable = 4;
        retro::Optional<int &> optional3 = other_variable;
        CHECK(optional3.has_value());
        swap(optional2, optional3);
        CHECK(*optional3 == 8);
        CHECK(free_variable == 4);
        CHECK(other_variable == 8);

        optional3.reset();
        swap(optional3, optional2);
        CHECK(*optional3 == 4);
        CHECK_FALSE(optional2.has_value());
        CHECK(free_variable == 4);

        std::string testString = "Hello world";
        retro::Optional<std::string &> optional4 = testString;
        CHECK(optional4.has_value());
        CHECK(optional4->length() == 11);
        CHECK(std::as_const(optional4)->length() == 11);
    }
}