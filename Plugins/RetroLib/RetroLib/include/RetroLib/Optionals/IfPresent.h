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

namespace retro::optionals {
    struct IfPresentInvoker {
        /**
         * Invokes the provided functor with the value contained in the optional if it has a value.
         *
         * @param optional The optional object to be checked and accessed.
         * @param functor The callable object to be invoked with the value of the optional if it exists.
         */
        template <OptionalType O, typename F>
            requires std::invocable<F, CommonReference<O>>
        constexpr void operator()(O &&optional, F &&functor) const {
            if (has_value(optional)) {
                std::invoke(std::forward<F>(functor), get(std::forward<O>(optional)));
            }
        }
    };

    constexpr IfPresentInvoker if_present_function;

    template <auto Functor = dynamic_functor>
        requires ValidFunctorParameter<Functor>
    constexpr FunctorBindingInvoker<Functor, if_present_function> if_present_invoker;

    /**
     * Executes the provided functionality if a certain condition is met.
     * This method forwards the input arguments to the underlying invoker,
     * leveraging deferred execution and composition.
     *
     * @param args Input arguments that are forwarded to the invoker. These arguments
     *             are passed as a parameter pack of type A and will be used
     *             by the underlying execution logic.
     * @return The result of invoking the extension method with the provided arguments,
     *         which acts as a wrapper for if_present_invoker with Functor.
     */
    RETROLIB_EXPORT template <auto Functor = dynamic_functor, typename... A>
        requires ValidFunctorParameter<Functor>
    constexpr auto if_present(A &&...args) {
        return extension_method<if_present_invoker<Functor>>(std::forward<A>(args)...);
    }
} // namespace retro::optionals