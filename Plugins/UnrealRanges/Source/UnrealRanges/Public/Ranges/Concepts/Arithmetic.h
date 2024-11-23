// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

namespace UE::Ranges {
    /**
     * Checks if the two types have the addition (+) operator.
     *
     * @tparam T The type of the left-hand side
     * @tparam U The type of the right-hand side
     */
    template <typename T, typename U = T>
    concept Addable = requires(const T &LHS, const U &RHS) { LHS + RHS; };

    /**
     * Checks if the two types have the subtraction (-) operator.
     *
     * @tparam T The type of the left-hand side
     * @tparam U The type of the right-hand side
     */
    template <typename T, typename U = T>
    concept Subtractable = requires(const T &LHS, const U &RHS) { LHS - RHS; };

    /**
     * Checks if the two types have the multiply (*) operator.
     *
     * @tparam T The type of the left-hand side
     * @tparam U The type of the right-hand side
     */
    template <typename T, typename U = T>
    concept Multipliable = requires(const T &LHS, const U &RHS) { LHS *RHS; };

    /**
     * Checks if the two types have the division (/) operator.
     *
     * @tparam T The type of the left-hand side
     * @tparam U The type of the right-hand side
     */
    template <typename T, typename U = T>
    concept Dividable = requires(const T &LHS, const U &RHS) { LHS / RHS; };

    /**
     * Checks if the two types have the modulo (%) operator.
     *
     * @tparam T The type of the left-hand side
     * @tparam U The type of the right-hand side
     */
    template <typename T, typename U = T>
    concept Moduloable = requires(const T &LHS, const U &RHS) { LHS % RHS; };
} // namespace UE::Ranges
