/**
 * @file AndThen.h
 * @brief Performs a transformation that returns an optional. Equivalent of Java's flatMap.
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

    /**
     * Applies a binding operation to an optional-like object if it has a value.
     *
     * @param optional The optional-like object from which to extract a value.
     * @param args The arguments used to create the binding function.
     * @return The result of invoking the binding function on the extracted value, if the optional-like object has a
     * value. Otherwise, returns an instance of std::invoke_result_t for the binding type and common reference type.
     */
    RETROLIB_EXPORT template <OptionalType O, typename... A>
        requires std::invocable<BindingType<A...>, CommonReference<O>> &&
                     OptionalType<std::invoke_result_t<BindingType<A...>, CommonReference<O>>>
    constexpr auto and_then(O &&optional, A &&...args) -> std::invoke_result_t<BindingType<A...>, CommonReference<O>> {
        return has_value(std::forward<O>(optional))
                   ? std::invoke(create_binding(std::forward<A>(args)...), get<O>(std::forward<O>(optional)))
                   : std::invoke_result_t<BindingType<A...>, CommonReference<O>>();
    }

    /**
     * Combines an optional object with additional arguments to produce a chained call.
     *
     * This method invokes `and_then` with the provided optional object and a binding
     * created using the specified arguments and Functor. It is primarily used to
     * streamline the chaining of computations or operations conditionally on the
     * presence of a value within the optional object.
     *
     * @param optional The optional object to be combined with the provided arguments.
     * @param args Additional arguments used to create a binding with the Functor.
     * @return A new result chaining the computation of the optional object and the provided arguments.
     */
    RETROLIB_EXPORT template <auto Functor, OptionalType O, typename... A>
        requires std::invocable<BindingType<decltype(Functor), A...>, CommonReference<O>> &&
                 OptionalType<std::invoke_result_t<BindingType<decltype(Functor), A...>, CommonReference<O>>>
    constexpr auto and_then(O &&optional, A &&...args) {
        return and_then(std::forward<O>(optional), create_binding<Functor>(std::forward<A>(args)...));
    }

    struct AndThenInvoker {
        template <OptionalType O, typename... A>
            requires std::invocable<BindingType<A...>, CommonReference<O>> &&
                     OptionalType<std::invoke_result_t<BindingType<A...>, CommonReference<O>>>
        constexpr auto operator()(O &&optional, A &&...args) const {
            return and_then(std::forward<O>(optional), std::forward<A>(args)...);
        }
    };

    constexpr AndThenInvoker and_then_invoker;

    template <auto Functor>
    struct ConstAndThenInvoker {
        template <OptionalType O, typename... A>
            requires std::invocable<BindingType<decltype(Functor), A...>, CommonReference<O>> &&
                     OptionalType<std::invoke_result_t<BindingType<decltype(Functor), A...>, CommonReference<O>>>
        constexpr auto operator()(O &&optional, A &&...args) const {
            return and_then<Functor>(std::forward<O>(optional), std::forward<A>(args)...);
        }
    };

    template <auto Functor>
    constexpr ConstAndThenInvoker<Functor> const_and_then_invoker;

    /**
     * Applies the specified arguments to an `and_then_invoker` extension method, forwarding them as needed.
     *
     * @tparam A Variadic template parameter pack deducing the argument types.
     * @param args The arguments to be passed to the `and_then_invoker` extension method.
     * @return A result obtained by invoking the `and_then_invoker` extension method with the provided arguments.
     */
    RETROLIB_EXPORT template <typename... A>
    constexpr auto and_then(A &&...args) {
        return extension_method<and_then_invoker>(std::forward<A>(args)...);
    }

    /**
     * Applies the `and_then` extension method with the specified invoker and arguments.
     *
     * The `and_then` method is typically used in functional programming-like pipelines
     * where a given invoker (in this case, `const_and_then_invoker`) processes the provided
     * arguments and returns a result.
     *
     * @param args List of arguments to be forwarded to the `const_and_then_invoker` for processing.
     *             These arguments are perfect-forwarded to maintain their value category and constness.
     * @return A result derived by applying the `const_and_then_invoker` and `Functor`
     *         to the forwarded arguments.
     */
    RETROLIB_EXPORT template <auto Functor, typename... A>
    constexpr auto and_then(A &&...args) {
        return extension_method<const_and_then_invoker<Functor>>(std::forward<A>(args)...);
    }

} // namespace retro::optionals