/**
 * @file TestInvoke.cpp
 * @brief Tests the constexpr invoke functions.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#include <catch2/catch_test_macros.hpp>

#if RETROLIB_WITH_MODULES
import std;
import RetroLib;
#else
#include "RetroLib/Functional/Invoke.h"
#endif

static int add_numbers(int a, int b, int c) {
    return a + b + c;
}

struct InvokeTestStruct {
    int data_member;
    std::string other_member = "Hello World";

    int get_data_member() const {
        return data_member;
    }

    std::string &get_other_member() & {
        return other_member;
    }

    const std::string &get_other_member_const() const & {
        return other_member;
    }

    std::string &&get_other_member_rvalue() && {
        return std::move(other_member);
    }


};

TEST_CASE("Test the constexpr invoke works in all cases") {
    SECTION("Verify that the static invoke works in on free functions/lambdas") {
        CHECK(retro::invoke<add_numbers>(1, 2, 3) == 6);

        constexpr auto add_numbers_lambda = [](int a, int b, int c) { return a + b + c; };
        CHECK(retro::invoke<add_numbers_lambda>(1, 2, 3) == 6);
    }

    SECTION("Verify it can be used on class members") {
        InvokeTestStruct object = { 5 };
        decltype(auto) other_member = retro::invoke<&InvokeTestStruct::get_other_member>(object);
        static_assert(std::is_lvalue_reference_v<decltype(other_member)>);
        CHECK(other_member == "Hello World");
        decltype(auto) const_other_member = retro::invoke<&InvokeTestStruct::get_other_member_const>(object);
        static_assert(std::is_lvalue_reference_v<decltype(const_other_member)>);
        CHECK(const_other_member == "Hello World");

        auto rvalue_other_member = retro::invoke<&InvokeTestStruct::get_other_member_rvalue>(InvokeTestStruct());
        CHECK(rvalue_other_member == "Hello World");

        decltype(auto) other_member_direct = retro::invoke<&InvokeTestStruct::other_member>(object);
        static_assert(std::is_lvalue_reference_v<decltype(other_member)>);
        CHECK(other_member_direct == "Hello World");
        decltype(auto) const_other_member_direct = retro::invoke<&InvokeTestStruct::other_member>(std::as_const(object));
        static_assert(std::is_lvalue_reference_v<decltype(const_other_member)>);
        CHECK(const_other_member_direct == "Hello World");

        auto rvalue_other_member_direct = retro::invoke<&InvokeTestStruct::other_member>(InvokeTestStruct());
        CHECK(rvalue_other_member_direct == "Hello World");
    }

    SECTION("Verify it can be used on class members with reference qualifiers") {
        InvokeTestStruct object = { 5 };
        CHECK(retro::invoke<&InvokeTestStruct::get_data_member>(object) == 5);
        CHECK(retro::invoke<&InvokeTestStruct::data_member>(object) == 5);
    }

    SECTION("Verify it can be used on class members with a reference wrapper") {
        InvokeTestStruct object = { 5 };
        std::reference_wrapper wrapped = object;
        CHECK(retro::invoke<&InvokeTestStruct::get_data_member>(wrapped) == 5);
        CHECK(retro::invoke<&InvokeTestStruct::data_member>(wrapped) == 5);
    }

    SECTION("Verify it can be used on class members with a raw pointer") {
        InvokeTestStruct object = { 5 };
        CHECK(retro::invoke<&InvokeTestStruct::get_data_member>(&object) == 5);
        CHECK(retro::invoke<&InvokeTestStruct::data_member>(&object) == 5);
    }

    SECTION("Verify it can be used on class members with a smart pointer") {
        auto object = std::make_unique<InvokeTestStruct>(5);
        CHECK(retro::invoke<&InvokeTestStruct::get_data_member>(object) == 5);
        CHECK(retro::invoke<&InvokeTestStruct::data_member>(object) == 5);
    }
}