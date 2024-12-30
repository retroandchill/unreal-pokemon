/**
 * @file AnyOf.h
 * @brief Functional binding helper for the any_of range operation.
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
    constexpr FunctorBindingInvoker<Functor, std::ranges::any_of> AnyOfCallback;

    /**
     * @brief A constexpr utility to evaluate if any element within a range satisfies a given condition.
     *
     * This function templa te facilitates the invocation of the `any_of` algorithm by forwarding
     * arguments to an extension method utilizing a specified invoker. The invoker encapsulates
     * the logic of the `any_of` operation and applies the provided functor to the range or elements
     * passed as arguments.
     *
     * @tparam A A parameter pack representing the types of the arguments to be forwarded to the invoker.
     *
     * @param Args A variadic pack of arguments to configure the operation. These include the range to be evaluated
     * and any additional arguments bound to the functor (predicate).
     *
     * @return The result of the `any_of` operation, determined by evaluating the condition across
     * the elements of the range. The actual return type is dependent on the invoker and the arguments provided.
     */
    RETROLIB_EXPORT template <auto Functor = DynamicFunctor, typename... A>
        requires ValidFunctorParameter<Functor>
    constexpr auto AnyOf(A &&...Args) {
        return ExtensionMethod<AnyOfCallback<Functor>>(std::forward<A>(Args)...);
    }
} // namespace retro::ranges