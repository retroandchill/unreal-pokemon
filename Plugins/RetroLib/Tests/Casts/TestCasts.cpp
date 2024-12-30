/**
 * @file TestInstanceOf.cpp
 * @brief Get for the instance_of functor.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#include <catch2/catch_test_macros.hpp>

#if RETROLIB_WITH_MODULES
import std;
import RetroLib;
#else
#include "RetroLib/Casting/ByteCast.h"
#include "RetroLib/Casting/DynamicCast.h"
#include "RetroLib/Casting/StaticCast.h"
#include "RetroLib/Casting/InstanceOf.h"

#include <memory>
#endif

namespace InstanceofTest {
    struct Base {
        virtual ~Base() = default;
        virtual int Foo() = 0;
    };

    struct Derived1 : Base {
        int Foo() override {
            return Value1;
        }

        int Value1 = 3;
    };
    struct Derived2 : Base {
        int Foo() override {
            return Value2;
        }

        int Value2 = 4;
    };
} // namespace instanceof_test

using namespace InstanceofTest;

TEST_CASE("Check to see if we can convert between types statically", "[utils]") {
    SECTION("Can convert between numberic types") {
        CHECK(Retro::StaticCast<double>(4) == 4.0);
        CHECK(Retro::StaticCast<int>(4.0) == 4);
    }

    SECTION("Can perform implicit conversions that call constructors") {
        CHECK(Retro::StaticCast<std::string>("Hello world") == "Hello world");
        CHECK(Retro::StaticCast<Retro::Optional<int>>(4) == 4);
    }

    SECTION("Can convert to pointers") {
        Derived1 Value1;
        auto BaseValue = Retro::StaticCast<Base *>(&Value1);
        CHECK(BaseValue == &Value1);
        CHECK(Retro::StaticCast<Derived1 *>(BaseValue) == &Value1);
        auto Opaque = Retro::StaticCast<void *>(BaseValue);
        CHECK(Opaque == &Value1);
        CHECK(Retro::StaticCast<Derived1 *>(Opaque) == &Value1);
        CHECK(std::addressof(Retro::ByteCast<Derived1>(Opaque)) == &Value1);
    }
}

TEST_CASE("Can check if a polymorphic type is an instance of another type", "[utils]") {
    SECTION("Can work with raw pointers") {
        Derived1 Value1;
        Derived2 Value2;
        Base *Ptr = nullptr;
        Base *ValidPtr1 = &Value1;
        Base *ValidPtr2 = &Value2;

        CHECK_FALSE(Retro::InstanceOf<Base>(Ptr));
        CHECK_FALSE(Retro::InstanceOf<Derived1>(Ptr));
        CHECK_FALSE(Retro::InstanceOf<Derived2>(Ptr));

        CHECK(Retro::InstanceOf<Base>(ValidPtr1));
        CHECK(Retro::InstanceOf<Derived1>(ValidPtr1));
        CHECK_FALSE(Retro::InstanceOf<Derived2>(ValidPtr1));

        CHECK(Retro::InstanceOf<Base>(ValidPtr2));
        CHECK_FALSE(Retro::InstanceOf<Derived1>(ValidPtr2));
        CHECK(Retro::InstanceOf<Derived2>(ValidPtr2));
    }

    SECTION("Handles nullptr literals") {
        CHECK_FALSE(Retro::InstanceOf<Derived1>(nullptr));
        CHECK_FALSE(Retro::InstanceOf<Derived2>(nullptr));
    }

    SECTION("Can work with wrapped pointer types (smart pointers)") {
        std::unique_ptr<Base> Ptr = nullptr;
        std::unique_ptr<Base> ValidPtr1 = std::make_unique<Derived1>();
        std::unique_ptr<Base> ValidPtr2 = std::make_unique<Derived2>();

        CHECK_FALSE(Retro::InstanceOf<Base>(Ptr));
        CHECK_FALSE(Retro::InstanceOf<Derived1>(Ptr));
        CHECK_FALSE(Retro::InstanceOf<Derived2>(Ptr));

        CHECK(Retro::InstanceOf<Base>(ValidPtr1));
        CHECK(Retro::InstanceOf<Derived1>(ValidPtr1));
        CHECK_FALSE(Retro::InstanceOf<Derived2>(ValidPtr1));

        CHECK(Retro::InstanceOf<Base>(ValidPtr2));
        CHECK_FALSE(Retro::InstanceOf<Derived1>(ValidPtr2));
        CHECK(Retro::InstanceOf<Derived2>(ValidPtr2));
    }

    SECTION("Can work with a polymorphic value instance") {
        Retro::Polymorphic<Base> ValidPtr1(std::in_place_type<Derived1>);
        Retro::Polymorphic<Base> ValidPtr2(std::in_place_type<Derived2>);

        CHECK(Retro::InstanceOf<Base>(ValidPtr1));
        CHECK(Retro::InstanceOf<Derived1>(ValidPtr1));
        CHECK_FALSE(Retro::InstanceOf<Derived2>(ValidPtr1));

        CHECK(Retro::InstanceOf<Base>(ValidPtr2));
        CHECK_FALSE(Retro::InstanceOf<Derived1>(ValidPtr2));
        CHECK(Retro::InstanceOf<Derived2>(ValidPtr2));
    }
}

TEST_CASE("Can check if a we can cast between types", "[utils]") {
    SECTION("Can work with raw pointers") {
        Derived1 Value1;
        Derived2 Value2;
        Base *Ptr = nullptr;
        Base *ValidPtr1 = &Value1;
        Base *ValidPtr2 = &Value2;

        CHECK_FALSE(Retro::DynamicCast<Base>(Ptr).HasValue());
        CHECK_FALSE(Retro::DynamicCast<Derived1>(Ptr).HasValue());
        CHECK_FALSE(Retro::DynamicCast<Derived2>(Ptr).HasValue());

        CHECK(Retro::DynamicCast<Base>(ValidPtr1).HasValue());
        CHECK(Retro::DynamicCast<Derived1>(ValidPtr1).HasValue());
        CHECK_FALSE(Retro::DynamicCast<Derived2>(ValidPtr1).HasValue());

        CHECK(Retro::DynamicCast<Base>(ValidPtr2).HasValue());
        CHECK_FALSE(Retro::DynamicCast<Derived1>(ValidPtr2).HasValue());
        CHECK(Retro::DynamicCast<Derived2>(ValidPtr2).HasValue());
    }

    SECTION("Handles nullptr literals") {
        CHECK_FALSE(Retro::DynamicCast<Derived1>(nullptr).HasValue());
        CHECK_FALSE(Retro::DynamicCast<Derived2>(nullptr).HasValue());
    }

    SECTION("Can work with wrapped pointer types (smart pointers)") {
        std::unique_ptr<Base> Ptr = nullptr;
        std::unique_ptr<Base> ValidPtr1 = std::make_unique<Derived1>();
        std::unique_ptr<Base> ValidPtr2 = std::make_unique<Derived2>();

        CHECK_FALSE(Retro::DynamicCast<Base>(Ptr).HasValue());
        CHECK_FALSE(Retro::DynamicCast<Derived1>(Ptr).HasValue());
        CHECK_FALSE(Retro::DynamicCast<Derived2>(Ptr).HasValue());

        CHECK(Retro::DynamicCast<Base>(ValidPtr1).HasValue());
        CHECK(Retro::DynamicCast<Derived1>(ValidPtr1).HasValue());
        CHECK_FALSE(Retro::DynamicCast<Derived2>(ValidPtr1).HasValue());

        CHECK(Retro::DynamicCast<Base>(ValidPtr2).HasValue());
        CHECK_FALSE(Retro::DynamicCast<Derived1>(ValidPtr2).HasValue());
        CHECK(Retro::DynamicCast<Derived2>(ValidPtr2).HasValue());
    }

    SECTION("Can work with a polymorphic value instance") {
        Retro::Polymorphic<Base> ValidPtr1(std::in_place_type<Derived1>);
        Retro::Polymorphic<Base> ValidPtr2(std::in_place_type<Derived2>);

        CHECK(Retro::InstanceOf<Base>(ValidPtr1));
        CHECK(Retro::InstanceOf<Derived1>(ValidPtr1));
        CHECK_FALSE(Retro::InstanceOf<Derived2>(ValidPtr1));

        CHECK(Retro::InstanceOf<Base>(ValidPtr2));
        CHECK_FALSE(Retro::InstanceOf<Derived1>(ValidPtr2));
        CHECK(Retro::InstanceOf<Derived2>(ValidPtr2));
    }
}