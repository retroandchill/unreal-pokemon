/**
 * @file TestBindMethod.cpp
 * @brief Test for the `bind_method` function.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#include <catch2/catch_test_macros.hpp>

#if RETROLIB_WITH_MODULES
import std;
import RetroLib;
#else
#include "RetroLib/Functional/BindMethod.h"
#endif

class TestClass {

  public:
    int method(int value1, int value2, int value3) const {
        return value1 + value2 + value3;
    }
};

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