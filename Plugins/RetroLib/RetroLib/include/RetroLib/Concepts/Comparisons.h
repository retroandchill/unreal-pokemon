/**
 * @file Comparisons.h
 * @brief Concept definitions for comparisons
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include <type_traits>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace retro {
    /**
     * Checks if the two types have the equality (==) operator.
     *
     * @tparam T The type of the left-hand side
     * @tparam U The type of the right-hand side
     */
    RETROLIB_EXPORT template <typename T, typename U = T>
    concept EqualityComparable = requires(const T &lhs, const U &rhs) {
        { lhs == rhs } -> std::convertible_to<bool>;
    };

    /**
     * Checks if the two types have the inequality (!=) operator.
     *
     * @tparam T The type of the left-hand side
     * @tparam U The type of the right-hand side
     */
    RETROLIB_EXPORT template <typename T, typename U = T>
    concept InequalityComparable = requires(const T &lhs, const U &rhs) {
        { lhs != rhs } -> std::convertible_to<bool>;
    };

    /**
     * Checks if the two types have the less than (<) operator.
     *
     * @tparam T The type of the left-hand side
     * @tparam U The type of the right-hand side
     */
    RETROLIB_EXPORT template <typename T, typename U = T>
    concept LessThanComparable = requires(const T &lhs, const U &rhs) {
        { lhs < rhs } -> std::convertible_to<bool>;
    };

    /**
     * Checks if the two types have the less than or equal (<=) operator.
     *
     * @tparam T The type of the left-hand side
     * @tparam U The type of the right-hand side
     */
    RETROLIB_EXPORT template <typename T, typename U = T>
    concept LessThanOrEqualComparable = requires(const T &lhs, const U &rhs) {
        { lhs <= rhs } -> std::convertible_to<bool>;
    };

    /**
     * Checks if the two types have the greater than (>) operator.
     *
     * @tparam T The type of the left-hand side
     * @tparam U The type of the right-hand side
     */
    RETROLIB_EXPORT template <typename T, typename U = T>
    concept GreaterThanComparable = requires(const T &lhs, const U &rhs) {
        { lhs > rhs } -> std::convertible_to<bool>;
    };

    /**
     * Checks if the two types have the greater than or equal (>=) operator.
     *
     * @tparam T The type of the left-hand side
     * @tparam U The type of the right-hand side
     */
    RETROLIB_EXPORT template <typename T, typename U = T>
    concept GreaterThanOrEqualComparable = requires(const T &lhs, const U &rhs) {
        { lhs >= rhs } -> std::convertible_to<bool>;
    };

    /**
     * Checks if the two types have the three-way comparison(<=>) operator.
     *
     * @tparam T The type of the left-hand side
     * @tparam U The type of the right-hand side
     */
    RETROLIB_EXPORT template <typename T, typename U = T>
    concept ThreeWayComparable = requires(const T &lhs, const U &rhs) {
        { lhs <=> rhs } -> std::convertible_to<std::partial_ordering>;
    };
}