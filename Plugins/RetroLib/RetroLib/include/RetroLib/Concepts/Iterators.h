/**
 * @file Iterators.h
 * @brief Concepts for handling iterators.
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
     * Concept to check if an iterator can only be used for a single pass.
     */
    RETROLIB_EXPORT template <typename I>
    concept SinglePassIterator = std::input_or_output_iterator<I> && !std::forward_iterator<I>;
} // namespace retro