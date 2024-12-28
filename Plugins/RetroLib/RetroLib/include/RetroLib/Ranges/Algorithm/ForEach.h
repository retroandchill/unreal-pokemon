/**
 * @file ForEach.h
 * @brief Terminal handler for handling a for each loop on a range.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/Functional/CreateBinding.h"
#include "RetroLib/Functional/ExtensionMethods.h"
#include "RetroLib/Ranges/FeatureBridge.h"

#include <algorithm>
#include <ranges>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace retro::ranges {

    /**
     * @brief Applies a function or callable object to each element in a given range.
     *
     * A wrapper function for `std::ranges::for_each` that applies a callable object
     * created by `create_binding` to the elements of the specified range.
     *
     * @tparam R The type of the range to iterate over. This can be a forwarding reference,
     * allowing both lvalue and rvalue ranges to be passed.
     * @tparam A Parameter pack representing the types of arguments to forward to `create_binding`.
     *
     * @param range The range of elements to process.
     * @param args Arbitrary arguments that will be forwarded to the `create_binding` function
     * for constructing a callable object.
     *
     * @return The result of invoking `std::ranges::for_each`. This includes the modified range
     * and the callable object applied to it, encapsulated in the returned type.
     *
     * @note This function utilizes `std::forward` to perfectly forward the range and arguments
     * to their respective targets. The lifetime of temporary objects passed to this function
     * must be managed to avoid dangling references.
     */
    RETROLIB_EXPORT template <std::ranges::input_range R, typename... A>
        requires std::invocable<BindingType<A...>, RangeCommonReference<R>>
    constexpr decltype(auto) for_each(R &&range, A &&...args) {
        return std::ranges::for_each(std::forward<R>(range), create_binding(std::forward<A>(args)...));
    }

    /**
     * Performs a for_each operation over the given range using a bound functor.
     *
     * @param range The input range to iterate over.
     * @param args Additional arguments to be used for creating the bound functor.
     * @return The result of the std::ranges::for_each operation over the given range.
     */
    RETROLIB_EXPORT template <auto Functor, std::ranges::input_range R, typename... A>
        requires HasFunctionCallOperator<decltype(Functor)> &&
                 std::invocable<ConstBindingType<Functor, A...>, RangeCommonReference<R>>
    constexpr decltype(auto) for_each(R &&range, A &&...args) {
        return std::ranges::for_each(std::forward<R>(range), create_binding<Functor>(std::forward<A>(args)...));
    }

    /**
     * @brief Wrapper struct providing a callable operator for invoking a function
     *        on each element of a range using the `for_each` algorithm.
     *
     * This struct utilizes a variadic template and constraints to ensure that the
     * specified function (deduced from the provided template arguments) can be invoked
     * with the common reference type of the specified range.
     */
    struct ForEachInvoker {
        /**
         * @brief Callable operator for applying a function to each element of a given range.
         *
         * This operator invokes the `for_each` algorithm on the provided range and forwards
         * any additional arguments to the invoked function.
         *
         * @param range The input range on which the `for_each` algorithm is applied.
         * @param args Additional arguments to be forwarded to the function during invocation.
         * @return The result of the `for_each` algorithm, which depends on the provided range
         *         and function.
         */
        template <std::ranges::input_range R, typename... A>
            requires std::invocable<BindingType<A...>, RangeCommonReference<R>>
        constexpr decltype(auto) operator()(R &&range, A &&...args) const {
            return for_each(std::forward<R>(range), std::forward<A>(args)...);
        }
    };

    /**
     * @brief Constant instance of ForEachInvoker, used for invoking a callable
     *        on each element of a range through the `for_each` algorithm.
     *
     * Provides a globally accessible and constexpr-qualified instance for use
     * with range-based operations, ensuring efficiency and compile-time evaluation
     * where applicable.
     */
    constexpr ForEachInvoker for_each_invoker;

    /**
     * @brief Wrapper struct providing a callable operator for invoking a function
     *        on each element of a range using the `for_each` algorithm with constant invocation.
     *
     * This struct ensures that the specified functor, when bound to the provided arguments,
     * can be invoked with the common reference type of the specified range. The invocation
     * utilizes a constant context to enforce immutability of the functor during execution.
     */
    template <auto Functor>
        requires HasFunctionCallOperator<decltype(Functor)>
    struct ForEachConstInvoker {
        /**
         * @brief Callable operator for applying a `for_each` algorithm on a given range.
         *
         * This operator invokes the `for_each` algorithm with the provided range and additional
         * arguments, forwarding them to ensure proper handling of lvalue and rvalue references.
         *
         * @param range The range on which the `for_each` algorithm will be applied.
         * @param args Additional arguments to be passed to the `for_each` algorithm.
         * @return The result of applying the `for_each` algorithm with the given range and arguments.
         */
        template <std::ranges::input_range R, typename... A>
            requires std::invocable<ConstBindingType<Functor, A...>, RangeCommonReference<R>>
        constexpr decltype(auto) operator()(R &&range, A &&...args) const {
            return for_each<Functor>(std::forward<R>(range), std::forward<A>(args)...);
        }
    };

    /**
     * @brief Constant instance of the `ForEachConstInvoker` template, parameterized by a `Functor`,
     *        used for invoking a constant functor on each element of a range.
     *
     * This object ensures that the provided functor operates on elements of a range under
     * constant access constraints, leveraging the `for_each` algorithm with immutable data.
     */
    template <auto Functor>
        requires HasFunctionCallOperator<decltype(Functor)>
    constexpr ForEachConstInvoker<Functor> for_each_const_invoker;

    /**
     * @brief Applies the `for_each` algorithm to the provided range and function using
     *        the specified invoker as an extension method.
     *
     * This function acts as a utility to extend the behavior of the `for_each` operation,
     * forwarding the given arguments to the `extension_method` framework for processing.
     *
     * @tparam A Variadic template parameter pack representing the argument types.
     * @param args The arguments to be forwarded, which may include a range and a callable function.
     * @return The result of applying the `for_each_invoker` through the `extension_method` framework
     *         with the provided arguments.
     */
    RETROLIB_EXPORT template <typename... A>
    constexpr decltype(auto) for_each(A &&...args) {
        return extension_method<for_each_invoker>(std::forward<A>(args)...);
    }

    /**
     * @brief Applies a given function to each element in a range using the `for_each` algorithm.
     *
     * This function forwards its arguments to an extension method that leverages the
     * `for_each_const_invoker` to execute the provided functor on the elements.
     *
     * @tparam A Variadic template parameter representing the types of arguments to be forwarded.
     * @param args The arguments to be forwarded to the extension method, which include the range
     *             and the callable functor.
     * @return The result of invoking the extension method with the forwarded arguments,
     *         typically the result of the `for_each` operation.
     */
    RETROLIB_EXPORT template <auto Functor, typename... A>
        requires HasFunctionCallOperator<decltype(Functor)>
    constexpr decltype(auto) for_each(A &&...args) {
        return extension_method<for_each_const_invoker<Functor>>(std::forward<A>(args)...);
    }

} // namespace retro::ranges