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

static int Add(int a, int b) {
    return a + b;
}

static void AddToSharedBack(int a, std::shared_ptr<int> ptr) {
    *ptr += a;
}

struct FunctionalObject {

    std::vector<int> &operator()(std::vector<int> &Vector, int A, int B) const {
        Vector.emplace_back(A);
        Vector.emplace_back(B);
        return Vector;
    }
};

constexpr FunctionalObject Functor;

static int AddMany(int a, int b, int c, int d) {
    return a + b + c + d;
}

static int AddNumbers(int a, int b, int c) {
    return a + b + c;
}

class TestClass {

  public:
    int Method(int Value1, int Value2, int Value3) const {
        return Value1 + Value2 + Value3;
    }

    int Member = 9;
};

TEST_CASE("Can bind back to a runtime defined functional type", "[functional]") {
    SECTION("Binding back to a single argument works") {
        auto Binding = Retro::BindBack(Add, 4);
        CHECK(Binding(3) == 7);
        CHECK(std::as_const(Binding)(5) == 9);
        auto Number = std::make_shared<int>(5);
        auto WeakNumber = std::weak_ptr<int>(Number);
        Retro::BindBack(AddToSharedBack, std::move(Number))(4);
        CHECK(WeakNumber.expired());
    }

    SECTION("Binding two arguments works") {
        std::vector<int> Elements;
        auto Binding = Retro::BindBack(Functor, 3, 4);
        CHECK(std::addressof(Elements) == std::addressof(Binding(Elements)));
        CHECK(Elements.size() == 2);
        std::as_const(Binding)(Elements);
        CHECK(Elements.size() == 4);
        Retro::BindBack(Functor, 5, 6)(Elements);
        CHECK(Elements.size() == 6);
    }

    SECTION("Binding back to more than two arguments works") {
        auto Binding = Retro::BindBack(&AddMany, 4, 5, 6);
        CHECK(Binding(3) == 18);
        CHECK(std::as_const(Binding)(5) == 20);
        CHECK(Retro::BindBack(&AddMany, 10, 20, 30)(5) == 65);
    }
}

TEST_CASE("Can bind back to a constexpr defined functional type", "[functional]") {
    SECTION("Binding back to a single argument works") {
        auto Binding = Retro::BindBack<Add>(4);
        CHECK(Binding(3) == 7);
        CHECK(std::as_const(Binding)(5) == 9);
        auto Number = std::make_shared<int>(5);
        auto WeakNumber = std::weak_ptr<int>(Number);
        Retro::BindBack<AddToSharedBack>(std::move(Number))(4);
        CHECK(WeakNumber.expired());
    }

    SECTION("Binding two arguments works") {
        std::vector<int> Elements;
        auto Binding = Retro::BindBack<Functor>(3, 4);
        CHECK(std::addressof(Elements) == std::addressof(Binding(Elements)));
        CHECK(Elements.size() == 2);
        std::as_const(Binding)(Elements);
        CHECK(Elements.size() == 4);
        Retro::BindBack<Functor>(5, 6)(Elements);
        CHECK(Elements.size() == 6);
    }

    SECTION("Binding back to more than two arguments works") {
        auto Binding = Retro::BindBack<AddMany>(4, 5, 6);
        CHECK(Binding(3) == 18);
        CHECK(std::as_const(Binding)(5) == 20);
        CHECK(Retro::BindBack<AddMany>(10, 20, 30)(5) == 65);
    }
}

TEST_CASE("Can bind front to a constexpr defined functional type", "[functional]") {
    SECTION("Can bind with one parameter") {
        auto Binding = Retro::BindFront<&AddNumbers>(1);
        CHECK(Binding(2, 3) == 6);
        CHECK(std::as_const(Binding)(5, 4) == 10);
        CHECK(Retro::BindFront<&AddNumbers>(3)(5, 4) == 12);
    }

    SECTION("Can bind with two parameters") {
        auto Binding = Retro::BindFront<&AddNumbers>(1, 2);
        CHECK(Binding(3) == 6);
        CHECK(std::as_const(Binding)(4) == 7);
        CHECK(Retro::BindFront<&AddNumbers>(3, 6)(5) == 14);
    }

    SECTION("Can bind with three parameters") {
        auto Binding = Retro::BindFront<&AddNumbers>(1, 2, 3);
        CHECK(Binding() == 6);
        CHECK(std::as_const(Binding)() == 6);
        CHECK(Retro::BindFront<&AddNumbers>(3, 6, 9)() == 18);
    }
}

TEST_CASE("Can bind a method with an object at runtime", "[functional]") {
    SECTION("Can bind to an object of the given type") {
        TestClass Object;
        auto Binding = Retro::BindMethod(Object, &TestClass::Method);
        CHECK(Binding(1, 2, 1) == 4);
    }

    SECTION("Can bind to a raw pointer of an object") {
        TestClass Object;
        auto Ptr = &Object;
        auto Binding = Retro::BindMethod(Ptr, &TestClass::Method, 5);
        CHECK(Binding(4, 1) == 10);
        CHECK(std::as_const(Binding)(5, 5) == 15);
        CHECK(Retro::BindMethod(Ptr, &TestClass::Method, 10)(5, 5) == 20);
    }

    SECTION("Can bind to wrapped pointer object") {
        auto Object = std::make_shared<TestClass>();
        auto Binding = Retro::BindMethod(Object, &TestClass::Method, 5, 6);
        CHECK(Binding(4) == 15);
        CHECK(std::as_const(Binding)(5) == 16);
        CHECK(Retro::BindMethod(Object, &TestClass::Method, 10, 12)(5) == 27);
    }

    SECTION("Can bind to a reference wrapper object") {
        TestClass Object;
        auto Binding = Retro::BindMethod(std::ref(Object), &TestClass::Method, 5, 6, 4);
        CHECK(Binding() == 15);
        CHECK(std::as_const(Binding)() == 15);
        CHECK(Retro::BindMethod(std::cref(Object), &TestClass::Method, 10, 12, 7)() == 29);
    }
}

