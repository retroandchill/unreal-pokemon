/**
 * @file IfPresent.h
 * @brief Functional callback for handling if a value is present.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/Functional/CreateBinding.h"
#include "RetroLib/Functional/FunctionalClosure.h"
#include "RetroLib/Optionals/OptionalOperations.h"
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro::Optionals {
    struct IfPresentInvoker {
        /**
         * Invokes the provided functor with the value contained in the optional if it has a value.
         *
         * @param Optional The optional object to be checked and accessed.
         * @param Functor The callable object to be invoked with the value of the optional if it exists.
         */
        template <OptionalType O, typename F>
            requires std::invocable<F, CommonReference<O>>
        constexpr void operator()(O &&Optional, F &&Functor) const {
            if (HasValue(Optional)) {
                std::invoke(std::forward<F>(Functor), Get(std::forward<O>(Optional)));
            }
        }
    };

    constexpr IfPresentInvoker IfPresentFunction;

    template <auto Functor = DynamicFunctor>
        requires ValidFunctorParameter<Functor>
    constexpr FunctorBindingInvoker<Functor, IfPresentFunction> IfPresentCallback;

    /**
     * Executes the provided functionality if a certain condition is met.
     * This method forwards the input arguments to the underlying invoker,
     * leveraging deferred execution and composition.
     *
     * @param Args Input arguments that are forwarded to the invoker. These arguments
     *             are passed as a parameter pack of type A and will be used
     *             by the underlying execution logic.
     * @return The result of invoking the extension method with the provided arguments,
     *         which acts as a wrapper for if_present_invoker with Functor.
     */
    RETROLIB_EXPORT template <auto Functor = DynamicFunctor, typename... A>
        requires ValidFunctorParameter<Functor>
    constexpr auto IfPresent(A &&...Args) {
        return ExtensionMethod<IfPresentCallback<Functor>>(std::forward<A>(Args)...);
    }
} // namespace retro::optionals