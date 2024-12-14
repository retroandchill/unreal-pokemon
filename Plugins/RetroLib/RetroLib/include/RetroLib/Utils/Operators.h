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

namespace retro {

    /**
     * @class Equals
     * @brief A class to compare two  values for equality.
     */
    struct Equals {
        /**
         * Perform the equality comparison on the target values.
         *
         * @tparam T The type of the left-hand side
         * @tparam U The type of the right-hand side
         * @param lhs The left-hand side value
         * @param rhs The right-hand side value
         * @return The result of the comparison
         */
        template <typename T, typename U = T>
            requires EqualityComparable<T, U>
        constexpr bool operator()(const T &lhs, const U &rhs) const {
            return lhs == rhs;
        }
    };

    /**
     * @brief An instance of the `Equals` struct used for comparing values for equality.
     */
    RETROLIB_EXPORT constexpr Equals equals;

    /**
     * @class NotEqual
     * @brief A class to compare two values for inequality.
     */
    struct NotEqual {
        /**
         * Perform the inequality comparison on the target values.
         *
         * @tparam T The type of the left-hand side
         * @tparam U The type of the right-hand side
         * @param lhs The left-hand side value
         * @param rhs The right-hand side value
         * @return The result of the comparison
         */
        template <typename T, typename U = T>
            requires InequalityComparable<T, U>
        constexpr bool operator()(const T &lhs, const U &rhs) const {
            return lhs != rhs;
        }
    };

    /**
     * @brief An instance of the `NotEqual` struct used for comparing values for inequality.
     */
    RETROLIB_EXPORT constexpr NotEqual not_equal;

    /**
     * @class LessThan
     * @brief A class to compare if one value is less than another.
     */
    struct LessThan {
        /**
         * Perform the less than comparison on the target values.
         *
         * @tparam T The type of the left-hand side
         * @tparam U The type of the right-hand side
         * @param lhs The left-hand side value
         * @param rhs The right-hand side value
         * @return The result of the comparison
         */
        template <typename T, typename U = T>
            requires LessThanComparable<T, U>
        constexpr bool operator()(const T &lhs, const U &rhs) const {
            return lhs < rhs;
        }
    };

    /**
     * @brief An instance of the `LessThan` struct used for comparing if one value is less than another.
     */
    RETROLIB_EXPORT constexpr LessThan less_than;

    /**
     * @class LessThanOrEqual
     * @brief A class to compare two values and check if one is less than or equal to the other.
     */
    struct LessThanOrEqual {
        /**
         * Perform the less than or equal comparison on the target values.
         *
         * @tparam T The type of the left-hand side
         * @tparam U The type of the right-hand side
         * @param lhs The left-hand side value
         * @param rhs The right-hand side value
         * @return The result of the comparison
         */
        template <typename T, typename U = T>
            requires LessThanOrEqualComparable<T, U>
        constexpr bool operator()(const T &lhs, const U &rhs) const {
            return lhs <= rhs;
        }
    };

    /**
     * @brief An instance of the `LessThanOrEqual` struct used for comparing values to check if one is less than or
     * equal to the other.
     */
    RETROLIB_EXPORT constexpr LessThanOrEqual less_than_or_equal;

    /**
     * @class GreaterThan
     * @brief A class to compare if one value is greater than another value.
     */
    struct GreaterThan {
        /**
         * Perform the greater than comparison on the target values.
         *
         * @tparam T The type of the left-hand side
         * @tparam U The type of the right-hand side
         * @param lhs The left-hand side value
         * @param rhs The right-hand side value
         * @return The result of the comparison
         */
        template <typename T, typename U = T>
            requires GreaterThanComparable<T, U>
        constexpr bool operator()(const T &lhs, const U &rhs) const {
            return lhs > rhs;
        }
    };

    /**
     * @brief An instance of the `GreaterThan` struct used for comparing if one value is greater than another.
     */
    RETROLIB_EXPORT constexpr GreaterThan greater_than;

    /**
     * @class GreaterThanOrEqual
     * @brief A class to evaluate if a value is greater than or equal to another value.
     */
    struct GreaterThanOrEqual {
        /**
         * Perform the greater than or equal comparison on the target values.
         *
         * @tparam T The type of the left-hand side
         * @tparam U The type of the right-hand side
         * @param lhs The left-hand side value
         * @param rhs The right-hand side value
         * @return The result of the comparison
         */
        template <typename T, typename U = T>
            requires GreaterThanOrEqualComparable<T, U>
        constexpr bool operator()(const T &lhs, const U &rhs) const {
            return lhs >= rhs;
        }
    };

    /**
     * @brief An instance of the `GreaterThanOrEqual` struct used for comparing if one value is greater than or equal
     * to another.
     */
    RETROLIB_EXPORT constexpr GreaterThanOrEqual greater_than_or_equal;

