/**
 * @file Transform.h
 * @brief Transform operation for optionals.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */

#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/Functional/CreateBinding.h"
#include "RetroLib/FunctionTraits.h"
#include "RetroLib/Optionals/Optional.h"

#include <ranges>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace retro::optionals {

    template <template <typename...> typename O, typename T, typename U>
        requires OptionalType<O<std::decay_t<T>>>
    constexpr auto from_result(const O<U> &, T &&value) {
        if constexpr (Nullable<T, O>) {
            return of_nullable<O>(std::forward<T>(value));
        } else if constexpr (std::is_lvalue_reference_v<T>) {
            return of_reference<O>(std::forward<T>(value));
        } else {
            return of<O>(std::forward<T>(value));
        }
    }

    struct TransformInvoker {
        /**
         * Invokes the given functor on the value held by the optional-like object, if it has a value.
         * If the optional-like object does not contain a value, returns a default-constructed result type.
         *
         * @param optional The optional-like object which might contain a value.
         * @param functor The callable object to be invoked with the value held by the optional-like object.
         * @return The result of invoking the functor on the value held by the optional-like object,
         *         or a default-constructed result type if the optional-like object does not contain a value.
         */
        template <OptionalType O, typename F>
            requires std::invocable<F, CommonReference<O>>
        constexpr auto operator()(O &&optional, F &&functor) const {
            using ResultType = decltype(from_result(
                std::forward<O>(optional), std::invoke(std::forward<F>(functor), get<O>(std::forward<O>(optional)))));
            return has_value(std::forward<O>(optional))
                       ? from_result(std::forward<O>(optional),
                                     std::invoke(std::forward<F>(functor), get<O>(std::forward<O>(optional))))
                       : ResultType();
        }
    };

    constexpr TransformInvoker transform_function;

    template <auto Functor = dynamic_functor>
        requires ValidFunctorParameter<Functor>
    constexpr FunctorBindingInvoker<Functor, transform_function> transform_invoker;

    /**
     * Applies a transformation to the given arguments using the specified functor.
     * It creates an extension method invoker where the transformation takes place.
     *
     * @param args The arguments to be passed to the transformation function.
     *             These arguments are forwarded to ensure perfect forwarding semantics.
     * @return The result of invoking the transformation method using the provided functor and arguments.
     *         The return type depends on the implementation of `extension_method` and `transform_invoker`.
     */
    RETROLIB_EXPORT template <auto Functor = dynamic_functor, typename... A>
        requires ValidFunctorParameter<Functor>
    constexpr auto transform(A &&...args) {
        return extension_method<transform_invoker<Functor>>(std::forward<A>(args)...);
    }

} // namespace retro::optionals