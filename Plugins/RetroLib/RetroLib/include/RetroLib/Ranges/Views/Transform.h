/**
 * @file Transform.h
 * @brief Constexpr transform view adapter.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/Functional/FunctionalClosure.h"

#include <ranges>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro::Ranges::Views {

    template <auto Functor = DynamicFunctor>
        requires ValidFunctorParameter<Functor>
    constexpr FunctorBindingInvoker<Functor, std::ranges::views::transform> TransformInvoker;

    /**
     * Applies a transformation operation on the given arguments, using the specified Functor.
     * This method wraps the transformation logic within an extension method.
     *
     * @tparam A Variadic template for the argument types to be passed to the transformation.
     * @tparam Functor The callable object or function used for the transformation operation.
     *
     * @param Args The arguments to be passed to the transformation operation. The parameters
     *        will be perfectly forwarded to the underlying implementation.
     *
     * @return A transformed result wrapped in an extension method that applies the given Functor
     *         with the specified arguments.
     */
    RETROLIB_EXPORT template <auto Functor = DynamicFunctor, typename... A>
        requires ValidFunctorParameter<Functor>
    constexpr auto Transform(A &&...Args) {
        return ExtensionMethod<TransformInvoker<Functor>>(std::forward<A>(Args)...);
    }
} // namespace retro::ranges::views
