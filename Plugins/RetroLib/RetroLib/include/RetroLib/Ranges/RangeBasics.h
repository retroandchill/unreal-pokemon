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

    RETROLIB_EXPORT enum class Cardinality : std::ptrdiff_t { Infinite = -3, Unknown = -2, Finite = -1 };

    struct BeginTag {};
    struct EndTag {};

    RETROLIB_EXPORT template <typename R>
    concept SimpleView = std::same_as<std::ranges::iterator_t<R>, std::ranges::iterator_t<const R>> &&
                         std::same_as<std::ranges::sentinel_t<R>, std::ranges::sentinel_t<const R>>;

} // namespace retro::ranges