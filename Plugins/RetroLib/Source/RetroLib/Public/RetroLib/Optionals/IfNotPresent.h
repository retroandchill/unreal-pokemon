// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "RetroLib/Functional/ExtensionMethods.h"
#include "RetroLib/Optionals/OptionalOperations.h"

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro::Optionals {
    struct FIfNotPresentInvoker {
        template <OptionalType O, typename F>
            requires (!ExpectedType<O>) && std::invocable<F>
        constexpr void operator()(O&& Optional, F&& Functor) const {
            if (!HasValue(Optional)) {
                std::invoke(std::forward<F>(Functor));
            }
        }

        template <ExpectedType O, typename F>
            requires std::invocable<F, TErrorType<O>>
        constexpr void operator()(O&& Optional, F&& Functor) const {
            if (!HasValue(Optional)) {
                std::invoke(std::forward<F>(Functor), GetError(std::forward<O>(Optional)));
            }
        }
    };

    RETROLIB_EXPORT constexpr auto IfNotPresent = ExtensionMethod<FIfNotPresentInvoker{}>;
}