    /**
     * @struct ThreeWayCompare
     * @brief A functor for performing a three-way comparison between two values.
     *
     * Utilizes the spaceship operator (<=>) to compare two values of potentially different types
     * that satisfy the ThreeWayComparable concept, resulting in a value that indicates whether
     * the first value is less than, equal to, or greater than the second value.
     */
    struct ThreeWayCompare {
        /**
         * @brief Compares two values using the three-way comparison operator.
         *
         * @tparam T The type of the left-hand side operand.
         * @tparam U The type of the right-hand side operand.
         *
         * @param lhs The left-hand side value to be compared.
         * @param rhs The right-hand side value to be compared.
         *
         * @return The result of the three-way comparison between lhs and rhs.
         */
        template <typename T, typename U = T>
            requires ThreeWayComparable<T, U>
        constexpr auto operator()(const T &lhs, const U &rhs) const {
            return lhs <=> rhs;
        }
    };

    /**
     * @brief A variable representing a three-way comparison operation.
     *
     * This constexpr variable is used to perform a three-way comparison between two values.
     */
    RETROLIB_EXPORT constexpr ThreeWayCompare three_way_compare;

    /**
     * @brief Represents an addition operation class.
     *
     * This class provides functionality to perform an addition operation on two input values.
     */
    struct Add {
        /**
         * Perform the addition operation on the target values.
         *
         * @tparam T The type of the left-hand side
         * @tparam U The type of the right-hand side
         * @param lhs The left-hand side value
         * @param rhs The right-hand side value
         * @return The result of the operation
         */
        template <typename T, typename U = T>
            requires Addable<T, U>
        constexpr decltype(auto) operator()(const T &lhs, const U &rhs) const {
            return lhs + rhs;
        }
    };

    /**
     * @brief An instance of the FAdd class used to perform addition operations.
     *
     * This variable provides a convenient way to use the FAdd class for addition of two values.
     */
    RETROLIB_EXPORT constexpr Add add;

    /**
     * @brief Represents a subtraction operation class.
     *
     * This class provides functionality to perform a subtraction operation on two input values.
     */
    struct Subtract {
        /**
         * Perform the subtraction operation on the target values.
         *
         * @tparam T The type of the left-hand side
         * @tparam U The type of the right-hand side
         * @param lhs The left-hand side value
         * @param rhs The right-hand side value
         * @return The result of the operation
         */
        template <typename T, typename U = T>
            requires Subtractable<T, U>
        constexpr decltype(auto) operator()(const T &lhs, const U &rhs) const {
            return lhs - rhs;
        }
    };

    /**
     * @brief An instance of the FSubtract class used to perform subtraction operations.
     *
     * This variable provides a convenient way to use the FSubtract class for subtraction of two values.
     */
    RETROLIB_EXPORT constexpr Subtract subtract;

    /**
     * @brief Represents a multiplication operation class.
     *
     * This class provides functionality to perform a multiplication operation on two input values.
     */
    struct Multiply {
        /**
         * Perform the multiplication operation on the target values.
         *
         * @tparam T The type of the left-hand side
         * @tparam U The type of the right-hand side
         * @param lhs The left-hand side value
         * @param rhs The right-hand side value
         * @return The result of the operation
         */
        template <typename T, typename U = T>
            requires Multipliable<T, U>
        constexpr decltype(auto) operator()(const T &lhs, const U &rhs) const {
            return lhs * rhs;
        }
    };

    /**
     * @brief An instance of the FMultiply class used to perform multiplication operations.
     *
     * This variable provides a convenient way to use the FMultiply class for multiplication of two values.
     */
    RETROLIB_EXPORT constexpr Multiply multiply;

    /**
     * @brief Represents a division operation class.
     *
     * This class provides functionality to perform a division operation on two input values.
     */
    struct Divide {
        /**
         * Perform the division operation on the target values.
         *
         * @tparam T The type of the left-hand side
         * @tparam U The type of the right-hand side
         * @param lhs The left-hand side value
         * @param rhs The right-hand side value
         * @return The result of the operation
         */
        template <typename T, typename U = T>
            requires Dividable<T, U>
        constexpr decltype(auto) operator()(const T &lhs, const U &rhs) const {
            return lhs / rhs;
        }
    };

    /**
     * @brief An instance of the FDivide class used to perform division operations.
     *
     * This variable provides a convenient way to use the FDivide class for division of two values.
     */
    RETROLIB_EXPORT constexpr Divide divide;

    /**
     * @brief Represents a modulo operation class.
     *
     * This class provides functionality to perform a modulo operation on two input values.
     */
    struct Modulo {
        /**
         * Perform the modulo operation on the target values.
         *
         * @tparam T The type of the left-hand side
         * @tparam U The type of the right-hand side
         * @param lhs The left-hand side value
         * @param rhs The right-hand side value
         * @return The result of the operation
         */
        template <typename T, typename U = T>
            requires Moduloable<T, U>
        constexpr decltype(auto) operator()(const T &lhs, const U &rhs) const {
            return lhs % rhs;
        }
    };

    /**
     * @brief An instance of the FModulo class used to perform modulo operations.
     *
     * This variable provides a convenient way to use the FModulo class for the modulo operation of two values.
     */
    RETROLIB_EXPORT constexpr Modulo modulo;
} // namespace retro