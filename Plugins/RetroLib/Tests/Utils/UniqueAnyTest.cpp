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
        Retro::UniqueAny any1 = std::move(shared_pointer);

        Retro::UniqueAny any2 = std::move(any1);
        any2.Reset();

        CHECK(weak_pointer.expired());
    }

    SECTION("Can hold any type and change type as needed") {
        Retro::UniqueAny any;
        CHECK_FALSE(any.HasValue());

        std::any value3 = 5;
        any = 4;
        CHECK(any.HasValue());
        REQUIRE(any.GetType() == typeid(int));
        CHECK(any.Get<int>() == 4);
        CHECK(std::as_const(any).Get<int>() == 4);
        CHECK_THROWS_AS(any.Get<float>(), std::bad_any_cast);
        CHECK_THROWS_AS(std::as_const(any).Get<float>(), std::bad_any_cast);

        any.Emplace<std::string>("Hello world");

        auto string_value = any.TryGet<std::string>();
        CHECK(string_value.HasValue());
        CHECK(string_value.Value() == "Hello world");

        auto string_view_value = any.TryGet<std::string_view>();
        CHECK_FALSE(string_view_value.HasValue());

        auto const_string_value = std::as_const(any).TryGet<std::string>();
        CHECK(const_string_value.HasValue());
        CHECK(const_string_value.Value() == "Hello world");

        auto const_string_view_value = std::as_const(any).TryGet<std::string_view>();
        CHECK_FALSE(const_string_view_value.HasValue());

        any.Emplace<std::array<std::string, 10>>();
        CHECK(any.HasValue());
    }

    SECTION("Moving by assignment invalidates as well") {
        Retro::UniqueAny any1(std::in_place_type<std::array<int, 20>>);

        Retro::UniqueAny any2;
        any2 = std::move(any1);
        CHECK(any2.HasValue());
        CHECK_FALSE(any1.HasValue());
        CHECK(any1.GetType() == typeid(void));
    }
}