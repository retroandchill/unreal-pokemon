/**
 * @file OrElse.h
 * @brief Get an optional or another one if this one is empty.
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

    struct FOrElseInvoker {
        /**
         * Applies a functor to provide a fallback value if the given optional input has no value.
         * If the optional contains a value, it is returned as is.
         *
         * @param Optional The input optional object to be checked for a value.
         * @param Functor A callable object that provides a fallback value if the optional has no value.
         * @return The value contained in the optional if it exists, or the fallback value provided by the functor.
         */
        template <OptionalType O, typename F>
            requires std::invocable<F> && std::same_as<std::invoke_result_t<F>, std::decay_t<O>>
        constexpr std::decay_t<O> operator()(O &&Optional, F &&Functor) const {
            if (HasValue(std::forward<O>(Optional))) {
                return std::forward<O>(Optional);
            }

            return std::invoke(std::forward<F>(Functor));
        }
    };

    RETROLIB_EXPORT constexpr auto OrElse = ExtensionMethod<FOrElseInvoker{}>;

} // namespace Retro::Optionals