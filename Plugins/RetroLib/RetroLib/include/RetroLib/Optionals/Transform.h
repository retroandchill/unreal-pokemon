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

    /**
     * @brief Transforms an optional-like value by applying a function created from the provided arguments.
     *
     * This function checks if the input optional-like object contains a value. If it does, it invokes
     * a transformation function created with the provided arguments on the contained value. The result
     * of the transformation is wrapped in an appropriate return type. If the input does not contain a value,
     * the function returns a default-constructed instance of the return type.
     *
     * @tparam O The type of the input optional-like object.
     * @tparam A Parameter pack representing the types of arguments used to create the binding function.
     * @param optional The input optional-like object whose value might be transformed.
     * @param args A variadic set of arguments used to create the transformation function.
     * @return A value of appropriate type obtained from the transformation function if the input contains a value;
     *         otherwise, a default-constructed result type.
     *
     * @note The function is constexpr, enabling compile-time evaluation if used with constant expressions,
     *       provided all dependencies support it.
     * @note The behavior relies on utilities `has_value`, `get`, `from_result`, `create_binding`, and `std::invoke`
     *       for managing the optional-like object and executing the transformation.
     */
    RETROLIB_EXPORT template <OptionalType O, typename... A>
        requires std::is_invocable_v<BindingType<A...>, CommonReference<O>>
    constexpr auto transform(O &&optional, A &&...args) {
        using ResultType =
            decltype(from_result(std::forward<O>(optional), std::invoke(create_binding(std::forward<A>(args)...),
                                                                        get<O>(std::forward<O>(optional)))));
        return has_value(std::forward<O>(optional))
                   ? from_result(std::forward<O>(optional), std::invoke(create_binding(std::forward<A>(args)...),
                                                                        get<O>(std::forward<O>(optional))))
                   : ResultType();
    }

    /**
     * Transforms the given optional object using the provided arguments,
     * creating a binding using the specified Functor.
     *
     * @param optional The optional object to be transformed.
     * @param args The arguments to create a binding for the Functor.
     * @return The transformed optional object after applying the binding.
     */
    RETROLIB_EXPORT template <auto Functor, OptionalType O, typename... A>
        requires std::is_invocable_v<BindingType<decltype(Functor), A...>, CommonReference<O>>
    constexpr auto transform(O &&optional, A &&...args) {
        return transform(std::forward<O>(optional), create_binding<Functor>(std::forward<A>(args)...));
    }

    struct TransformInvoker {
        template <OptionalType O, typename... A>
            requires std::is_invocable_v<BindingType<A...>, CommonReference<O>>
        constexpr auto operator()(O &&optional, A &&...args) const {
            return transform(std::forward<O>(optional), std::forward<A>(args)...);
        }
    };

    constexpr TransformInvoker transform_invoker;

    template <auto Functor>
    struct ConstTransformInvoker {
        template <OptionalType O, typename... A>
            requires std::is_invocable_v<BindingType<decltype(Functor), A...>, CommonReference<O>>
        constexpr auto operator()(O &&optional, A &&...args) const {
            return transform<Functor>(std::forward<O>(optional), std::forward<A>(args)...);
        }
    };

    template <auto Functor>
    constexpr ConstTransformInvoker<Functor> const_transform_invoker;

    /**
     * @brief Applies a transformation to the provided arguments using the `transform_invoker`
     *        and returns the resulting value.
     *
     * @tparam A Variadic template parameter pack representing the types of the input arguments.
     * @param args The arguments to be transformed by the `extension_method` utilizing the `transform_invoker`.
     * @return The result of the transformation applied to the input arguments.
     *
     * @details
     * This function is designed to serve as a utility for applying transformations to input parameters.
     * Using the `transform_invoker`, it invokes the `extension_method` to perform the desired operation.
     * The purpose of the function is to abstract and streamline transformation operations applied
     * to different data types.
     */
    RETROLIB_EXPORT template <typename... A>
    constexpr auto transform(A &&...args) {
        return extension_method<transform_invoker>(std::forward<A>(args)...);
    }

    /**
     * @brief Applies a transformation using the specified functor to the given arguments.
     *
     * This method enables chaining transformations or operations using the provided functor
     * by forwarding the arguments to an internal invoker.
     *
     * @tparam A The type(s) of the arguments passed to the transformation.
     * @tparam Functor The functor used to perform the transformation.
     * @param args The arguments to be forwarded to the transformation.
     * @return Returns the result of the transformation wrapped in the invoker for further processing.
     */
    RETROLIB_EXPORT template <auto Functor, typename... A>
    constexpr auto transform(A &&...args) {
        return extension_method<const_transform_invoker<Functor>>(std::forward<A>(args)...);
    }

} // namespace retro::optionals