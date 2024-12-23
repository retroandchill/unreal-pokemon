/**
 * @file OrElseGet.h
 * @brief Allows the user to get the value of an optional or an alternative.
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

namespace retro::optionals {

    struct OrElseGetInvoker {
        /**
         * Invokes the provided functor or returns the value from the optional object.
         *
         * @param optional The optional object to inspect. If this object has a value, the value will be
         *                 extracted using `get` and returned.
         * @param functor The functor to invoke in case the optional object does not contain a value.
         * @return The value contained in the optional object if it has a value, otherwise the result of
         *         invoking the functor.
         */
        template <OptionalType O, typename F>
            requires std::invocable<F> && std::convertible_to<CommonReference<O>, std::invoke_result_t<F>>
        constexpr decltype(auto) operator()(O &&optional, F &&functor) const {
            if (has_value(std::forward<O>(optional))) {
                return static_cast<std::invoke_result_t<F>>(get(std::forward<O>(optional)));
            }

            return std::invoke(std::forward<F>(functor));
        }
    };

    constexpr OrElseGetInvoker or_else_get_function;

    template <auto Functor = dynamic_functor>
        requires ValidFunctorParameter<Functor>
    constexpr FunctorBindingInvoker<Functor, or_else_get_function> or_else_get_invoker;

    /**
     * Applies a provided function or functor when a source optional contains no value.
     * If the source optional is empty (std::nullopt), the provided function or functor is executed
     * and its result is returned. Otherwise, the value of the source optional is returned.
     *
     * @param args... The arguments forwarded to the functor or callable object to retrieve an alternative value
     *                when the source optional is empty.
     * @return The value contained in the source optional if it's not empty,
     *         otherwise, the result of invoking the provided callable object with the given arguments.
     */
    RETROLIB_EXPORT template <auto Functor = dynamic_functor, typename... A>
        requires ValidFunctorParameter<Functor>
    constexpr auto or_else_get(A &&...args) {
        return extension_method<or_else_get_invoker<Functor>>(std::forward<A>(args)...);
    }

} // namespace retro::optionals
