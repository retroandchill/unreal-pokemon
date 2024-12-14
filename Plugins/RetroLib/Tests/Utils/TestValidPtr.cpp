/**
 * @file TestValidPtr.cpp
 * @brief Test for validating pointer values.
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

TEST_CASE("Can check if a pointer is valid or invalid", "[utils]") {
    SECTION("Can work with raw pointers") {
        int value = 3;
        int *ptr = nullptr;
        int *valid_ptr = &value;

        CHECK_FALSE(retro::valid_ptr(ptr));
        CHECK(retro::valid_ptr(valid_ptr));

        CHECK(retro::invalid_ptr(ptr));
        CHECK_FALSE(retro::invalid_ptr(valid_ptr));
    }

    SECTION("Handles nullptr literals") {
        CHECK_FALSE(retro::valid_ptr(nullptr));
        CHECK(retro::invalid_ptr(nullptr));
    }

    SECTION("Can work with wrapped pointer types (smart pointers)") {
        std::unique_ptr<int> ptr = nullptr;
        auto valid_ptr = std::make_unique<int>(3);

        CHECK_FALSE(retro::valid_ptr(ptr));
        CHECK(retro::valid_ptr(valid_ptr));

        CHECK(retro::invalid_ptr(ptr));
        CHECK_FALSE(retro::invalid_ptr(valid_ptr));
    }
}