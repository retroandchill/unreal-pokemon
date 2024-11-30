/**
 * @file TestBindBack.cpp
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