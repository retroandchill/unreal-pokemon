/**
 * @file Filter.h
 * @brief Specialized constexpr filter operation.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */

#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/Functional/CreateBinding.h"
#include "RetroLib/FunctionTraits.h"

#include <ranges>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace retro::ranges::views {

    /**
     * Filters a given range using a provided predicate or multiple arguments that bind to a predicate.
     *
     * @param range The input range to be filtered.
     * @param args Variadic arguments used to create the predicate binding.
     * @return A range view containing elements from the input range that satisfy the predicate.
     */
    RETROLIB_EXPORT template <std::ranges::input_range R, typename... A>
    constexpr decltype(auto) filter(R &&range, A &&...args) {
        return std::ranges::views::filter(std::forward<R>(range), create_binding(std::forward<A>(args)...));
    }

    /**
     * Filters a range based on a callable predicate or filter condition provided.
     *
     * @param args The arguments used to create the filter binding, typically including the range and a callable
     * predicate. These arguments will be forwarded to create the necessary filter binding.
     * @return A range view that only includes elements satisfying the given filter condition.
     */
    RETROLIB_EXPORT template <typename... A>
    constexpr decltype(auto) filter(A &&...args) {
        return std::ranges::views::filter(create_binding(std::forward<A>(args)...));
    }

    /**
     * Filters a given range using a functor that determines which elements to include.
     *
     * This method applies a filtering operation on the provided range, producing a new
     * range containing only those elements for which the bound functor returns true.
     *
     * @param range A forwardable range of elements to filter. The range should support
     *              copying or moving and must be compatible with std::ranges::views::filter.
     * @param args The binding arguments for the functor
     * @return An instance of a range, resulting from the application of the filter view
     *         with the specified functor, containing only elements that satisfy the functor's criteria.
     */
    RETROLIB_EXPORT template <auto Functor, std::ranges::input_range R, typename... A>
        requires HasFunctionCallOperator<decltype(Functor)>
    constexpr decltype(auto) filter(R &&range, A &&...args) {
        return std::ranges::views::filter(std::forward<R>(range), create_binding<Functor>(std::forward<A>(args)...));
    }

    /**
     * Applies a filter operation on a range by utilizing a functor to determine
     * the criteria for selecting elements from the range. It uses the
     * standard library's ranges view filter mechanism.
     *
     * @param args The binding arguments for the functor
     * @return A range view that filters elements based on the provided functor.
     *         Only elements that satisfy the condition specified by the functor
     *         will be included in the view.
     */
    RETROLIB_EXPORT template <auto Functor, typename... A>
        requires HasFunctionCallOperator<decltype(Functor)>
    constexpr decltype(auto) filter(A &&...args) {
        return std::ranges::views::filter(create_binding<Functor>(std::forward<A>(args)...));
    }

} // namespace retro::ranges::views