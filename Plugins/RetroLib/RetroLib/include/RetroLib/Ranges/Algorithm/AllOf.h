/**
 * @file AllOf.h
 * @brief Functional binding helper for the all_of range operation.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */

#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/Functional/CreateBinding.h"
#include "RetroLib/Functional/ExtensionMethods.h"
#include "RetroLib/Functional/FunctionalClosure.h"
#include "RetroLib/Ranges/FeatureBridge.h"

#include <algorithm>
#include <ranges>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro::Ranges {

    template <auto Functor = DynamicFunctor>
        requires ValidFunctorParameter<Functor>
    constexpr FunctorBindingInvoker<Functor, std::ranges::all_of> AllOfCallback;

    /**
     * @brief Constructs an extension method to evaluate if the predicate holds true for all elements.
     *
     * The `all_of` function creates an extension method using `all_of_const_invoker` that applies
     * the given arguments to a callable object for verification on a range of elements. It returns
     * whether all elements satisfy the given predicate.
     *
     * @tparam A The parameter pack of argument types passed to the `ExtensionMethod` and ultimately to the predicate
     * function.
     *
     * @param Args A variadic pack of arguments forwarded to the predicate or callable object within the
     * `ExtensionMethod`.
     *
     * @return An extension method constructed with `all_of_const_invoker`, which can be used to check conditions on a
     * range.
     */
    RETROLIB_EXPORT template <auto Functor = DynamicFunctor, typename... A>
        requires ValidFunctorParameter<Functor>
    constexpr auto AllOf(A &&...Args) {
        return ExtensionMethod<AllOfCallback<Functor>>(std::forward<A>(Args)...);
    }
} // namespace retro::ranges