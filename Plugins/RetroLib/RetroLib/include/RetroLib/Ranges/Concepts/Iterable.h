/**
 * @file Iterable.h
 * @brief Concepts pertaining to iterability.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */

#pragma once

#if !RETROLIB_WITH_MODULES
#include <ranges>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace retro {
    /**
     * Concept for the minimum requirements for an iterator.
     *
     * @tparam T The iterator type
     */
    template <typename T>
    concept Iterator = requires(T iter) {
        { ++iter } -> std::same_as<T &>;
        *iter;
    };

    /**
     * Concept for the minimum requirements for an iterator sentinel.
     *
     * @tparam S The sentinel type
     * @tparam I The iterator type
     */
    template <typename S, typename I>
    concept Sentinel = requires(S s, I i) {
        { i != s } -> std::same_as<bool>;
    };

    /**
     * The iterator type from a container.
     *
     * @tparam T The container type.
     */
    template <typename T>
    using IteratorType = std::decay_t<decltype(std::declval<T>().begin())>;

    /**
     * The sentinel type from a container.
     *
     * @tparam T The container type.
     */
    template <typename T>
    using SentinelType = std::decay_t<decltype(std::declval<T>().end())>;

    /**
     * The concept for checking if container has iterator support.
     *
     * @tparam T The container type.
     */
    template <typename T>
    concept Iterable = requires(T &&range) {
        { std::forward<T>(range).begin() } -> Iterator;
        { std::forward<T>(range).end() } -> Sentinel<IteratorType<T>>;
    };
} // namespace retro