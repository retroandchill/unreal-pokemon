/**
 * @file TestBindings.cpp
 * @brief Test the `bind_back` method to ensure that it works.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#include <catch2/catch_test_macros.hpp>

#if RETROLIB_WITH_MODULES
import std;
import RetroLib;
#else
#include "RetroLib/Functional/BindBack.h"
#include "RetroLib/Functional/BindFront.h"
#include "RetroLib/Functional/BindMethod.h"
#include "RetroLib/Functional/CreateBinding.h"

#include <vector>
#endif

static int add(int a, int b) {
    return a + b;
}

static void add_to_shared_back(int a, std::shared_ptr<int> ptr) {
    *ptr += a;
}

struct FunctionalObject {

    std::vector<int> &operator()(std::vector<int> &vector, int a, int b) const {
        vector.emplace_back(a);
        vector.emplace_back(b);
        return vector;
    }
};

constexpr FunctionalObject functor;

static int add_many(int a, int b, int c, int d) {
    return a + b + c + d;
}

static int add_numbers(int a, int b, int c) {
    return a + b + c;
}

class TestClass {

  public:
    int method(int value1, int value2, int value3) const {
        return value1 + value2 + value3;
    }

    int member = 9;
};

TEST_CASE("Can bind back to a constexpr defined functional type", "[functional]") {
    SECTION("Binding back to a single argument works") {
        auto binding = retro::bind_back<add>(4);
        CHECK(binding(3) == 7);
        CHECK(std::as_const(binding)(5) == 9);
        auto number = std::make_shared<int>(5);
        auto weakNumber = std::weak_ptr<int>(number);
        retro::bind_back<add_to_shared_back>(std::move(number))(4);
        CHECK(weakNumber.expired());
    }

    SECTION("Binding two arguments works") {
        std::vector<int> elements;
        auto binding = retro::bind_back<functor>(3, 4);
        CHECK(std::addressof(elements) == std::addressof(binding(elements)));
        CHECK(elements.size() == 2);
        std::as_const(binding)(elements);
        CHECK(elements.size() == 4);
        retro::bind_back<functor>(5, 6)(elements);
        CHECK(elements.size() == 6);
    }

    SECTION("Binding back to more than two arguments works") {
        auto binding = retro::bind_back<add_many>(4, 5, 6);
        CHECK(binding(3) == 18);
        CHECK(std::as_const(binding)(5) == 20);
        CHECK(retro::bind_back<add_many>(10, 20, 30)(5) == 65);
    }
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

TEST_CASE("Can bind a method with an object", "[functional]") {
    SECTION("Can bind to an object of the given type") {
        TestClass object;
        auto binding = retro::bind_method<&TestClass::method>(object);
        CHECK(binding(1, 2, 1) == 4);
    }

    SECTION("Can bind to a raw pointer of an object") {
        TestClass object;
        auto ptr = &object;
        auto binding = retro::bind_method<&TestClass::method>(ptr, 5);
        CHECK(binding(4, 1) == 10);
        CHECK(std::as_const(binding)(5, 5) == 15);
        CHECK(retro::bind_method<&TestClass::method>(ptr, 10)(5, 5) == 20);
    }

    SECTION("Can bind to wrapped pointer object") {
        auto object = std::make_shared<TestClass>();
        auto binding = retro::bind_method<&TestClass::method>(object, 5, 6);
        CHECK(binding(4) == 15);
        CHECK(std::as_const(binding)(5) == 16);
        CHECK(retro::bind_method<&TestClass::method>(object, 10, 12)(5) == 27);
    }

    SECTION("Can bind to a reference wrapper object") {
        TestClass object;
        auto binding = retro::bind_method<&TestClass::method>(std::reference_wrapper(object), 5, 6, 4);
        CHECK(binding() == 15);
        CHECK(std::as_const(binding)() == 15);
        CHECK(retro::bind_method<&TestClass::method>(std::reference_wrapper(object), 10, 12, 7)() == 29);
    }
}

TEST_CASE("Can use the opaque binding wrapper", "[functional]") {
    SECTION("Can bind a regular functor") {
        auto binding = retro::create_binding<add>(4);
        CHECK(binding(3) == 7);
        CHECK(std::as_const(binding)(5) == 9);
        auto number = std::make_shared<int>(5);
        auto weakNumber = std::weak_ptr(number);
        retro::create_binding<add_to_shared_back>(std::move(number))(4);
        CHECK(weakNumber.expired());
    }

    SECTION("Can bind a method using the this tag, or bind back without it") {
        auto object = std::make_shared<TestClass>();
        auto binding = retro::create_binding<&TestClass::method>(retro::this_type, object, 5, 6);
        CHECK(binding(4) == 15);
        CHECK(std::as_const(binding)(5) == 16);
        CHECK(retro::create_binding<&TestClass::method>(10, 12)(object, 5) == 27);
    }

    SECTION("Can bind to a member") {
        TestClass object;
        auto binding1 = retro::create_binding<&TestClass::member>(object);
        CHECK(binding1() == 9);
        auto binding2 = retro::create_binding<&TestClass::member>();
        CHECK(binding2(object) == 9);
    }

    SECTION("Can bind a functor and use tuples with it") {
        auto binding = retro::create_binding<add>();
        CHECK(binding(std::make_pair(3, 4)) == 7);
        CHECK(std::as_const(binding)(std::make_pair(5, 4)) == 9);
        auto number = std::make_shared<int>(5);
        auto weakNumber = std::weak_ptr(number);
        retro::create_binding<add_to_shared_back>()(std::make_pair(4, std::move(number)));
        CHECK(weakNumber.expired());
    }
}