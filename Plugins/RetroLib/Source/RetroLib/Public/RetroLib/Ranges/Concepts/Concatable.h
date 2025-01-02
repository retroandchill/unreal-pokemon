/**
 * @file Concatable.h
 * @brief Concepts for judging the concatability to multiple ranges
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#include "RetroLib/Ranges/FeatureBridge.h"

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro::Ranges {
    /**
     * The reference type for the reference type of multiple ranges.
     *
     * @tparam R The source range types
     */
    template <typename... R>
    using TConcatReference = std::common_reference_t<std::ranges::range_reference_t<R>...>;

    /**
     * The value type for the value type of multiple ranges.
     *
     * @tparam R The source range types
     */
    template <typename... R>
    using TConcatValue = std::common_type_t<std::ranges::range_value_t<R>...>;

    /**
     * The r-value reference type for the r-value reference type of multiple ranges.
     *
     * @tparam R The source range types
     */
    template <typename... R>
    using TConcatRValueReference = std::common_reference_t<std::ranges::range_rvalue_reference_t<R>...>;

    /**
     * Implementation to check if a concatenation is indirectly readable.
     *
     * @tparam T The reference type of the range
     * @tparam R The r-value reference type
     * @tparam I The iterator type
     */
    template <typename T, typename R, typename I>
    concept ConcatIndirectlyReadableImpl = requires(const I it) {
        { *it } -> std::convertible_to<T>;
        { std::ranges::iter_move(it) } -> std::convertible_to<R>;
    };

    /**
     * Concept to check if a concatenation is indirectly readable.
     *
     * @tparam R The range types to check against
     */
    template <typename... R>
    concept ConcatIndirectlyReadable =
        std::common_reference_with<TConcatReference<R...> &&, TConcatValue<R...> &> &&
        std::common_reference_with<TConcatReference<R...> &&, TConcatRValueReference<R...> &&> &&
        std::common_reference_with<TConcatRValueReference<R...> &&, TConcatValue<R...> const &> &&
        (ConcatIndirectlyReadableImpl<TConcatReference<R...>, TConcatRValueReference<R...>, std::ranges::iterator_t<R>> &&
         ...);

    /**
     * Concept to check if two or more range types can be concatenated
     *
     * @tparam R The source range type
     */
    template <typename... R>
    concept Concatable = requires {
        typename TConcatReference<R...>;
        typename TConcatValue<R...>;
        typename TConcatRValueReference<R...>;
    } && ConcatIndirectlyReadable<R...>;
} // namespace retro::ranges
