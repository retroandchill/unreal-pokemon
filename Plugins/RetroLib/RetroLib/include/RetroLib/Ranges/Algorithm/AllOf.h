/**
 * @file AllOf.h
 * @brief Functional binding helper for the all_of range operation.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */

#pragma once

#if !RETROLIB_WITH_MODULES
#include "Retrolib/Functional/CreateBinding.h"
#include "Retrolib/Functional/ExtensionMethods.h"
#include "Retrolib/Ranges/FeatureBridge.h"

#include <ranges>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace retro::ranges {
    /**
     * Checks if all elements in the given range satisfy the condition defined
     * by the specified arguments.
     *
     * Utilizes std::ranges::all_of to evaluate the condition on all elements
     * in the range after applying create_binding with the provided arguments.
     *
     * @param range The range of elements to check for the condition.
     *              It can be any range that satisfies the standard range concept.
     * @param args The arguments used to create the predicate binding applied to the elements in the range.
     *             These arguments are forwarded to the create_binding function.
     * @return true if the condition is satisfied for all elements in the range, false otherwise.
     */
    RETROLIB_EXPORT template <std::ranges::input_range R, typename... A>
        requires std::invocable<BindingType<A...>, RangeCommonReference<R>>
    constexpr bool all_of(R &&range, A &&...args) {
        return std::ranges::all_of(std::forward<R>(range), create_binding(std::forward<A>(args)...));
    }

    /**
     * Checks whether all elements in the given range satisfy the specified condition.
     *
     * @param range The input range to be checked.
     * @param args Additional arguments utilized to bind to the condition specified by Functor.
     *
     * @return true if all elements in the range satisfy the condition, otherwise false.
     */
    RETROLIB_EXPORT template <auto Functor, std::ranges::input_range R, typename... A>
        requires HasFunctionCallOperator<decltype(Functor)> &&
                 std::invocable<ConstBindingType<Functor, A...>, RangeCommonReference<R>>
    constexpr bool all_of(R &&range, A &&...args) {
        return std::ranges::all_of(std::forward<R>(range), create_binding<Functor>(std::forward<A>(args)...));
    }

    /**
     * @brief A functional object for checking if all elements of a range satisfy a predicate.
     *
     * The AllOfInvoker struct provides an operator() that applies a predicate or set of arguments to all
     * elements of a given range and returns true if the predicate holds for all elements. Otherwise, it returns false.
     *
     * @tparam R The input range type that satisfies std::ranges::input_range.
     * @tparam A The parameter pack of argument types passed to the predicate function.
     *
     * @requires The predicate function, constructed using BindingType<A...>, must be invocable with the common
     * reference type of the range elements (RangeCommonReference<R>).
     */
    struct AllOfInvoker {
        /**
         * @brief Invokes a predicate on all elements of a range to check if the condition holds for all.
         *
         * This operator applies the given arguments to a predicate over all elements of the specified range
         * and returns true if the predicate is satisfied for every element. Otherwise, it returns false.
         *
         * @param range The input range to be evaluated. Must satisfy std::ranges::input_range requirements.
         * @param args The arguments to be forwarded to the predicate function.
         *
         * @return true if the predicate holds for all elements in the range; otherwise, false.
         *
         * @requires The predicate function must be invocable with the common reference type of the range
         * elements after forwarding the provided arguments.
         */
        template <std::ranges::input_range R, typename... A>
            requires std::invocable<BindingType<A...>, RangeCommonReference<R>>
        constexpr bool operator()(R &&range, A &&...args) const {
            return all_of(std::forward<R>(range), std::forward<A>(args)...);
        }
    };

    /**
     * @brief A constant instance of the AllOfInvoker functional object.
     *
     * The all_of_invoker provides a predefined, constexpr instance of the AllOfInvoker,
     * enabling checks to determine if all elements within a range satisfy a specific predicate.
     *
     * @see AllOfInvoker
     */
    constexpr AllOfInvoker all_of_invoker;

    /**
     * @brief A functional object for validating if all elements of a range satisfy a predicate under const binding.
     *
     * The AllOfConstInvoker struct provides a const version of an operator() that evaluates whether a predicate,
     * constructed with ConstBindingType and a set of arguments, holds true for every element in a given range.
     * It returns true if the condition is met for all elements in the range; otherwise, it returns false.
     *
     * @tparam R The input range type that satisfies std::ranges::input_range.
     * @tparam A The parameter pack of argument types passed to the predicate function.
     *
     * @requires The predicate function, constructed using ConstBindingType<A...>, must be invocable with the
     * common reference type of the range elements (RangeCommonReference<R>).
     */
    template <auto Functor>
        requires HasFunctionCallOperator<decltype(Functor)>
    struct AllOfConstInvoker {
        /**
         * @brief Invokes the all_of algorithm on a given range with specified arguments.
         *
         * This function call operator applies the all_of algorithm to the provided range and arguments,
         * checking if a predicate holds true for all elements in the range. It relies on the Functor
         * to perform the actual predicate evaluation.
         *
         * @tparam R The type of the input range, which must satisfy the std::ranges::input_range concept.
         * @tparam A Parameter pack representing the argument types used by the predicate function.
         *
         * @param range The input range over which the predicate is to be evaluated.
         * @param args The arguments passed to the predicate function.
         *
         * @return true if the predicate is satisfied for all elements in the range; otherwise, false.
         *
         * @requires The Functor must be invocable with the elements of the input range and the specified arguments.
         */
        template <std::ranges::input_range R, typename... A>
            requires std::invocable<ConstBindingType<Functor, A...>, RangeCommonReference<R>>
        constexpr bool operator()(R &&range, A &&...args) const {
            return all_of<Functor>(std::forward<R>(range), std::forward<A>(args)...);
        }
    };

    /**
     * @brief An instance of a functional object for evaluating if all elements of a constant range satisfy a given
     * predicate.
     *
     * The `all_of_const_invoker` is a constexpr object that invokes a provided functor on each element of a constant
     * range and returns true if the functor yields true for all elements. Otherwise, it returns false.
     *
     * @tparam Functor The type of the predicate functor that will be applied to the elements of the constant range.
     *
     * @requires The `Functor` must be invocable with the constant reference type of the elements in the range.
     */
    template <auto Functor>
        requires HasFunctionCallOperator<decltype(Functor)>
    constexpr AllOfConstInvoker<Functor> all_of_const_invoker;

    /**
     * @brief Creates an invoker to check if all elements of a range satisfy a given condition.
     *
     * The all_of function constructs an instance of the all_of_invoker using the provided arguments.
     * This invoker can be applied to a range to determine if a predicate holds true for all elements in the range.
     *
     * @tparam A Variadic template parameter representing the types of arguments passed to the invoker.
     * @param args The arguments to be forwarded as parameters to the all_of_invoker.
     *
     * @return A callable object for the all_of operation, ready to be applied to a range.
     */
    RETROLIB_EXPORT template <typename... A>
    constexpr auto all_of(A &&...args) {
        return extension_method<all_of_invoker>(std::forward<A>(args)...);
    }

    /**
     * @brief Constructs an extension method to evaluate if the predicate holds true for all elements.
     *
     * The `all_of` function creates an extension method using `all_of_const_invoker` that applies
     * the given arguments to a callable object for verification on a range of elements. It returns
     * whether all elements satisfy the given predicate.
     *
     * @tparam A The parameter pack of argument types passed to the `extension_method` and ultimately to the predicate
     * function.
     *
     * @param args A variadic pack of arguments forwarded to the predicate or callable object within the
     * `extension_method`.
     *
     * @return An extension method constructed with `all_of_const_invoker`, which can be used to check conditions on a
     * range.
     */
    RETROLIB_EXPORT template <auto Functor, typename... A>
        requires HasFunctionCallOperator<decltype(Functor)>
    constexpr auto all_of(A &&...args) {
        return extension_method<all_of_const_invoker<Functor>>(std::forward<A>(args)...);
    }
} // namespace retro::ranges