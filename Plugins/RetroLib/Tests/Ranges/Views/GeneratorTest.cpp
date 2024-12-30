/**
 * @file GeneratorTest.cpp
 * @brief Test for the coroutine generator type
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#if RETROLIB_WITH_COROUTINES
#include <catch2/catch_test_macros.hpp>

#if RETROLIB_WITH_MODULES
import std;
import RetroLib;
#else
#include "RetroLib/Ranges/Algorithm/To.h"
#include "RetroLib/Ranges/Views/Filter.h"
#include "RetroLib/Ranges/Views/Generator.h"

#include <array>
#include <vector>
#endif

namespace Retro::Ranges::Testing {
    static Generator<int> GenerateIntegers(int Num) {
        for (int i = 0; i < Num; i++) {
            co_yield i;
        }
    }

    template <typename T>
    struct Tree {
        T Value;
        Tree *Left{};
        Tree* Right{};

        Generator<const T &> TraverseInorder() const {
            if (Left) {
                co_yield Ranges::ElementsOf(Left->TraverseInorder());
            }

            co_yield Value;

            if (Right) {
                co_yield Ranges::ElementsOf(Right->TraverseInorder());
            }
        }
    };
} // namespace retro::ranges::testing

TEST_CASE("Can create a lazily evaluated generator", "[ranges]") {
    using namespace Retro::Ranges::Testing;
    SECTION("Use a generator to loop through some numbers") {
        std::vector<int> Numbers;
        for (int i : GenerateIntegers(5)) {
            Numbers.push_back(i);
        }
        CHECK(Numbers == std::vector({0, 1, 2, 3, 4}));
    }

    SECTION("Can use a generator with a range pipe") {
        auto Numbers = GenerateIntegers(10) | Retro::Ranges::Views::Filter([](int Value) { return Value % 2 == 0; }) |
                       Retro::Ranges::To<std::vector>();
        CHECK(Numbers == std::vector({0, 2, 4, 6, 8}));
    }

    SECTION("Can traverse a nested range using ElementsOf") {
        std::array<Tree<char>, 7> Tree;
        Tree[0] = {'D', &Tree[1], &Tree[2]};
        Tree[1] = {'B', &Tree[3], &Tree[4]};
        Tree[2] = {'F', &Tree[5], &Tree[6]};
        Tree[3] = {'A'};
        Tree[4] = {'C'};
        Tree[5] = {'E'};
        Tree[6] = {'G'};

        std::vector<char> Values;
        for (auto Value : Tree[0].TraverseInorder()) {
            Values.push_back(Value);
        }
        CHECK(Values == std::vector({'A', 'B', 'C', 'D', 'E', 'F', 'G'}));
    }
}
#endif