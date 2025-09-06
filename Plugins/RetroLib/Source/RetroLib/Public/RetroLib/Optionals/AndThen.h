/**
 * @file AndThen.h
 * @brief Performs a transformation that returns an optional. Equivalent of Java's flatMap.
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

namespace Retro::Optionals
{

    struct FAndThenInvoker
    {
        /**
         * Applies the provided functor to the contained value of the optional if it has a value,
         * otherwise returns the default value of the functor's return type.
         *
         * @param Optional The optional object which may or may not contain a value.
         *                 Must support `has_value` and `get` functions.
         * @param Functor The callable object to be invoked with the value contained in the optional.
         *                Must be invokable with the type `decltype(get(optional))` as a parameter.
         * @return The result of invoking the functor with the contained value of the optional,
         *         if the optional has a value. Otherwise, returns the default value of the functor's result type.
         */
        template <OptionalType O, typename F, typename... A>
            requires std::invocable<F, TCommonReference<O>> &&
                     OptionalType<std::invoke_result_t<F, TCommonReference<O>>> &&
                     CanPassOptional<std::invoke_result_t<F, TCommonReference<O>>, O, A...>
        constexpr auto operator()(O &&Optional, F &&Functor, A &&...Args) const
        {
            return HasValue(std::forward<O>(Optional))
                       ? std::invoke(std::forward<F>(Functor), Get<O>(std::forward<O>(Optional)))
                       : PassOptional<std::invoke_result_t<F, TCommonReference<O>>>(std::forward<O>(Optional),
                                                                                    std::forward<A>(Args)...);
        }

        template <VoidOptional O, typename F, typename... A>
            requires std::invocable<F> && OptionalType<std::invoke_result_t<F>> &&
                     CanPassOptional<std::invoke_result_t<F>, O, A...>
        constexpr auto operator()(O &&Optional, F &&Functor, A &&...Args) const
        {
            return HasValue(std::forward<O>(Optional))
                       ? std::invoke(std::forward<F>(Functor))
                       : PassOptional<std::invoke_result_t<F>>(std::forward<O>(Optional), std::forward<A>(Args)...);
        }
    };

    RETROLIB_EXPORT constexpr auto AndThen = ExtensionMethod<FAndThenInvoker{}>;
} // namespace Retro::Optionals