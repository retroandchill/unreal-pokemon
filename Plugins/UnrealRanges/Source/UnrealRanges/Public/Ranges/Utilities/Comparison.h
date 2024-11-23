// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Concepts/Comparison.h"
#include "Ranges/Functional/Overloaded.h"

namespace UE::Ranges {
    /**
     * @class FEquals
     * @brief A class to compare two  values for equality.
     */
    struct FEquals {
        /**
         * Perform the equality comparison on the target values.
         *
         * @tparam T The type of the left-hand side
         * @tparam U The type of the right-hand side
         * @param LHS The left-hand side value
         * @param RHS The right-hand side value
         * @return The result of the comparison
         */
        template <typename T, typename U = T>
            requires std::equality_comparable_with<T, U>
        constexpr bool operator()(const T &LHS, const U &RHS) const {
            return LHS == RHS;
        }
    };

    /**
     * @brief An instance of the `FEquals` struct used for comparing values for equality.
     */
    constexpr FEquals Equals;

    /**
     * @class FNotEqual
     * @brief A class to compare two values for inequality.
     */
    struct FNotEqual {
        /**
         * Perform the inequality comparison on the target values.
         *
         * @tparam T The type of the left-hand side
         * @tparam U The type of the right-hand side
         * @param LHS The left-hand side value
         * @param RHS The right-hand side value
         * @return The result of the comparison
         */
        template <typename T, typename U = T>
            requires InequalityComparable<T, U>
        constexpr bool operator()(const T &LHS, const U &RHS) const {
            return LHS != RHS;
        }
    };

    /**
     * @brief An instance of the `FNotEqual` struct used for comparing values for inequality.
     */
    constexpr FNotEqual NotEqual;

    /**
     * @class FLessThan
     * @brief A class to compare if one value is less than another.
     */
    struct FLessThan {
        /**
         * Perform the less than comparison on the target values.
         *
         * @tparam T The type of the left-hand side
         * @tparam U The type of the right-hand side
         * @param LHS The left-hand side value
         * @param RHS The right-hand side value
         * @return The result of the comparison
         */
        template <typename T, typename U = T>
            requires LessThanComparable<T, U>
        constexpr bool operator()(const T &LHS, const U &RHS) const {
            return LHS < RHS;
        }
    };

    /**
     * @brief An instance of the `FLessThan` struct used for comparing if one value is less than another.
     */
    constexpr FLessThan LessThan;

    /**
     * @class FLessThanOrEqual
     * @brief A class to compare two values and check if one is less than or equal to the other.
     */
    struct FLessThanOrEqual {
        /**
         * Perform the less than or equal comparison on the target values.
         *
         * @tparam T The type of the left-hand side
         * @tparam U The type of the right-hand side
         * @param LHS The left-hand side value
         * @param RHS The right-hand side value
         * @return The result of the comparison
         */
        template <typename T, typename U = T>
            requires LessThanOrEqualComparable<T, U>
        constexpr bool operator()(const T &LHS, const U &RHS) const {
            return LHS <= RHS;
        }
    };

    /**
     * @brief An instance of the `FLessThanOrEqual` struct used for comparing values to check if one is less than or
     * equal to the other.
     */
    constexpr FLessThanOrEqual LessThanOrEqual;

    /**
     * @class FGreaterThan
     * @brief A class to compare if one value is greater than another value.
     */
    struct FGreaterThan {
        /**
         * Perform the greater than comparison on the target values.
         *
         * @tparam T The type of the left-hand side
         * @tparam U The type of the right-hand side
         * @param LHS The left-hand side value
         * @param RHS The right-hand side value
         * @return The result of the comparison
         */
        template <typename T, typename U = T>
            requires GreaterThanComparable<T, U>
        constexpr bool operator()(const T &LHS, const U &RHS) const {
            return LHS > RHS;
        }
    };

    /**
     * @brief An instance of the `FGreaterThan` struct used for comparing if one value is greater than another.
     */
    constexpr FGreaterThan GreaterThan;

    /**
     * @class FGreaterThanOrEqual
     * @brief A class to evaluate if a value is greater than or equal to another value.
     */
    struct FGreaterThanOrEqual {
        /**
         * Perform the greater than or equal comparison on the target values.
         *
         * @tparam T The type of the left-hand side
         * @tparam U The type of the right-hand side
         * @param LHS The left-hand side value
         * @param RHS The right-hand side value
         * @return The result of the comparison
         */
        template <typename T, typename U = T>
            requires GreaterThanOrEqualComparable<T, U>
        constexpr bool operator()(const T &LHS, const U &RHS) const {
            return LHS >= RHS;
        }
    };

    /**
     * @brief An instance of the `FGreaterThanOrEqual` struct used for comparing if one value is greater than or equal
     * to another.
     */
    constexpr FGreaterThanOrEqual GreaterThanOrEqual;

} // namespace UE::Ranges