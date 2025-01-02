/**
 * @file UniqueAnyTest.cpp
 * @brief Verifies that UniqueAny works as expected.
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

TEST_CASE_NAMED(FUniqueAnyTest, "RetroLib::Utils::UniqueAny", "[utils]") {
    SECTION("Can move a value from one location to another") {
        auto SharedPointer = std::make_shared<int>(3);
        std::weak_ptr WeakPointer = SharedPointer;
        Retro::FUniqueAny Any1 = std::move(SharedPointer);

        Retro::FUniqueAny Any2 = std::move(Any1);
        Any2.Reset();

        CHECK(WeakPointer.expired());
    }

    SECTION("Can hold any type and change type as needed") {
        Retro::FUniqueAny Any;
        CHECK_FALSE(Any.HasValue());

        std::any Value3 = 5;
        Any = 4;
        CHECK(Any.HasValue());
        REQUIRE(Any.GetType() == typeid(int));
        CHECK(Any.Get<int>() == 4);
        CHECK(std::as_const(Any).Get<int>() == 4);
        CHECK_THROWS_AS(Any.Get<float>(), std::bad_any_cast);
        CHECK_THROWS_AS(std::as_const(Any).Get<float>(), std::bad_any_cast);

        Any.Emplace<std::string>("Hello world");

        auto StringValue = Any.TryGet<std::string>();
        CHECK(Retro::Optionals::HasValue(StringValue));
        CHECK(static_cast<std::string&>(Retro::Optionals::Get(StringValue)) == "Hello world");

        auto StringViewValue = Any.TryGet<std::string_view>();
        CHECK_FALSE(Retro::Optionals::HasValue(StringViewValue));

        auto ConstStringValue = std::as_const(Any).TryGet<std::string>();
        CHECK(Retro::Optionals::HasValue(ConstStringValue));
        CHECK(static_cast<const std::string&>(Retro::Optionals::Get(ConstStringValue)) == "Hello world");

        auto ConstStringViewValue = std::as_const(Any).TryGet<std::string_view>();
        CHECK_FALSE(Retro::Optionals::HasValue(ConstStringViewValue));

        Any.Emplace<std::array<std::string, 10>>();
        CHECK(Any.HasValue());
    }

    SECTION("Moving by assignment invalidates as well") {
        Retro::FUniqueAny Any1(std::in_place_type<std::array<int, 20>>);

        Retro::FUniqueAny Any2;
        Any2 = std::move(Any1);
        CHECK(Any2.HasValue());
        CHECK_FALSE(Any1.HasValue());
        CHECK(Any1.GetType() == typeid(void));
    }
}