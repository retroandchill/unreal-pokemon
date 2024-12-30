/**
 * @file Operators.h
 * @brief Basic operator functors for usage in functional callbacks.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/Concepts/Operators.h"
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro {

    /**
     * @class EqualsFunction
     * @brief A class to compare two  values for equality.
     */
    struct EqualsFunction {
        /**
         * Perform the equality comparison on the target values.
         *
         * @tparam T The type of the left-hand side
         * @tparam U The type of the right-hand side
         * @param Lhs The left-hand side value
         * @param Rhs The right-hand side value
         * @return The result of the comparison
         */
        template <typename T, typename U = T>
            requires EqualityComparable<T, U>
        constexpr bool operator()(const T &Lhs, const U &Rhs) const {
            return Lhs == Rhs;
        }
    };

    /**
     * @brief An instance of the `Equals` struct used for comparing values for equality.
     */
    RETROLIB_EXPORT constexpr EqualsFunction Equals;

    /**
     * @class NotEqualFunction
     * @brief A class to compare two values for inequality.
     */
    struct NotEqualFunction {
        /**
         * Perform the inequality comparison on the target values.
         *
         * @tparam T The type of the left-hand side
         * @tparam U The type of the right-hand side
         * @param Lhs The left-hand side value
         * @param Rhs The right-hand side value
         * @return The result of the comparison
         */
        template <typename T, typename U = T>
            requires InequalityComparable<T, U>
        constexpr bool operator()(const T &Lhs, const U &Rhs) const {
            return Lhs != Rhs;
        }
    };

    /**
     * @brief An instance of the `NotEqual` struct used for comparing values for inequality.
     */
    RETROLIB_EXPORT constexpr NotEqualFunction NotEqual;

    /**
     * @class LessThanFunction
     * @brief A class to compare if one value is less than another.
     */
    struct LessThanFunction {
        /**
         * Perform the less than comparison on the target values.
         *
         * @tparam T The type of the left-hand side
         * @tparam U The type of the right-hand side
         * @param Lhs The left-hand side value
         * @param Rhs The right-hand side value
         * @return The result of the comparison
         */
        template <typename T, typename U = T>
            requires LessThanComparable<T, U>
        constexpr bool operator()(const T &Lhs, const U &Rhs) const {
            return Lhs < Rhs;
        }
    };

    /**
     * @brief An instance of the `LessThan` struct used for comparing if one value is less than another.
     */
    RETROLIB_EXPORT constexpr LessThanFunction LessThan;

    /**
     * @class LessThanOrEqualFunction
     * @brief A class to compare two values and check if one is less than or equal to the other.
     */
    struct LessThanOrEqualFunction {
        /**
         * Perform the less than or equal comparison on the target values.
         *
         * @tparam T The type of the left-hand side
         * @tparam U The type of the right-hand side
         * @param Lhs The left-hand side value
         * @param Rhs The right-hand side value
         * @return The result of the comparison
         */
        template <typename T, typename U = T>
            requires LessThanOrEqualComparable<T, U>
        constexpr bool operator()(const T &Lhs, const U &Rhs) const {
            return Lhs <= Rhs;
        }
    };

    /**
     * @brief An instance of the `LessThanOrEqual` struct used for comparing values to check if one is less than or
     * equal to the other.
     */
    RETROLIB_EXPORT constexpr LessThanOrEqualFunction LessThanOrEqual;

    /**
     * @class GreaterThanFunction
     * @brief A class to compare if one value is greater than another value.
     */
    struct GreaterThanFunction {
        /**
         * Perform the greater than comparison on the target values.
         *
         * @tparam T The type of the left-hand side
         * @tparam U The type of the right-hand side
         * @param Lhs The left-hand side value
         * @param Rhs The right-hand side value
         * @return The result of the comparison
         */
        template <typename T, typename U = T>
            requires GreaterThanComparable<T, U>
        constexpr bool operator()(const T &Lhs, const U &Rhs) const {
            return Lhs > Rhs;
        }
    };

    /**
     * @brief An instance of the `GreaterThan` struct used for comparing if one value is greater than another.
     */
    RETROLIB_EXPORT constexpr GreaterThanFunction GreaterThan;

    /**
     * @class GreaterThanOrEqualFunction
     * @brief A class to evaluate if a value is greater than or equal to another value.
     */
    struct GreaterThanOrEqualFunction {
        /**
         * Perform the greater than or equal comparison on the target values.
         *
         * @tparam T The type of the left-hand side
         * @tparam U The type of the right-hand side
         * @param Lhs The left-hand side value
         * @param Rhs The right-hand side value
         * @return The result of the comparison
         */
        template <typename T, typename U = T>
            requires GreaterThanOrEqualComparable<T, U>
        constexpr bool operator()(const T &Lhs, const U &Rhs) const {
            return Lhs >= Rhs;
        }
    };

    /**
     * @brief An instance of the `GreaterThanOrEqual` struct used for comparing if one value is greater than or equal
     * to another.
     */
    RETROLIB_EXPORT constexpr GreaterThanOrEqualFunction GreaterThanOrEqual;

    /**
     * @struct ThreeWayCompareFunction
     * @brief A functor for performing a three-way comparison between two values.
     *
     * Utilizes the spaceship operator (<=>) to compare two values of potentially different types
     * that satisfy the ThreeWayComparable concept, resulting in a value that indicates whether
     * the first value is less than, equal to, or greater than the second value.
     */
    struct ThreeWayCompareFunction {
        /**
         * @brief Compares two values using the three-way comparison operator.
         *
         * @tparam T The type of the left-hand side operand.
         * @tparam U The type of the right-hand side operand.
         *
         * @param Lhs The left-hand side value to be compared.
         * @param Rhs The right-hand side value to be compared.
         *
         * @return The result of the three-way comparison between lhs and rhs.
         */
        template <typename T, typename U = T>
            requires ThreeWayComparable<T, U>
        constexpr auto operator()(const T &Lhs, const U &Rhs) const {
            return Lhs <=> Rhs;
        }
    };

    /**
     * @brief A variable representing a three-way comparison operation.
     *
     * This constexpr variable is used to perform a three-way comparison between two values.
     */
    RETROLIB_EXPORT constexpr ThreeWayCompareFunction ThreeWayCompare;

    /**
     * @brief Represents an addition operation class.
     *
     * This class provides functionality to perform an addition operation on two input values.
     */
    struct AddFunction {
        /**
         * Perform the addition operation on the target values.
         *
         * @tparam T The type of the left-hand side
         * @tparam U The type of the right-hand side
         * @param Lhs The left-hand side value
         * @param Rhs The right-hand side value
         * @return The result of the operation
         */
        template <typename T, typename U = T>
            requires Addable<T, U>
        constexpr decltype(auto) operator()(const T &Lhs, const U &Rhs) const {
            return Lhs + Rhs;
        }
    };

    /**
     * @brief An instance of the FAdd class used to perform addition operations.
     *
     * This variable provides a convenient way to use the FAdd class for addition of two values.
     */
    RETROLIB_EXPORT constexpr AddFunction Add;

    /**
     * @brief Represents a subtraction operation class.
     *
     * This class provides functionality to perform a subtraction operation on two input values.
     */
    struct SubtractFunction {
        /**
         * Perform the subtraction operation on the target values.
         *
         * @tparam T The type of the left-hand side
         * @tparam U The type of the right-hand side
         * @param Lhs The left-hand side value
         * @param Rhs The right-hand side value
         * @return The result of the operation
         */
        template <typename T, typename U = T>
            requires Subtractable<T, U>
        constexpr decltype(auto) operator()(const T &Lhs, const U &Rhs) const {
            return Lhs - Rhs;
        }
    };

    /**
     * @brief An instance of the FSubtract class used to perform subtraction operations.
     *
     * This variable provides a convenient way to use the FSubtract class for subtraction of two values.
     */
    RETROLIB_EXPORT constexpr SubtractFunction Subtract;

    /**
     * @brief Represents a multiplication operation class.
     *
     * This class provides functionality to perform a multiplication operation on two input values.
     */
    struct MultiplyFunction {
        /**
         * Perform the multiplication operation on the target values.
         *
         * @tparam T The type of the left-hand side
         * @tparam U The type of the right-hand side
         * @param Lhs The left-hand side value
         * @param Rhs The right-hand side value
         * @return The result of the operation
         */
        template <typename T, typename U = T>
            requires Multipliable<T, U>
        constexpr decltype(auto) operator()(const T &Lhs, const U &Rhs) const {
            return Lhs * Rhs;
        }
    };

    /**
     * @brief An instance of the FMultiply class used to perform multiplication operations.
     *
     * This variable provides a convenient way to use the FMultiply class for multiplication of two values.
     */
    RETROLIB_EXPORT constexpr MultiplyFunction Multiply;

    /**
     * @brief Represents a division operation class.
     *
     * This class provides functionality to perform a division operation on two input values.
     */
    struct DivideFunction {
        /**
         * Perform the division operation on the target values.
         *
         * @tparam T The type of the left-hand side
         * @tparam U The type of the right-hand side
         * @param Lhs The left-hand side value
         * @param Rhs The right-hand side value
         * @return The result of the operation
         */
        template <typename T, typename U = T>
            requires Dividable<T, U>
        constexpr decltype(auto) operator()(const T &Lhs, const U &Rhs) const {
            return Lhs / Rhs;
        }
    };

    /**
     * @brief An instance of the FDivide class used to perform division operations.
     *
     * This variable provides a convenient way to use the FDivide class for division of two values.
     */
    RETROLIB_EXPORT constexpr DivideFunction Divide;

    /**
     * @brief Represents a modulo operation class.
     *
     * This class provides functionality to perform a modulo operation on two input values.
     */
    struct ModuloFunction {
        /**
         * Perform the modulo operation on the target values.
         *
         * @tparam T The type of the left-hand side
         * @tparam U The type of the right-hand side
         * @param Lhs The left-hand side value
         * @param Rhs The right-hand side value
         * @return The result of the operation
         */
        template <typename T, typename U = T>
            requires Moduloable<T, U>
        constexpr decltype(auto) operator()(const T &Lhs, const U &Rhs) const {
            return Lhs % Rhs;
        }
    };

    /**
     * @brief An instance of the FModulo class used to perform modulo operations.
     *
     * This variable provides a convenient way to use the FModulo class for the modulo operation of two values.
     */
    RETROLIB_EXPORT constexpr ModuloFunction Modulo;
} // namespace retro