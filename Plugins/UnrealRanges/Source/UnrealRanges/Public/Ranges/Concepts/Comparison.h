// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

namespace UE::Ranges {
    /**
     * Checks if the two types have the inequality (!=) operator.
     *
     * @tparam T The type of the left-hand side
     * @tparam U The type of the right-hand side
     */
    template <typename T, typename U = T>
    concept InequalityComparable = requires(const T &LHS, const U &RHS) {
        { LHS != RHS } -> std::convertible_to<bool>;
    };

    /**
     * Checks if the two types have the less than (<) operator.
     *
     * @tparam T The type of the left-hand side
     * @tparam U The type of the right-hand side
     */
    template <typename T, typename U = T>
    concept LessThanComparable = requires(const T &LHS, const U &RHS) {
        { LHS < RHS } -> std::convertible_to<bool>;
    };

    /**
     * Checks if the two types have the less than or equal (<=) operator.
     *
     * @tparam T The type of the left-hand side
     * @tparam U The type of the right-hand side
     */
    template <typename T, typename U = T>
    concept LessThanOrEqualComparable = requires(const T &LHS, const U &RHS) {
        { LHS <= RHS } -> std::convertible_to<bool>;
    };

    /**
     * Checks if the two types have the greater than (>) operator.
     *
     * @tparam T The type of the left-hand side
     * @tparam U The type of the right-hand side
     */
    template <typename T, typename U = T>
    concept GreaterThanComparable = requires(const T &LHS, const U &RHS) {
        { LHS > RHS } -> std::convertible_to<bool>;
    };

    /**
     * Checks if the two types have the greater than or equal (>=) operator.
     *
     * @tparam T The type of the left-hand side
     * @tparam U The type of the right-hand side
     */
    template <typename T, typename U = T>
    concept GreaterThanOrEqualComparable = requires(const T &LHS, const U &RHS) {
        { LHS >= RHS } -> std::convertible_to<bool>;
    };
} // namespace UE::Ranges