TEST_CASE("Can bind a method with an object at compile time", "[functional]") {
    SECTION("Can bind to an object of the given type") {
        TestClass Object;
        auto Binding = Retro::BindMethod<&TestClass::Method>(Object);
        CHECK(Binding(1, 2, 1) == 4);
    }

    SECTION("Can bind to a raw pointer of an object") {
        TestClass Object;
        auto Ptr = &Object;
        auto Binding = Retro::BindMethod<&TestClass::Method>(Ptr, 5);
        CHECK(Binding(4, 1) == 10);
        CHECK(std::as_const(Binding)(5, 5) == 15);
        CHECK(Retro::BindMethod<&TestClass::Method>(Ptr, 10)(5, 5) == 20);
    }

    SECTION("Can bind to wrapped pointer object") {
        auto Object = std::make_shared<TestClass>();
        auto Binding = Retro::BindMethod<&TestClass::Method>(Object, 5, 6);
        CHECK(Binding(4) == 15);
        CHECK(std::as_const(Binding)(5) == 16);
        CHECK(Retro::BindMethod<&TestClass::Method>(Object, 10, 12)(5) == 27);
    }

    SECTION("Can bind to a reference wrapper object") {
        TestClass Object;
        auto Binding = Retro::BindMethod<&TestClass::Method>(std::reference_wrapper(Object), 5, 6, 4);
        CHECK(Binding() == 15);
        CHECK(std::as_const(Binding)() == 15);
        CHECK(Retro::BindMethod<&TestClass::Method>(std::reference_wrapper(Object), 10, 12, 7)() == 29);
    }
}

TEST_CASE("Can use the opaque binding wrapper as runtime", "[functional]") {
    SECTION("Can bind a regular functor") {
        auto Binding = Retro::CreateBinding(Add, 4);
        CHECK(Binding(3) == 7);
        CHECK(std::as_const(Binding)(5) == 9);
        auto Number = std::make_shared<int>(5);
        auto WeakNumber = std::weak_ptr(Number);
        Retro::CreateBinding(AddToSharedBack, std::move(Number))(4);
        CHECK(WeakNumber.expired());
    }

    SECTION("Can bind a method using the object as the owner, or bind back without it") {
        auto Object = std::make_shared<TestClass>();
        auto Binding = Retro::CreateBinding(Object, &TestClass::Method, 5, 6);
        CHECK(Binding(4) == 15);
        CHECK(std::as_const(Binding)(5) == 16);
        CHECK(Retro::CreateBinding(&TestClass::Method, 10, 12)(Object, 5) == 27);
    }

    SECTION("Can bind to a member") {
        TestClass Object;
        auto Binding1 = Retro::CreateBinding(Object, &TestClass::Member);
        CHECK(Binding1() == 9);
        auto Binding2 = Retro::CreateBinding(&TestClass::Member);
        CHECK(Binding2(Object) == 9);
    }

    SECTION("Can bind a functor and use tuples with it") {
        static_assert(Retro::TupleLike<std::pair<int, int>>);
        auto Binding = Retro::CreateBinding(Add);
        CHECK(Binding(std::make_pair(3, 4)) == 7);
        CHECK(std::as_const(Binding)(std::make_pair(5, 4)) == 9);
        auto Number = std::make_shared<int>(5);
        auto WeakNumber = std::weak_ptr(Number);
        Retro::CreateBinding(AddToSharedBack)(std::make_pair(4, std::move(Number)));
        CHECK(WeakNumber.expired());
    }
}

TEST_CASE("Can use the opaque binding wrapper at compile time", "[functional]") {
    SECTION("Can bind a regular functor") {
        auto Binding = Retro::CreateBinding<Add>(4);
        CHECK(Binding(3) == 7);
        CHECK(std::as_const(Binding)(5) == 9);
        auto Number = std::make_shared<int>(5);
        auto WeakNumber = std::weak_ptr(Number);
        Retro::CreateBinding<AddToSharedBack>(std::move(Number))(4);
        CHECK(WeakNumber.expired());
    }

    SECTION("Can bind a method using the this tag, or bind back without it") {
        auto Object = std::make_shared<TestClass>();
        auto Binding = Retro::CreateBinding<&TestClass::Method>(Retro::This, Object, 5, 6);
        CHECK(Binding(4) == 15);
        CHECK(std::as_const(Binding)(5) == 16);
        CHECK(Retro::CreateBinding<&TestClass::Method>(10, 12)(Object, 5) == 27);
    }

    SECTION("Can bind to a member") {
        TestClass Object;
        auto Binding1 = Retro::CreateBinding<&TestClass::Member>(Object);
        CHECK(Binding1() == 9);
        auto Binding2 = Retro::CreateBinding<&TestClass::Member>();
        CHECK(Binding2(Object) == 9);
    }

    SECTION("Can bind a functor and use tuples with it") {
        auto Binding = Retro::CreateBinding<Add>();
        CHECK(Binding(std::make_pair(3, 4)) == 7);
        CHECK(std::as_const(Binding)(std::make_pair(5, 4)) == 9);
        auto Number = std::make_shared<int>(5);
        auto WeakNumber = std::weak_ptr(Number);
        Retro::CreateBinding<AddToSharedBack>()(std::make_pair(4, std::move(Number)));
        CHECK(WeakNumber.expired());
    }
}