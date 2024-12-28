/**
 * @file AnyOf.h
 * @brief Functional binding helper for the any_of range operation.
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
     * @brief Checks if any of the elements in the given range satisfies the specified condition.
     *
     * This function uses `std::ranges::any_of` to determine if at least one element in the provided
     * range satisfies the condition created by binding the passed arguments.
     *
     * @param range The range of elements to be checked.
     * @param args Additional arguments used to create the binding function that defines the condition.
     * @return `true` if at least one element in the range satisfies the condition; `false` otherwise.
     */
    RETROLIB_EXPORT template <std::ranges::input_range R, typename... A>
        requires std::invocable<BindingType<A...>, RangeCommonReference<R>>
    constexpr bool any_of(R &&range, A &&...args) {
        return std::ranges::any_of(std::forward<R>(range), create_binding(std::forward<A>(args)...));
    }

    /**
     * Checks if any element in the given range satisfies a condition determined by provided arguments.
     *
     * @param range The range of elements to examine.
     * @param args The arguments to create the predicate used for checking the condition.
     * @return true if any element in the range satisfies the condition; otherwise, false.
     */
    RETROLIB_EXPORT template <auto Functor, std::ranges::input_range R, typename... A>
        requires HasFunctionCallOperator<decltype(Functor)> &&
                 std::invocable<ConstBindingType<Functor, A...>, RangeCommonReference<R>>
    constexpr bool any_of(R &&range, A &&...args) {
        return std::ranges::any_of(std::forward<R>(range), create_binding<Functor>(std::forward<A>(args)...));
    }

    /**
     * @brief A callable structure that evaluates if any element in a range satisfies a specified condition.
     *
     * The AnyOfInvoker struct provides an overload for the function-call operator `operator()`.
     * It operates on ranges and accepts additional arguments to bind with the predicate to be evaluated.
     * The function checks if any element of the provided range satisfies the condition defined
     * by invoking the specified predicate with the additional arguments.
     *
     * @tparam R A type satisfying the std::ranges::input_range concept.
     * @tparam A A parameter pack of additional argument types to be used for predicate binding.
     *
     * @requirement The passed arguments must satisfy the std::invocable constraint for the provided
     * predicate when bound using the `BindingType` that matches with the range's common reference type.
     *
     * @remarks The function dispatches to `any_of` and uses perfect forwarding for the range and additional arguments.
     *
     * @return A boolean value indicating whether any element in the range satisfies the predicate.
     */
    struct AnyOfInvoker {
        /**
         * @brief Checks if any element in the provided range satisfies a given predicate.
         *
         * This function-call operator dispatches the provided range and additional arguments
         * to the `any_of` algorithm to evaluate whether at least one element in the range
         * meets the condition defined by the predicate.
         *
         * @tparam R A type representing a range, satisfying the std::ranges::input_range concept.
         * @tparam A A parameter pack representing additional arguments for the predicate.
         *
         * @param range The range of elements to be checked.
         * @param args Additional arguments to be forwarded to the predicate.
         *
         * @return True if any element in the range satisfies the predicate; otherwise, false.
         */
        template <std::ranges::input_range R, typename... A>
            requires std::invocable<BindingType<A...>, RangeCommonReference<R>>
        constexpr bool operator()(R &&range, A &&...args) const {
            return any_of(std::forward<R>(range), std::forward<A>(args)...);
        }
    };

    /**
     * @brief A constexpr instance of AnyOfInvoker for evaluating predicates on ranges.
     *
     * The `any_of_invoker` provides a pre-defined, constant instance of the AnyOfInvoker
     * structure. It allows direct and efficient invocation of the `AnyOfInvoker` functionality
     * on ranges with minimal overhead, ensuring the predicate evaluation can be performed
     * without the need to explicitly create an AnyOfInvoker instance.
     *
     * @remarks This instance can be used directly for concise and expressive code when checking
     * if any element of a range satisfies a given predicate.
     */
    constexpr AnyOfInvoker any_of_invoker;

    /**
     * @brief A callable structure that evaluates if any element in a range satisfies a condition using a constant
     * predicate binding.
     *
     * The AnyOfConstInvoker struct provides an overload for the function-call operator `operator()`.
     * It operates on ranges and ensures that the predicate is bound as a constant to be used for evaluation.
     * The function checks if any element of the provided range satisfies the condition defined
     * by invoking the specified predicate with the given arguments.
     *
     * @tparam R A type satisfying the std::ranges::input_range concept.
     * @tparam A A parameter pack of additional argument types to be forwarded to the predicate.
     *
     * @requirement The passed arguments must satisfy the std::invocable constraint for the provided
     * predicate when bound using the `ConstBindingType`, matching the range's common reference type.
     *
     * @remarks The function dispatches to `any_of` and efficiently uses perfect forwarding for
     * the range and additional arguments, ensuring constant binding of the predicate.
     *
     * @return A boolean value indicating whether any element in the range satisfies the predicate.
     */
    template <auto Functor>
        requires HasFunctionCallOperator<decltype(Functor)>
    struct AnyOfConstInvoker {
        /**
         * @brief Invokes a predicate on a range to determine if any element satisfies the condition.
         *
         * This function-call operator checks whether at least one element in the given range meets the criteria
         * defined by the specified predicate. The predicate is invoked with the provided range and additional
         * arguments.
         *
         * @tparam R A type satisfying the std::ranges::input_range concept.
         * @tparam A A parameter pack of additional argument types to be forwarded to the predicate.
         *
         * @param range The range of elements to be evaluated.
         * @param args Additional arguments to be forwarded to the predicate along with the elements of the range.
         *
         * @return A boolean value indicating whether any element in the range satisfies the predicate.
         */
        template <std::ranges::input_range R, typename... A>
            requires std::invocable<ConstBindingType<Functor, A...>, RangeCommonReference<R>>
        constexpr bool operator()(R &&range, A &&...args) const {
            return any_of<Functor>(std::forward<R>(range), std::forward<A>(args)...);
        }
    };

    /**
     * @brief A constexpr instance that invokes a callable object to check if any elements in a range satisfy a
     * condition.
     *
     * The `any_of_const_invoker` is a specialization of `AnyOfConstInvoker` for a specific functor.
     * It provides a constant, callable object that operates on constant ranges to evaluate if any element
     * satisfies a given predicate.
     *
     * @tparam Functor The type of the callable object used to evaluate the condition on elements in the range.
     *
     * @requirement The provided `Functor` must be invocable with elements of the range, and it must meet the
     * constraints of the `AnyOfConstInvoker` implementation to work with constant ranges.
     *
     * @remarks Ideal for use cases involving compile-time or contextually constant evaluations where the predicate
     * is applied to immutable range elements.
     *
     * @return A boolean value indicating whether any element in the constant range satisfies the condition.
     */
    template <auto Functor>
        requires HasFunctionCallOperator<decltype(Functor)>
    constexpr AnyOfConstInvoker<Functor> any_of_const_invoker;

    /**
     * @brief Determines if any element in a range satisfies a given predicate.
     *
     * This function serves as a wrapper around the `any_of_invoker` and provides a generic
     * mechanism to evaluate a predicate across a range. The function forwards the provided
     * arguments to construct the invoker with perfect forwarding.
     *
     * @tparam A A parameter pack representing the types of the arguments to forward.
     *
     * @param args The arguments to be forwarded to the `any_of_invoker`. These typically
     * include the range and the predicate, along with any additional parameters necessary
     * for the predicate.
     *
     * @return The result returned by the `any_of_invoker`, which is a boolean value indicating
     * whether any element in the range satisfies the provided predicate.
     */
    RETROLIB_EXPORT template <typename... A>
    constexpr auto any_of(A &&...args) {
        return extension_method<any_of_invoker>(std::forward<A>(args)...);
    }

    /**
     * @brief A constexpr utility to evaluate if any element within a range satisfies a given condition.
     *
     * This function template facilitates the invocation of the `any_of` algorithm by forwarding
     * arguments to an extension method utilizing a specified invoker. The invoker encapsulates
     * the logic of the `any_of` operation and applies the provided functor to the range or elements
     * passed as arguments.
     *
     * @tparam A A parameter pack representing the types of the arguments to be forwarded to the invoker.
     *
     * @param args A variadic pack of arguments to configure the operation. These include the range to be evaluated
     * and any additional arguments bound to the functor (predicate).
     *
     * @return The result of the `any_of` operation, determined by evaluating the condition across
     * the elements of the range. The actual return type is dependent on the invoker and the arguments provided.
     */
    RETROLIB_EXPORT template <auto Functor, typename... A>
        requires HasFunctionCallOperator<decltype(Functor)>
    constexpr auto any_of(A &&...args) {
        return extension_method<any_of_const_invoker<Functor>>(std::forward<A>(args)...);
    }
} // namespace retro::ranges