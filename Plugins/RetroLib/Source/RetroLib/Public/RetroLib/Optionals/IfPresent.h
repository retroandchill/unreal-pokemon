/**
 * @file IfPresent.h
 * @brief Functional callback for handling if a value is present.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/RetroLibMacros.h"
#endif

#include "RetroLib/Functional/CreateBinding.h"
#include "RetroLib/Functional/FunctionalClosure.h"
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
    };

    RETROLIB_FUNCTIONAL_EXTENSION(RETROLIB_EXPORT, FIfPresentInvoker{}, IfPresent)
} // namespace retro::optionals