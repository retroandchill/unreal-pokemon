/**
 * @file ForEach.h
 * @brief Terminal handler for handling a for each loop on a range.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/Functional/CreateBinding.h"
#include "RetroLib/Functional/ExtensionMethods.h"
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
  constexpr FunctorBindingInvoker<Functor, std::ranges::for_each> ForEachCallback;

    /**
     * @brief Applies the `for_each` algorithm to the provided range and function using
     *        the specified invoker as an extension method.
     *
     * This function acts as a utility to extend the behavior of the `for_each` operation,
     * forwarding the given arguments to the `ExtensionMethod` framework for processing.
     *
     * @tparam A Variadic template parameter pack representing the argument types.
     * @param Args The arguments to be forwarded, which may include a range and a callable function.
     * @return The result of applying the `for_each_invoker` through the `ExtensionMethod` framework
     *         with the provided arguments.
     */
    RETROLIB_EXPORT template <auto Functor = DynamicFunctor, typename... A>
        requires ValidFunctorParameter<Functor>
    constexpr auto ForEach(A &&...Args) {
        return ExtensionMethod<ForEachCallback<Functor>>(std::forward<A>(Args)...);
    }

} // namespace retro::ranges