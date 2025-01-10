/**
 * @file RangeBasics.h
 * @brief Basic concepts used by all ranges.
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

namespace Retro::Ranges {

    RETROLIB_EXPORT enum class ECardinality : std::ptrdiff_t { Infinite = -3, Unknown = -2, Finite = -1 };

    struct FBeginTag {};
    struct FEndTag {};

    RETROLIB_EXPORT template <typename R>
    concept SimpleView = std::same_as<std::ranges::iterator_t<R>, std::ranges::iterator_t<const R>> &&
                         std::same_as<std::ranges::sentinel_t<R>, std::ranges::sentinel_t<const R>>;

    RETROLIB_EXPORT template <typename R>
    concept RangeWithMovableReference =
        std::ranges::input_range<R> && std::move_constructible<std::ranges::range_reference_t<R>> &&
        std::move_constructible<std::ranges::range_reference_t<R>>;

} // namespace Retro::Ranges