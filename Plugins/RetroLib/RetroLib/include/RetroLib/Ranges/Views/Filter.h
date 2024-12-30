/**
 * @file Filter.h
 * @brief Specialized constexpr filter operation.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */

#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/Functional/FunctionalClosure.h"
#include "RetroLib/FunctionTraits.h"

#include <ranges>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro::Ranges::Views {

    template <auto Functor = DynamicFunctor>
        requires ValidFunctorParameter<Functor>
    constexpr FunctorBindingInvoker<Functor, std::ranges::views::filter> FilterInvoker;

    /**
     * Applies a filter transformation on the given arguments using the specified functor.
     *
     * @param Args Variadic template arguments representing the elements or ranges to be filtered.
     *             These will be forwarded to the filter operation.
     * @return A transformed range or sequence obtained by applying the filter operation
     *         defined by the Functor to the given arguments.
     */
    RETROLIB_EXPORT template <auto Functor = DynamicFunctor, typename... A>
        requires ValidFunctorParameter<Functor>
    constexpr auto Filter(A &&...Args) {
        return ExtensionMethod<FilterInvoker<Functor>>(std::forward<A>(Args)...);
    }

} // namespace retro::ranges::views