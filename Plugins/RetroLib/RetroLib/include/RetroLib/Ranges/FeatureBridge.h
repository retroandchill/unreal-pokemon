/**
 * @file FeatureBridge.h
 * @brief Help to bridge some features that are missing from the current STL.
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
     * For some reason `range_common_reference_t` is missing on the linux clang builder, so this should add that
     * missing concept.
     *
     * @tparam R The range type to check
     */
    RETROLIB_EXPORT template <std::ranges::range R>
    using RangeCommonReference = std::iter_common_reference_t<std::ranges::iterator_t<R>>;
}
