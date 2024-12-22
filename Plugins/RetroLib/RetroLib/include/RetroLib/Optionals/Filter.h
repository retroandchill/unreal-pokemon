/**
 * @file Filter.h
 * @brief Closure for filtering an optional value
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/Functional/CreateBinding.h"
#include "RetroLib/Functional/ExtensionMethods.h"
#include "RetroLib/Optionals/OptionalOperations.h"
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace retro::optionals {

    struct FilterInvoker {
        /**
         * @brief Applies the given functor to the value within an optional-like object and returns the optional based
         * on the functor's result.
         *
         * This function checks if the optional-like object contains a value and applies a functor to the value to
         * decide whether to return the same optional-like object or an empty one. If the functor returns true, the
         * optional-like object is returned as is, otherwise an empty one is returned.
         *
         * @tparam O The type of the optional-like object, can be an lvalue or rvalue reference.
         * @tparam F The type of the functor to be applied on the contained value.
         *
         * @param optional An optional-like object whose value is evaluated and possibly passed to the functor.
         * @param functor A callable object which takes the value from the optional and returns a boolean.
         *
         * @return If the optional contains a value and the functor returns true, the optional-like object is returned
         * with its value preserved. Otherwise, an empty optional-like object of the same type is returned.
         */
        template <OptionalType O, typename F>
            requires std::is_invocable_r_v<bool, F, CommonReference<O>>
        constexpr auto operator()(O &&optional, F &&functor) const {
            if constexpr (std::is_lvalue_reference_v<O>) {
                using FilteredType = decltype(make_optional_reference(std::forward<O>(optional)));
                return has_value(std::forward<O>(optional)) &&
                               std::invoke(std::forward<F>(functor), get<O>(std::forward<O>(optional)))
                           ? make_optional_reference(std::forward<O>(optional))
                           : FilteredType();
            } else {
                return has_value(std::forward<O>(optional)) &&
                               std::invoke(std::forward<F>(functor), get<O>(std::forward<O>(optional)))
                           ? O(std::forward<O>(optional))
                           : O();
            }
        }
    };

    constexpr FilterInvoker filter_function;

    template <auto Functor = dynamic_functor>
        requires ValidFunctorParameter<Functor>
    constexpr FunctorBindingInvoker<Functor, filter_function> filter_invoker;

    /**
     * @brief Filters the input based on the provided functor and arguments, returning a wrapped filtered result.
     *
     * This function applies an extension method that uses a specified `filter_invoker` to evaluate whether the input
     * satisfies a condition defined by the provided functor and accompanying arguments.
     *
     * @tparam Functor The type of the predicate or callable object used for filtering.
     * @tparam A The types of the arguments passed to the invoker.
     *
     * @param args Arguments to be forwarded to the `filter_invoker` along with the functor.
     *
     * @return An object resulting from the invocation of the specified filter logic, where the value is conditionally
     * preserved based on the evaluation of the provided functor.
     */
    RETROLIB_EXPORT template <auto Functor = dynamic_functor, typename... A>
        requires ValidFunctorParameter<Functor>
    constexpr auto filter(A &&...args) {
        return extension_method<filter_invoker<Functor>>(std::forward<A>(args)...);
    }

} // namespace retro::optionals