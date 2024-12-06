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
#include "RetroLib/Utils/ValidPtr.h"
#include <memory>
#endif

namespace instanceof_test {
    struct Base {
        virtual ~Base() = default;
        virtual int foo() = 0;
    };

    struct Derived1 : Base {
        int foo() override {
            return value1;
        }

        int value1 = 3;
    };
    struct Derived2 : Base {
        int foo() override {
            return value2;
        }

        int value2 = 4;
    };
}

using namespace instanceof_test;

TEST_CASE("Check to see if we can convert between types statically", "[utils]") {
    SECTION("Can convert between numberic types") {
        CHECK(retro::convert<double>(4) == 4.0);
        CHECK(retro::convert<int>(4.0) == 4);
    }

    SECTION("Can perform implicit conversions that call constructors") {
        CHECK(retro::convert<std::string>("Hello world") == "Hello world");
        CHECK(retro::convert<retro::Optional<int>>(4) == 4);
    }

    SECTION("Can convert to pointers") {
        Derived1 derived1;
        auto base = retro::convert<Base*>(&derived1);
        CHECK(base == &derived1);
        CHECK(retro::convert<Derived1*>(base) == &derived1);
        auto opaque = retro::convert<void*>(base);
        CHECK(opaque == &derived1);
        CHECK(retro::convert<Derived1*>(opaque) == &derived1);
        CHECK(std::addressof(retro::byte_cast<Derived1>(opaque)) == &derived1);
    }
}

TEST_CASE("Can check if a polymorphic type is an instance of another type", "[utils]") {
    SECTION("Can work with raw pointers") {
        Derived1 value1;
        Derived2 value2;
        Base* ptr = nullptr;
        Base* valid_ptr1 = &value1;
        Base* valid_ptr2 = &value2;

        CHECK_FALSE(retro::instance_of<Base>(ptr));
        CHECK_FALSE(retro::instance_of<Derived1>(ptr));
        CHECK_FALSE(retro::instance_of<Derived2>(ptr));

        CHECK(retro::instance_of<Base>(valid_ptr1));
        CHECK(retro::instance_of<Derived1>(valid_ptr1));
        CHECK_FALSE(retro::instance_of<Derived2>(valid_ptr1));

        CHECK(retro::instance_of<Base>(valid_ptr2));
        CHECK_FALSE(retro::instance_of<Derived1>(valid_ptr2));
        CHECK(retro::instance_of<Derived2>(valid_ptr2));
    }

    SECTION("Handles nullptr literals") {
        CHECK_FALSE(retro::instance_of<Derived1>(nullptr));
        CHECK_FALSE(retro::instance_of<Derived2>(nullptr));
    }

    SECTION("Can work with wrapped pointer types (smart pointers)") {
        std::unique_ptr<Base> ptr = nullptr;
        std::unique_ptr<Base> valid_ptr1 = std::make_unique<Derived1>();
        std::unique_ptr<Base> valid_ptr2 = std::make_unique<Derived2>();

        CHECK_FALSE(retro::instance_of<Base>(ptr));
        CHECK_FALSE(retro::instance_of<Derived1>(ptr));
        CHECK_FALSE(retro::instance_of<Derived2>(ptr));

        CHECK(retro::instance_of<Base>(valid_ptr1));
        CHECK(retro::instance_of<Derived1>(valid_ptr1));
        CHECK_FALSE(retro::instance_of<Derived2>(valid_ptr1));

        CHECK(retro::instance_of<Base>(valid_ptr2));
        CHECK_FALSE(retro::instance_of<Derived1>(valid_ptr2));
        CHECK(retro::instance_of<Derived2>(valid_ptr2));
    }

    SECTION("Can work with a polymorphic value instance") {
        retro::Polymorphic<Base> valid_ptr1(std::in_place_type<Derived1>);
        retro::Polymorphic<Base> valid_ptr2(std::in_place_type<Derived2>);

        CHECK(retro::instance_of<Base>(valid_ptr1));
        CHECK(retro::instance_of<Derived1>(valid_ptr1));
        CHECK_FALSE(retro::instance_of<Derived2>(valid_ptr1));

        CHECK(retro::instance_of<Base>(valid_ptr2));
        CHECK_FALSE(retro::instance_of<Derived1>(valid_ptr2));
        CHECK(retro::instance_of<Derived2>(valid_ptr2));
    }
}

TEST_CASE("Can check if a we can cast between types", "[utils]") {
    SECTION("Can work with raw pointers") {
        Derived1 value1;
        Derived2 value2;
        Base* ptr = nullptr;
        Base* valid_ptr1 = &value1;
        Base* valid_ptr2 = &value2;

        CHECK_FALSE(retro::class_cast<Base>(ptr).has_value());
        CHECK_FALSE(retro::class_cast<Derived1>(ptr).has_value());
        CHECK_FALSE(retro::class_cast<Derived2>(ptr).has_value());

        CHECK(retro::class_cast<Base>(valid_ptr1).has_value());
        CHECK(retro::class_cast<Derived1>(valid_ptr1).has_value());
        CHECK_FALSE(retro::class_cast<Derived2>(valid_ptr1).has_value());

        CHECK(retro::class_cast<Base>(valid_ptr2).has_value());
        CHECK_FALSE(retro::class_cast<Derived1>(valid_ptr2).has_value());
        CHECK(retro::class_cast<Derived2>(valid_ptr2).has_value());
    }

    SECTION("Handles nullptr literals") {
        CHECK_FALSE(retro::class_cast<Derived1>(nullptr).has_value());
        CHECK_FALSE(retro::class_cast<Derived2>(nullptr).has_value());
    }

    SECTION("Can work with wrapped pointer types (smart pointers)") {
        std::unique_ptr<Base> ptr = nullptr;
        std::unique_ptr<Base> valid_ptr1 = std::make_unique<Derived1>();
        std::unique_ptr<Base> valid_ptr2 = std::make_unique<Derived2>();

        CHECK_FALSE(retro::class_cast<Base>(ptr).has_value());
        CHECK_FALSE(retro::class_cast<Derived1>(ptr).has_value());
        CHECK_FALSE(retro::class_cast<Derived2>(ptr).has_value());

        CHECK(retro::class_cast<Base>(valid_ptr1).has_value());
        CHECK(retro::class_cast<Derived1>(valid_ptr1).has_value());
        CHECK_FALSE(retro::class_cast<Derived2>(valid_ptr1).has_value());

        CHECK(retro::class_cast<Base>(valid_ptr2).has_value());
        CHECK_FALSE(retro::class_cast<Derived1>(valid_ptr2).has_value());
        CHECK(retro::class_cast<Derived2>(valid_ptr2).has_value());
    }

    SECTION("Can work with a polymorphic value instance") {
        retro::Polymorphic<Base> valid_ptr1(std::in_place_type<Derived1>);
        retro::Polymorphic<Base> valid_ptr2(std::in_place_type<Derived2>);

        CHECK(retro::instance_of<Base>(valid_ptr1));
        CHECK(retro::instance_of<Derived1>(valid_ptr1));
        CHECK_FALSE(retro::instance_of<Derived2>(valid_ptr1));

        CHECK(retro::instance_of<Base>(valid_ptr2));
        CHECK_FALSE(retro::instance_of<Derived1>(valid_ptr2));
        CHECK(retro::instance_of<Derived2>(valid_ptr2));
    }
}