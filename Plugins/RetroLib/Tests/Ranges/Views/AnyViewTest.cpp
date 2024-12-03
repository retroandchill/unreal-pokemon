/**
 * @file AnyViewTest.cpp
 * @brief Test file for the AnyView class.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#include <catch2/catch_test_macros.hpp>

#if RETROLIB_WITH_MODULES
import std;
import RetroLib;
#else
#include "RetroLib/Ranges/Views/AnyView.h"

#include <vector>
#endif

TEST_CASE("Any view can take in a view of any type and iterate over it", "[views]") {
    SECTION("Iterating over a default initialized view is empty") {
        retro::ranges::AnyView<int> view;
        int count = 0;
        for (auto _ : view) {
            count++;
        }
        CHECK(count == 0);
    }

    SECTION("We can iterate over multiple different containers that has range semantics") {
        retro::ranges::AnyView<int> view(std::vector<int>({1, 2, 3, 4}));
        int count = 0;
        for (auto value : view) {
            count += value;
        }
        CHECK(count == 10);

        std::array values = {1, 2, 3, 4, 5};
        view = values;
        count = 0;
        for (auto value : view) {
            count += value;
        }
        CHECK(count == 15);
    }

    SECTION("Can iterate over a range pipe") {
        std::array values = {1, 2, 3, 4, 5};
        retro::ranges::AnyView<int> view = values | std::ranges::views::transform([](int value) { return value * 2; });
        int count = 0;
        for (auto value : view) {
            count += value;
        }
        CHECK(count == 30);
    }

    SECTION("Can compare iterators between view instances") {
        retro::ranges::AnyView<int> view(std::vector<int>({1, 2, 3, 4}));
        auto iterator1 = view.begin();
        auto iterator2 = view.begin();
        iterator1++;
        iterator2++;
        CHECK(iterator1 == iterator2);
    }

    SECTION("Can use any view with a range pipe") {
        auto view = retro::ranges::AnyView<int>(std::vector<int>({1, 2, 3, 4})) |
                    std::ranges::views::filter([](int value) { return value % 2 == 0; });
        int count = 0;
        for (auto value : view) {
            count += value;
        }
        CHECK(count == 6);
    }
}