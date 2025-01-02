/**
 * @file AnyViewTest.cpp
 * @brief Test file for the AnyView class.
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

#include <vector>
#endif

TEST_CASE_NAMED(FAnyViewTest, "RetroLib::Ranges::Views::AnyView", "[views]") {
    SECTION("Iterating over a default initialized view is empty") {
        Retro::Ranges::TAnyView<int> View;
        int Count = 0;
        for (auto _ : View) {
            Count++;
        }
        CHECK(Count == 0);
    }

    SECTION("We can iterate over multiple different containers that has range semantics") {
        Retro::Ranges::TAnyView<int> View(std::vector<int>({1, 2, 3, 4}));
        int Count = 0;
        for (auto Value : View) {
            Count += Value;
        }
        CHECK(Count == 10);

        std::array Values = {1, 2, 3, 4, 5};
        View = Values;
        Count = 0;
        for (auto Value : View) {
            Count += Value;
        }
        CHECK(Count == 15);
    }

    SECTION("Can iterate over a range pipe") {
        std::array Values = {1, 2, 3, 4, 5};
        Retro::Ranges::TAnyView<int> view = Values | std::ranges::views::transform([](int Value) { return Value * 2; });
        int Count = 0;
        for (auto Value : view) {
            Count += Value;
        }
        CHECK(Count == 30);
    }

    SECTION("Can compare iterators between view instances") {
        Retro::Ranges::TAnyView<int> view(std::vector<int>({1, 2, 3, 4}));
        auto Iterator1 = view.begin();
        auto Iterator2 = view.begin();
        Iterator1++;
        Iterator2++;
        CHECK(Iterator1 == Iterator2);
    }

    SECTION("Can use any view with a range pipe") {
        auto View = Retro::Ranges::TAnyView<int>(std::vector<int>({1, 2, 3, 4})) |
                    std::ranges::views::filter([](const int value) { return value % 2 == 0; });
        int Count = 0;
        for (auto Value : View) {
            Count += Value;
        }
        CHECK(Count == 6);
    }
}