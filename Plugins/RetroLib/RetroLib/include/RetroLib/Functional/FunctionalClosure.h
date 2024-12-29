/**
 * @file FunctionalClosure.h
 * @brief A specialized version of an extension method that handles the binding of functors.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */

#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/Functional/CreateBinding.h"
#include "RetroLib/Functional/ExtensionMethods.h"
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace retro {

    /**
     * @class FunctorBindingInvoker
     * @brief A utility class that handles the invocation of functor-based bindings.
     *
     * This class is designed to wrap and invoke callable objects (functors) that are
     * dynamically bound to it. The FunctorBindingInvoker facilitates the dynamic execution
     * of these callables, enabling flexible and reusable coding paradigms.
     *
     * The primary purpose of this class is to decouple the actual callable from its execution
     * logic, allowing for cleaner and more maintainable code. It can be used in scenarios
     * where a generic invocation mechanism is required.
     *
     * Responsibilities:
     * - Encapsulation of a callable object (functor).
     * - Providing a uniform interface for invoking the encapsulated callable.
     * - Supporting the execution of arbitrary callables that conform to expected interfaces.
     *
     * Thread Safety:
     * - It is the user's responsibility to ensure appropriate thread safety when using
     *   instances of FunctorBindingInvoker in a multithreaded context.
     *
     * Exception Safety:
     * - Ensure that the callable object does not throw exceptions unintentionally. If exceptions
     *   are thrown, the caller must handle them to avoid unexpected behavior.
     *
     * Usage:
     * - Bind a callable object or function to an instance of FunctorBindingInvoker.
     * - Invoke the encapsulated callable using the class's interface.
     *
     * Note:
     * - FunctorBindingInvoker does not own the callable object by default, and the lifetime
     *   of the callable should be managed by the user as necessary.
     */
    RETROLIB_EXPORT template <auto BoundFunctor, auto BaseFunctor>
        requires(ValidFunctorParameter<BoundFunctor> && is_valid_functor_object(BaseFunctor))
    struct FunctorBindingInvoker {
        using BoundFunctorType = decltype(BoundFunctor);
        using BaseFunctorType = decltype(BaseFunctor);

        /**
         * Overloads the function-call operator to invoke the provided functor with
         * the given operand. Uses `std::invoke` to call the functor, forwarding the
         * operand and the functor as arguments.
         *
         * @param operand The input value or argument that will be passed to the functor.
         *                The operand is forwarded, preserving its value category.
         * @param functor The callable object that will be invoked using the operand.
         *                The functor is forwarded, preserving its value category.
         * @return The result of invoking the functor with the given operand.
         *         The return type depends on the functor's return type.
         */
        template <typename T, typename F>
            requires std::invocable<BaseFunctorType, T, BindingType<F>> &&
                     DynamicFunctorBinding<BoundFunctor>
        constexpr decltype(auto) operator()(T &&operand, F &&functor) const {
            return std::invoke(BaseFunctor, std::forward<T>(operand), create_binding(std::forward<F>(functor)));
        }

        /**
         * Calls the functor operator by invoking the BaseFunctor with the provided operand
         * and the result of the create_binding function applied to the provided arguments.
         *
         * @tparam T The type of the operand to be passed to the functor.
         * @tparam A The type(s) of additional arguments to be passed to the binding creation function.
         * @param operand The main operand passed to the BaseFunctor.
         * @param args Additional arguments to create a binding using create_binding.
         * @return The result of invoking BaseFunctor with the operand and the created binding.
         */
        template <typename T, typename... A>
            requires(sizeof...(A) > 1) && std::invocable<BaseFunctorType, T, BindingType<A...>> &&
                    DynamicFunctorBinding<BoundFunctor>
        constexpr decltype(auto) operator()(T &&operand, A &&...args) const {
            return std::invoke(BaseFunctor, std::forward<T>(operand), create_binding(std::forward<A>(args)...));
        }

        /**
         * Invokes the BaseFunctor with the provided operand and a bound version of BoundFunctor.
         *
         * @tparam T The type of the operand passed to the BaseFunctor.
         * @param operand The input parameter to be forwarded to the BaseFunctor.
         * @return The result of invoking BaseFunctor with the provided operand and the bound BoundFunctor.
         */
        template <typename T>
            requires std::invocable<BaseFunctorType, T, BoundFunctorType> && (!DynamicFunctorBinding<BoundFunctor>)
        constexpr decltype(auto) operator()(T &&operand) const {
            return std::invoke(BaseFunctor, std::forward<T>(operand), create_binding<BoundFunctor>());
        }

        /**
         * Overloaded function call operator that invokes the BaseFunctor with the provided operand
         * and arguments after binding them with the BoundFunctor.
         *
         * @tparam T The type of the operand to be passed to the BaseFunctor.
         * @tparam A Variadic template parameter representing the types of additional arguments used
         *           for creating the binding.
         * @param operand The main input to be passed to the BaseFunctor.
         * @param args Additional arguments used to create a binding with the BoundFunctor.
         * @return The result of invoking the BaseFunctor with the given operand and bound arguments.
         */
        template <typename T, typename... A>
            requires(sizeof...(A) >= 1) && std::invocable<BaseFunctorType, T, BindingType<BoundFunctorType, A...>> &&
                    (!DynamicFunctorBinding<BoundFunctor>)
        constexpr decltype(auto) operator()(T &&operand, A &&...args) const {
            return std::invoke(BaseFunctor, std::forward<T>(operand),
                               create_binding<BoundFunctor>(std::forward<A>(args)...));
        }
    };
} // namespace retro