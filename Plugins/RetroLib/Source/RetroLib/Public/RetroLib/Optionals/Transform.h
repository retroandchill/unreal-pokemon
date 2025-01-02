/**
 * @file Transform.h
 * @brief Transform operation for optionals.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */

#pragma once

#include "RetroLib/RetroLibMacros.h"
#include "RetroLib/Functional/FunctionalClosure.h"
#include "RetroLib/FunctionTraits.h"
#include "RetroLib/Optionals/OptionalOperations.h"

#if !RETROLIB_WITH_MODULES
#include <ranges>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro::Optionals {

    template <template <typename...> typename O, typename T, typename U>
    constexpr auto FromResult(const O<U> &, T &&Value) {
        if constexpr (Nullable<T, O>) {
            return OfNullable<O>(std::forward<T>(Value));
        } else if constexpr (std::is_lvalue_reference_v<T>) {
            return OfReference<O>(std::forward<T>(Value));
        } else {
            return Of<O>(std::forward<T>(Value));
        }
    }

    struct FTransformInvoker {
        /**
         * Invokes the given functor on the value held by the optional-like object, if it has a value.
         * If the optional-like object does not contain a value, returns a default-constructed result type.
         *
         * @param Optional The optional-like object which might contain a value.
         * @param Functor The callable object to be invoked with the value held by the optional-like object.
         * @return The result of invoking the functor on the value held by the optional-like object,
         *         or a default-constructed result type if the optional-like object does not contain a value.
         */
        template <OptionalType O, typename F>
            requires std::invocable<F, TCommonReference<O>>
        constexpr auto operator()(O &&Optional, F &&Functor) const {
            using ResultType = decltype(FromResult(
                std::forward<O>(Optional), std::invoke(std::forward<F>(Functor), Get<O>(std::forward<O>(Optional)))));
            return HasValue(std::forward<O>(Optional))
                       ? FromResult(std::forward<O>(Optional),
                                     std::invoke(std::forward<F>(Functor), Get<O>(std::forward<O>(Optional))))
                       : ResultType();
        }
    };

    RETROLIB_FUNCTIONAL_EXTENSION(RETROLIB_EXPORT, FTransformInvoker{}, Transform)
} // namespace retro::optionals