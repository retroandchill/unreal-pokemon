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

namespace Retro {
    /**
     * Concept for the minimum requirements for an iterator.
     *
     * @tparam T The iterator type
     */
    template <typename T>
    concept Iterator = requires(T Iter) {
        { ++Iter } -> std::same_as<T &>;
        *Iter;
    };

    /**
     * Concept for the minimum requirements for an iterator sentinel.
     *
     * @tparam S The sentinel type
     * @tparam I The iterator type
     */
    template <typename S, typename I>
    concept Sentinel = requires(S Sentinel, I Iterator) {
        { Iterator != Sentinel } -> std::same_as<bool>;
    };

    /**
     * The iterator type from a container.
     *
     * @tparam T The container type.
     */
    template <typename T>
    using TIteratorType = std::decay_t<decltype(std::declval<T>().begin())>;

    /**
     * The sentinel type from a container.
     *
     * @tparam T The container type.
     */
    template <typename T>
    using TSentinelType = std::decay_t<decltype(std::declval<T>().end())>;

    /**
     * The concept for checking if container has iterator support.
     *
     * @tparam T The container type.
     */
    template <typename T>
    concept Iterable = requires(T &&Range) {
        { std::forward<T>(Range).begin() } -> Iterator;
        { std::forward<T>(Range).end() } -> Sentinel<TIteratorType<T>>;
    };
} // namespace retro