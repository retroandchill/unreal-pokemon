/**
 * @file IfPresent.h
 * @brief Functional callback for handling if a value is present.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#include "RetroLib/Functional/ExtensionMethods.h"
#include "RetroLib/Optionals/OptionalOperations.h"

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro::Optionals {
    struct FIfPresentInvoker {
        /**
         * Invokes the provided functor with the value contained in the optional if it has a value.
         *
         * @param Optional The optional object to be checked and accessed.
         * @param Functor The callable object to be invoked with the value of the optional if it exists.
         */
        template <OptionalType O, typename F>
            requires std::invocable<F, TCommonReference<O>>
        constexpr void operator()(O &&Optional, F &&Functor) const {
            if (HasValue(Optional)) {
                std::invoke(std::forward<F>(Functor), Get(std::forward<O>(Optional)));
            }
        }

        template <VoidOptional O, typename F>
            requires std::invocable<F>
        constexpr void operator()(O &&Optional, F &&Functor) const {
            if (HasValue(Optional)) {
                std::invoke(std::forward<F>(Functor));
            }
        }
    };

    RETROLIB_EXPORT constexpr auto IfPresent = ExtensionMethod<FIfPresentInvoker{}>;
} // namespace Retro::Optionals