/**
 * @file CompleteType.h
 * @brief Concepts for identifying complete/incomplete types.
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

namespace Retro {
    /**
     * @struct IsComplete
     * @brief A type trait structure that inherits from std::false_type.
     *
     * This structure is typically used as a base trait that indicates
     * a condition or property that is incomplete or false. It can be
     * specialized or overridden to represent specific compile-time conditions.
     *
     * The default behavior (inheriting std::false_type) results in the
     * static member `value` being `false`, which may be leveraged in
     * metaprogramming techniques.
     */
    template <typename, typename = void>
    struct IsComplete : std::false_type {};

    /**
     * @struct IsComplete
     * @brief A type trait to determine if a given type T is complete.
     *
     * The IsComplete structure evaluates whether the provided type T is a
     * complete type. A type is considered complete when its size can be
     * determined using sizeof(T). This is achieved through substitution in
     * decltype(sizeof(T)) within the std::void_t metafunction.
     *
     * @tparam T The type to be checked for completeness.
     *
     * @note This type trait will inherit from std::true_type if T is complete.
     */
    template <typename T>
    struct IsComplete<T, std::void_t<decltype(sizeof(T))>> : std::true_type {};

    /**
     * Concept to check if a type is a complete type or not.
     *
     * @tparam T The type to be checked for completeness.
     */
    RETROLIB_EXPORT template <typename T>
    concept CompleteType = IsComplete<T>::value;

    /**
     * Concept to check if a type is an incomplete (i.e. forward declared) type or not.
     *
     * @tparam T The type to be checked for incompleteness.
     */
    RETROLIB_EXPORT template <typename T>
    concept IncompleteType = !CompleteType<T>;
} // namespace retro