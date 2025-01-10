/**
 * @file OrElseGet.h
 * @brief Allows the user to get the value of an optional or an alternative.
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

    struct FOrElseGetInvoker {
        /**
         * Invokes the provided functor or returns the value from the optional object.
         *
         * @param Optional The optional object to inspect. If this object has a value, the value will be
         *                 extracted using `get` and returned.
         * @param Functor The functor to invoke in case the optional object does not contain a value.
         * @return The value contained in the optional object if it has a value, otherwise the result of
         *         invoking the functor.
         */
        template <OptionalType O, typename F>
            requires std::invocable<F> && std::convertible_to<TCommonReference<O>, std::invoke_result_t<F>>
        constexpr decltype(auto) operator()(O &&Optional, F &&Functor) const {
            if (HasValue(std::forward<O>(Optional))) {
                return static_cast<std::invoke_result_t<F>>(Get(std::forward<O>(Optional)));
            }

            return std::invoke(std::forward<F>(Functor));
        }

#ifdef __UNREAL__
        template <OptionalType O, typename F>
            requires std::invocable<F> && UnrealInterface<TValueType<O>> &&
                     SpecializationOf<std::invoke_result_t<F>, TScriptInterface>
        constexpr decltype(auto) operator()(O &&Optional, F &&Functor) const {
            if (HasValue(std::forward<O>(Optional))) {
                return static_cast<std::invoke_result_t<F>>(Get(std::forward<O>(Optional))._getUObject());
            }

            return std::invoke(std::forward<F>(Functor));
        }
#endif
    };

    RETROLIB_EXPORT constexpr auto OrElseGet = ExtensionMethod<FOrElseGetInvoker{}>;
} // namespace Retro::Optionals
