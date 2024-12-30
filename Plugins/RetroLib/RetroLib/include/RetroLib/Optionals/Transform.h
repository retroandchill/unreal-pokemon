/**
 * @file Transform.h
 * @brief Transform operation for optionals.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */

#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/Functional/FunctionalClosure.h"
#include "RetroLib/FunctionTraits.h"
#include "RetroLib/Optionals/Optional.h"

#include <ranges>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro::Optionals {

    template <template <typename...> typename O, typename T, typename U>
        requires OptionalType<O<std::decay_t<T>>>
    constexpr auto FromResult(const O<U> &, T &&Value) {
        if constexpr (Nullable<T, O>) {
            return OfNullable<O>(std::forward<T>(Value));
        } else if constexpr (std::is_lvalue_reference_v<T>) {
            return OfReference<O>(std::forward<T>(Value));
        } else {
            return Of<O>(std::forward<T>(Value));
        }
    }

    struct TransformInvoker {
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
            requires std::invocable<F, CommonReference<O>>
        constexpr auto operator()(O &&Optional, F &&Functor) const {
            using ResultType = decltype(FromResult(
                std::forward<O>(Optional), std::invoke(std::forward<F>(Functor), Get<O>(std::forward<O>(Optional)))));
            return HasValue(std::forward<O>(Optional))
                       ? FromResult(std::forward<O>(Optional),
                                     std::invoke(std::forward<F>(Functor), Get<O>(std::forward<O>(Optional))))
                       : ResultType();
        }
    };

    constexpr TransformInvoker TransformFunction;

    template <auto Functor = DynamicFunctor>
        requires ValidFunctorParameter<Functor>
    constexpr FunctorBindingInvoker<Functor, TransformFunction> TransformCallback;

    /**
     * Applies a transformation to the given arguments using the specified functor.
     * It creates an extension method invoker where the transformation takes place.
     *
     * @param Args The arguments to be passed to the transformation function.
     *             These arguments are forwarded to ensure perfect forwarding semantics.
     * @return The result of invoking the transformation method using the provided functor and arguments.
     *         The return type depends on the implementation of `ExtensionMethod` and `transform_invoker`.
     */
    RETROLIB_EXPORT template <auto Functor = DynamicFunctor, typename... A>
        requires ValidFunctorParameter<Functor>
    constexpr auto Transform(A &&...Args) {
        return ExtensionMethod<TransformCallback<Functor>>(std::forward<A>(Args)...);
    }

} // namespace retro::optionals