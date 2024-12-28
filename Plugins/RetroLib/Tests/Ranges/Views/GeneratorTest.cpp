/**
 * @file GeneratorTest.cpp
 * @brief Test for the coroutine generator type
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
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

namespace retro::ranges::testing {
    static Generator<int> generate_integers(int num) {
        for (int i = 0; i < num; i++) {
            co_yield i;
        }
    }

    template <typename T>
    struct Tree {
        T value;
        Tree *left{}, *right{};

        Generator<const T &> traverse_inorder() const {
            if (left) {
                co_yield ranges::ElementsOf(left->traverse_inorder());
            }

            co_yield value;

            if (right) {
                co_yield ranges::ElementsOf(right->traverse_inorder());
            }
        }
    };
} // namespace retro::ranges::testing

TEST_CASE("Can create a lazily evaluated generator", "[ranges]") {
    using namespace retro::ranges::testing;
    SECTION("Use a generator to loop through some numbers") {
        std::vector<int> numbers;
        for (int i : generate_integers(5)) {
            numbers.push_back(i);
        }
        CHECK(numbers == std::vector({0, 1, 2, 3, 4}));
    }

    SECTION("Can use a generator with a range pipe") {
        auto numbers = generate_integers(10) | retro::ranges::views::filter([](int value) { return value % 2 == 0; }) |
                       retro::ranges::to<std::vector>();
        CHECK(numbers == std::vector({0, 2, 4, 6, 8}));
    }

    SECTION("Can traverse a nested range using ElementsOf") {
        std::array<Tree<char>, 7> tree;
        tree[0] = {'D', &tree[1], &tree[2]};
        tree[1] = {'B', &tree[3], &tree[4]};
        tree[2] = {'F', &tree[5], &tree[6]};
        tree[3] = {'A'};
        tree[4] = {'C'};
        tree[5] = {'E'};
        tree[6] = {'G'};

        std::vector<char> values;
        for (auto value : tree[0].traverse_inorder()) {
            values.push_back(value);
        }
        CHECK(values == std::vector({'A', 'B', 'C', 'D', 'E', 'F', 'G'}));
    }
}