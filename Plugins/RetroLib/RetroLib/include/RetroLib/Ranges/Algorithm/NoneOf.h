/**
 * @file NoneOf.h
 * @brief Functional binding helper for the none_of range operation.
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
  constexpr FunctorBindingInvoker<Functor, std::ranges::none_of> NoneOfCallback;

  RETROLIB_EXPORT template <auto Functor = DynamicFunctor, typename... A>
      requires ValidFunctorParameter<Functor>
  constexpr auto NoneOf(A &&...Args) {
    return ExtensionMethod<NoneOfCallback<Functor>>(std::forward<A>(Args)...);
  }
} // namespace retro::ranges