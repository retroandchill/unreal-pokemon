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

    template <auto Functor = dynamic_functor>
        requires ValidFunctorParameter<Functor>
    constexpr FunctorBindingInvoker<Functor, std::ranges::views::filter> filter_invoker;

    /**
     * Applies a filter transformation on the given arguments using the specified functor.
     *
     * @param args Variadic template arguments representing the elements or ranges to be filtered.
     *             These will be forwarded to the filter operation.
     * @return A transformed range or sequence obtained by applying the filter operation
     *         defined by the Functor to the given arguments.
     */
    RETROLIB_EXPORT template <auto Functor = dynamic_functor, typename... A>
        requires ValidFunctorParameter<Functor>
    constexpr auto filter(A &&...args) {
        return extension_method<filter_invoker<Functor>>(std::forward<A>(args)...);
    }

} // namespace retro::ranges::views