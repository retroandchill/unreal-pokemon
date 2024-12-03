/**
 * @file Transform.h
 * @brief Constexpr transform view adapter.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/FunctionTraits.h"
#include "RetroLib/Functional/BindFunctor.h"
#include <ranges>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace retro::ranges::views {

    /**
     * @brief Transforms the given range using a specified functor.
     *
     * This function applies a transformation to each element of the provided range
     * by utilizing a functor. It uses the standard library's range transform view
     * to produce a new range where each element is the result of the functor applied
     * to the corresponding element in the input range.
     *
     * @tparam R A forwarding reference to the range type. Can be either an lvalue or rvalue.
     *           The range must satisfy the requirements imposed by std::ranges.
     * @tparam Functor The type of the functor used to transform the elements of the range.
     *                 The functor must be callable and return a type that can be stored
     *                 in the resulting range.
     *
     * @param range An R-value or L-value that represents the range of elements to be transformed.
     *
     * @return Returns a range that is the result of applying the transform operation using
     *         the specified functor on each element of the provided range.
     *
     * @note The function utilizes perfect forwarding for the input range to preserve
     *       its value category. The actual transformation is wrapped in a functor
     *       that is bound to the appropriate transformation mechanism.
     */
    RETROLIB_EXPORT template <auto Functor, std::ranges::input_range R>
        requires HasFunctionCallOperator<decltype(Functor)>
    constexpr decltype(auto) transform(R&& range) {
        return std::ranges::views::transform(std::forward<R>(range), bind_functor<Functor>());
    }

    /**
     * Applies a transformation to a range using a specified functor.
     *
     * This method leverages the C++ ranges library to transform each element
     * of a range by applying a functor. The functor is bound using the
     * `bind_functor` function, encapsulating the transformation logic.
     *
     * @return A view of the transformed range, where each element has been
     * modified according to the specified functor.
     */
    RETROLIB_EXPORT template <auto Functor>
        requires HasFunctionCallOperator<decltype(Functor)>
    constexpr decltype(auto) transform() {
        return std::ranges::views::transform(bind_functor<Functor>());
    }
}
