/**
 * @file UniqueAnyTest.cpp
 * @brief Verifies that UniqueAny works as expected.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#include <catch2/catch_test_macros.hpp>

#if RETROLIB_WITH_MODULES
import std;
import RetroLib;
#else
#include "RetroLib/Utils/UniqueAny.h"

#include <array>
#include <memory>
#endif

TEST_CASE("Can create a move-only unique any type", "[utils]") {
    SECTION("Can move a value from one location to another") {
        auto shared_pointer = std::make_shared<int>(3);
        std::weak_ptr weak_pointer = shared_pointer;
        retro::UniqueAny any1 = std::move(shared_pointer);

        retro::UniqueAny any2 = std::move(any1);
        any2.reset();

        CHECK(weak_pointer.expired());
    }

    SECTION("Can hold any type and change type as needed") {
        retro::UniqueAny any;
        CHECK_FALSE(any.has_value());

        std::any value3 = 5;
        any = 4;
        CHECK(any.has_value());
        REQUIRE(any.get_type() == typeid(int));
        CHECK(any.get<int>() == 4);
        CHECK(std::as_const(any).get<int>() == 4);
        CHECK_THROWS_AS(any.get<float>(), std::bad_any_cast);
        CHECK_THROWS_AS(std::as_const(any).get<float>(), std::bad_any_cast);

        any.emplace<std::string>("Hello world");

        auto string_value = any.try_get<std::string>();
        CHECK(string_value.has_value());
        CHECK(string_value.value() == "Hello world");

        auto string_view_value = any.try_get<std::string_view>();
        CHECK_FALSE(string_view_value.has_value());

        auto const_string_value = std::as_const(any).try_get<std::string>();
        CHECK(const_string_value.has_value());
        CHECK(const_string_value.value() == "Hello world");

        auto const_string_view_value = std::as_const(any).try_get<std::string_view>();
        CHECK_FALSE(const_string_view_value.has_value());

        any.emplace<std::array<std::string, 10>>();
        CHECK(any.has_value());
    }

    SECTION("Moving by assignment invalidates as well") {
        retro::UniqueAny any1(std::in_place_type<std::array<int, 20>>);

        retro::UniqueAny any2;
        any2 = std::move(any1);
        CHECK(any2.has_value());
        CHECK_FALSE(any1.has_value());
        CHECK(any1.get_type() == typeid(void));
    }
}