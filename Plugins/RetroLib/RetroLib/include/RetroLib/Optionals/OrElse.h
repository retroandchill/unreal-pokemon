/**
 * @file OrElse.h
 * @brief Get an optional or another one if this one is empty.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */

#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/Functional/CreateBinding.h"
#include "RetroLib/Functional/FunctionalClosure.h"
#include "RetroLib/Optionals/Optional.h"
#include "RetroLib/Optionals/OptionalOperations.h"
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro::Optionals {

    struct OrElseInvoker {
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

    constexpr OrElseInvoker OrElseFunction;

    template <auto Functor = DynamicFunctor>
        requires ValidFunctorParameter<Functor>
    constexpr FunctorBindingInvoker<Functor, OrElseFunction> OrElseCallback;

    /**
     * Performs an operation using the provided arguments if the current context allows,
     * leveraging the or_else_invoker with the specified Functor.
     *
     * @tparam A The types of the arguments to be passed to the extension method.
     * @param Args The arguments to be forwarded to the extension method.
     * @return The result of invoking the or_else_invoker with the given Functor and forwarded arguments.
     */
    RETROLIB_EXPORT template <auto Functor = DynamicFunctor, typename... A>
        requires ValidFunctorParameter<Functor>
    constexpr auto OrElse(A &&...Args) {
        return ExtensionMethod<OrElseCallback<Functor>>(std::forward<A>(Args)...);
    }

} // namespace retro::optionals