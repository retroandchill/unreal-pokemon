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

namespace retro::ranges::views {

    template <auto Functor = dynamic_functor>
        requires ValidFunctorParameter<Functor>
    constexpr FunctorBindingInvoker<Functor, std::ranges::views::transform> transform_invoker;

    /**
     * Applies a transformation operation on the given arguments, using the specified Functor.
     * This method wraps the transformation logic within an extension method.
     *
     * @tparam A Variadic template for the argument types to be passed to the transformation.
     * @tparam Functor The callable object or function used for the transformation operation.
     *
     * @param args The arguments to be passed to the transformation operation. The parameters
     *        will be perfectly forwarded to the underlying implementation.
     *
     * @return A transformed result wrapped in an extension method that applies the given Functor
     *         with the specified arguments.
     */
    RETROLIB_EXPORT template <auto Functor = dynamic_functor, typename... A>
        requires ValidFunctorParameter<Functor>
    constexpr auto transform(A &&...args) {
        return extension_method<transform_invoker<Functor>>(std::forward<A>(args)...);
    }
} // namespace retro::ranges::views
