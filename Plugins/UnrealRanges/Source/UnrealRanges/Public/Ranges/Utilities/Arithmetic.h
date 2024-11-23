// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Concepts/Arithmetic.h"

namespace UE::Ranges {

    /**
     * @brief Represents an addition operation class.
     *
     * This class provides functionality to perform an addition operation on two input values.
     */
    struct FAdd {
        /**
         * Perform the addition operation on the target values.
         * 
         * @tparam T The type of the left-hand side
         * @tparam U The type of the right-hand side
         * @param LHS The left-hand side value
         * @param RHS The right-hand side value
         * @return The result of the operation
         */
        template <typename T, typename U = T>
            requires Addable<T, U>
        constexpr decltype(auto) operator()(const T& LHS, const U& RHS) const {
            return LHS + RHS;
        }
    };

    /**
     * @brief An instance of the FAdd class used to perform addition operations.
     *
     * This variable provides a convenient way to use the FAdd class for addition of two values.
     */
    constexpr FAdd Add;

    /**
     * @brief Represents a subtraction operation class.
     *
     * This class provides functionality to perform a subtraction operation on two input values.
     */
    struct FSubtract {
        /**
         * Perform the subtraction operation on the target values.
         * 
         * @tparam T The type of the left-hand side
         * @tparam U The type of the right-hand side
         * @param LHS The left-hand side value
         * @param RHS The right-hand side value
         * @return The result of the operation
         */
        template <typename T, typename U = T>
            requires Subtractable<T, U>
        constexpr decltype(auto) operator()(const T& LHS, const U& RHS) const {
            return LHS - RHS;
        }
    };

    /**
     * @brief An instance of the FSubtract class used to perform subtraction operations.
     *
     * This variable provides a convenient way to use the FSubtract class for subtraction of two values.
     */
    constexpr FSubtract Subtract;

    /**
     * @brief Represents a multiplication operation class.
     *
     * This class provides functionality to perform a multiplication operation on two input values.
     */
    struct FMultiply {
        /**
         * Perform the multiplication operation on the target values.
         * 
         * @tparam T The type of the left-hand side
         * @tparam U The type of the right-hand side
         * @param LHS The left-hand side value
         * @param RHS The right-hand side value
         * @return The result of the operation
         */
        template <typename T, typename U = T>
            requires Multipliable<T, U>
        constexpr decltype(auto) operator()(const T& LHS, const U& RHS) const {
            return LHS * RHS;
        }
    };

    /**
     * @brief An instance of the FMultiply class used to perform multiplication operations.
     *
     * This variable provides a convenient way to use the FMultiply class for multiplication of two values.
     */
    constexpr FMultiply Multiply;

    /**
     * @brief Represents a division operation class.
     *
     * This class provides functionality to perform a division operation on two input values.
     */
    struct FDivide {
        /**
         * Perform the division operation on the target values.
         * 
         * @tparam T The type of the left-hand side
         * @tparam U The type of the right-hand side
         * @param LHS The left-hand side value
         * @param RHS The right-hand side value
         * @return The result of the operation
         */
        template <typename T, typename U = T>
            requires Dividable<T, U>
        constexpr decltype(auto) operator()(const T& LHS, const U& RHS) const {
            return LHS / RHS;
        }
    };

    /**
     * @brief An instance of the FDivide class used to perform division operations.
     *
     * This variable provides a convenient way to use the FDivide class for division of two values.
     */
    constexpr FDivide Divide;

    /**
     * @brief Represents a modulo operation class.
     *
     * This class provides functionality to perform a modulo operation on two input values.
     */
    struct FModulo {
        /**
         * Perform the modulo operation on the target values.
         * 
         * @tparam T The type of the left-hand side
         * @tparam U The type of the right-hand side
         * @param LHS The left-hand side value
         * @param RHS The right-hand side value
         * @return The result of the operation
         */
        template <typename T, typename U = T>
            requires Moduloable<T, U>
        constexpr decltype(auto) operator()(const T& LHS, const U& RHS) const {
            return LHS % RHS;
        }
    };

    /**
     * @brief An instance of the FModulo class used to perform modulo operations.
     *
     * This variable provides a convenient way to use the FModulo class for the modulo operation of two values.
     */
    constexpr FModulo Modulo;
    
}
