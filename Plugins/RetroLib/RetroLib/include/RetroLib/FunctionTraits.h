/**
 * @file FunctionTraits.h
 * @brief Contains the basic trait information for functional types.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include <concepts>
#include <type_traits>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace retro {

    /**
     * A traits structure to determine if a given type has exactly one function call operator.
     *
     * This template specialization inherits from `std::false_type` by default,
     * indicating that the specified type does not have a single function call operator.
     *
     * To extend the functionality for a specific type that has exactly one
     * function call operator, you need to provide a template specialization
     * for that type which inherits from `std::true_type`.
     *
     * Template Parameter:
     * - typename: The type to be checked for a single function call operator.
     */
    template <typename>
    struct HasOneFunctionCallOperator : std::false_type {};

    /**
     * @brief Determines if a class has exactly one function call operator.
     *
     * This template struct inherits from std::true_type if the specified
     * type T has exactly one function call operator. It makes use of
     * SFINAE (Substitution Failure Is Not An Error) principle by requiring
     * that T has a member function pointer to operator().
     *
     * @tparam T The class type to be inspected for a function call operator.
     *
     * The check is valid at compile time and is useful in template
     * metaprogramming where knowing the presence of a callable operator
     * can conditionally enable or disable certain code paths.
     */
    template <typename T>
        requires std::is_member_function_pointer_v<decltype(&T::operator())>
    struct HasOneFunctionCallOperator<T> : std::true_type {};

    /**
     * @class WithOp
     *
     * @brief Represents a callable object that can be executed using the function call operator.
     *
     * The WithOp struct provides an override of the function call operator, enabling instances
     * of this struct to be used as callable objects. This is used for utility checking to see if a type has
     * a valid function call operator or not.
     */
    struct WithOp {
        void operator()() const;
    };

    /**
     * @class Mixin
     *
     * @brief A template structure that combines the functionality of two types.
     *
     * The Mixin struct is a template that inherits from the specified type `T` and the
     * `WithOp` struct. This allows the Mixin to extend the capabilities of `T` by
     * adding the callable operator functionality provided by `WithOp`.
     * Ideal for scenarios where a type needs to exhibit both its original
     * behavior and additional operations defined in `WithOp`.
     *
     * This is used for a type trait check to see if a type has a functional callback at all, by checking if the
     * `operator()` overload can be resolved to a function pointer.
     *
     * @tparam T The type to be extended with additional callable operations.
     */
    template <typename T>
    struct Mixin : T, WithOp {};

    /**
     * @brief Concept to determine if a type does not have a single function call operator.
     *
     * This concept evaluates to true if the specified type T, when mixed with a certain `Mixin`,
     * does not possess exactly one function call operator. It leverages a helper named
     * `HasOneFunctionCallOperator` for this evaluation.
     *
     * @tparam T The type to be checked for the function call operator presence.
     */
    RETROLIB_EXPORT template <typename T>
    concept HasFunctionCallOperator =
        std::is_pointer_v<T> || std::is_member_pointer_v<T> || !HasOneFunctionCallOperator<Mixin<T>>::value;

    RETROLIB_EXPORT template <typename F>
    consteval bool is_valid_functor_object(const F &functor) {
        if constexpr (!HasFunctionCallOperator<F>) {
            return false;
        } else if constexpr (std::is_pointer_v<F> || std::is_member_pointer_v<F>) {
            return functor != nullptr;
        } else {
            return true;
        }
    }
} // namespace retro