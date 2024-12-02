/**
 * @file BindMethod.h
 * @brief Special functional binding methods for usage with methods.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/Concepts/ParameterPacks.h"
#include "RetroLib/Functional/BindFront.h"
#include "RetroLib/FunctionTraits.h"
#include "RetroLib/Functional/Invoke.h"
#include "RetroLib/Utils/ForwardLike.h"

#include <functional>
#include <tuple>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace retro {
    template <typename T, typename M>
    concept CanBindMethod =
        Method<M> && (std::convertible_to<T, RefQualifiedClassType<M>> ||
                      (Dereferenceable<T> && std::convertible_to<DereferencedType<T>, RefQualifiedClassType<M>>));

    /**
     * @brief A class template for binding a method to an object with pre-specified arguments.
     *
     * @tparam C The type of the class object to which the method is bound.
     * @tparam Functor A compile-time constant that must satisfy the Method concept, representing the member function
     * pointer.
     * @tparam A Variadic template parameters representing the types of the pre-specified arguments.
     *
     * @requires The Functor must satisfy the Method concept.
     *
     * This template class allows binding a member function (specified by Functor) to an instance of a class (specified
     * by C), along with a set of arguments (specified by A...) that will be used each time the method is invoked.
     *
     * The class template supports invocation with additional arguments provided at call time, combined with the
     * pre-specified ones. It ensures the object and arguments match the requirements of the specified member function.
     *
     * @note This class ensures that the object and pre-specified arguments are stored and used in the invocation
     * of the method when called, honoring value categories and noexcept specifications.
     *
     * @tparam T The type of the object passed for construction. Must be convertible to C.
     * @tparam U... Types of the additional arguments passed at construction. Must be constructible into ArgsTuple.
     */
    template <typename C, auto Functor, typename... A>
        requires CanBindMethod<C, decltype(Functor)>
    struct MethodBinding {
        using F = decltype(Functor);
        using ArgsTuple = std::tuple<A...>;

        /**
         * @brief Constructs a MethodBinding object with a specified object and arguments.
         *
         * @tparam T The type of the object being bound. Must be convertible to the type C in the MethodBinding.
         * @tparam U Variadic template arguments representing types for additional arguments passed during construction.
         *
         * @param object The object to which the method is bound.
         * @param args Additional arguments to be bound, used in conjunction with the member function.
         *
         * @requires The constructor requires that the type T is not the same as a decayed MethodBinding type,
         *           ensuring that the object type is convertible to C, and the additional arguments can be
         *           constructed into ArgsTuple.
         */
        template <typename T, typename... U>
            requires std::convertible_to<C, T> && std::constructible_from<ArgsTuple, U...> &&
                         (!std::same_as<std::decay_t<T>, MethodBinding>)
        constexpr explicit MethodBinding(T &&object, U &&...args)
            : object(std::forward<T>(object)), args(std::forward<U>(args)...) {
        }

        /**
         * @brief Invokes the stored functor with the specified call-time arguments
         *        followed by the pre-bound arguments.
         *
         * This invocable operator enables the MethodBinding to be called with
         * additional arguments that are further processed and combined with the
         * pre-bound arguments. It performs the invocation ensuring the combination
         * is safe, considering exception specifications and argument forwarding.
         *
         * @tparam T The types of the additional arguments to be passed at call-time.
         * @param call_args The arguments provided during the invocation.
         * @return The result of invoking the functor with both pre-bound and additional arguments.
         *
         * @noexcept Conditional noexcept specification based on the nothrow
         *          invocability of the functor with the provided and bound arguments.
         */
        template <typename... T>
        constexpr decltype(auto)
        operator()(T &&...call_args) & noexcept(std::is_nothrow_invocable_v<F, C &, T..., A &...>) {
            return std::apply(
                [&]<typename... U>(U &&...final_args) -> decltype(auto) {
                    return invoke<Functor>(object, std::forward<T>(call_args)..., std::forward<U>(final_args)...);
                },
                args);
        }

        /**
         * @brief Invokes the stored functor with the specified call-time arguments
         *        followed by the pre-bound arguments.
         *
         * This invocable operator enables the MethodBinding to be called with
         * additional arguments that are further processed and combined with the
         * pre-bound arguments. It performs the invocation ensuring the combination
         * is safe, considering exception specifications and argument forwarding.
         *
         * @tparam T The types of the additional arguments to be passed at call-time.
         * @param call_args The arguments provided during the invocation.
         * @return The result of invoking the functor with both pre-bound and additional arguments.
         *
         * @noexcept Conditional noexcept specification based on the nothrow
         *          invocability of the functor with the provided and bound arguments.
         */
        template <typename... T>
        constexpr decltype(auto)
        operator()(T &&...call_args) const & noexcept(std::is_nothrow_invocable_v<F, const C &, T..., const A &...>) {
            return std::apply(
                [&]<typename... U>(U &&...final_args) -> decltype(auto) {
                    return invoke<Functor>(object, std::forward<T>(call_args)..., std::forward<U>(final_args)...);
                },
                args);
        }

        /**
         * @brief Invokes the stored functor with the specified call-time arguments
         *        followed by the pre-bound arguments.
         *
         * This invocable operator enables the MethodBinding to be called with
         * additional arguments that are further processed and combined with the
         * pre-bound arguments. It performs the invocation ensuring the combination
         * is safe, considering exception specifications and argument forwarding.
         *
         * @tparam T The types of the additional arguments to be passed at call-time.
         * @param call_args The arguments provided during the invocation.
         * @return The result of invoking the functor with both pre-bound and additional arguments.
         *
         * @noexcept Conditional noexcept specification based on the nothrow
         *          invocability of the functor with the provided and bound arguments.
         */
        template <typename... T>
        constexpr decltype(auto)
        operator()(T &&...call_args) && noexcept(std::is_nothrow_invocable_v<F, C, T..., A...>) {
            return std::apply(
                [&]<typename... U>(U &&...final_args) -> decltype(auto) {
                    return invoke<Functor>(std::move(object), std::forward<T>(call_args)...,
                                       std::forward<U>(final_args)...);
                },
                std::move(args));
        }

      private:
        C object;
        ArgsTuple args;
    };

    /**
     * @brief A utility structure to encapsulate the binding of a specific method
     *        to a class instance with predefined arguments.
     *
     * The MethodBinding struct is designed to wrap a method (Functor) from a class (C)
     * along with a pre-bound argument (A). It is templated to allow flexible usage
     * across various types and ensures that the specified method can be correctly
     * invoked with the given arguments. The struct provides operator() that allows
     * the object to be called as a function, invoking the wrapped method with
     * both pre-defined and additional call-time arguments.
     *
     * @tparam C The type of the class to which the method belongs.
     * @tparam Functor The method to be bound, meeting the Method constraint.
     * @tparam A The type of the pre-bound argument to be used when invoking the method.
     */
    template <typename C, auto Functor, typename A>
        requires CanBindMethod<C, decltype(Functor)>
    struct MethodBinding<C, Functor, A> {
        using F = decltype(Functor);

        /**
         * @brief Constructs a MethodBinding instance, initializing the object and argument
         *        that will be used during method invocation.
         *
         * This constructor ensures that the provided object and argument are suitable types
         * for the method being bound. The arguments are forwarded to ensure they are
         * perfectly captured.
         *
         * @tparam T The type of the object that the method will be invoked on.
         * @tparam U The type of the argument to be pre-bound to the method.
         * @param object An instance of the class containing the method to be bound.
         * @param arg The argument to be used when invoking the method.
         *
         * @note The constructor uses concepts to ensure the provided object and argument
         * types are convertible to the expected types for the class and argument.
         */
        template <typename T, typename U>
            requires std::convertible_to<T, C> && std::convertible_to<U, A>
        constexpr explicit MethodBinding(T &&object, U &&arg)
            : object(std::forward<T>(object)), arg(std::forward<U>(arg)) {
        }

        /**
         * @brief Invokes the stored functor with the specified call-time arguments
         *        followed by the pre-bound arguments.
         *
         * This invocable operator enables the MethodBinding to be called with
         * additional arguments that are further processed and combined with the
         * pre-bound arguments. It performs the invocation ensuring the combination
         * is safe, considering exception specifications and argument forwarding.
         *
         * @tparam T The types of the additional arguments to be passed at call-time.
         * @param call_args The arguments provided during the invocation.
         * @return The result of invoking the functor with both pre-bound and additional arguments.
         *
         * @noexcept Conditional noexcept specification based on the nothrow
         *          invocability of the functor with the provided and bound arguments.
         */
        template <typename... T>
            requires std::invocable<F, C &, T..., A &>
        constexpr decltype(auto)
        operator()(T &&...call_args) & noexcept(std::is_nothrow_invocable_v<F, C &, T..., A &>) {
            return invoke<Functor>(object, std::forward<T>(call_args)..., arg);
        }

        /**
         * @brief Invokes the stored functor with the specified call-time arguments
         *        followed by the pre-bound arguments.
         *
         * This invocable operator enables the MethodBinding to be called with
         * additional arguments that are further processed and combined with the
         * pre-bound arguments. It performs the invocation ensuring the combination
         * is safe, considering exception specifications and argument forwarding.
         *
         * @tparam T The types of the additional arguments to be passed at call-time.
         * @param call_args The arguments provided during the invocation.
         * @return The result of invoking the functor with both pre-bound and additional arguments.
         *
         * @noexcept Conditional noexcept specification based on the nothrow
         *          invocability of the functor with the provided and bound arguments.
         */
        template <typename... T>
            requires std::invocable<F, const C &, T..., const A &>
        constexpr decltype(auto)
        operator()(T &&...call_args) const & noexcept(std::is_nothrow_invocable_v<F, const C &, T..., const A &>) {
            return invoke<Functor>(object, std::forward<T>(call_args)..., arg);
        }

        /**
         * @brief Invokes the stored functor with the specified call-time arguments
         *        followed by the pre-bound arguments.
         *
         * This invocable operator enables the MethodBinding to be called with
         * additional arguments that are further processed and combined with the
         * pre-bound arguments. It performs the invocation ensuring the combination
         * is safe, considering exception specifications and argument forwarding.
         *
         * @tparam T The types of the additional arguments to be passed at call-time.
         * @param call_args The arguments provided during the invocation.
         * @return The result of invoking the functor with both pre-bound and additional arguments.
         *
         * @noexcept Conditional noexcept specification based on the nothrow
         *          invocability of the functor with the provided and bound arguments.
         */
        template <typename... T>
            requires std::invocable<F, C, T..., A>
        constexpr decltype(auto) operator()(T &&...call_args) && noexcept(std::is_nothrow_invocable_v<F, C, T..., A>) {
            return invoke<Functor>(std::move(object), std::forward<T>(call_args)..., std::move(arg));
        }

      private:
        C object;
        A arg;
    };

    /**
     * @brief A template struct that binds a method (Functor) with specified arguments to an object,
     * allowing the method to be invoked later with additional arguments.
     *
     * @tparam C The type of the object to which the method is bound.
     * @tparam Functor The method to be bound to the object.
     * @tparam A The type of the first additional argument to be bound with the method.
     * @tparam B The type of the second additional argument to be bound with the method.
     *
     * This struct provides several operator() overloads to allow invocation of the bound
     * method with additional arguments. The method is invoked with a combination of bound
     * object, bound arguments, and additional arguments passed to operator().
     *
     * @note The Functor must satisfy the Method concept, which ensures it is a valid
     * method binding and is not a free function.
     *
     * @tparam T The types of the additional arguments to be passed when calling the bound method.
     * @returns The result of invoking the Functor with the combined arguments.
     *
     * Three operator() overloads are provided for different value category contexts:
     * - The method may be invoked on an lvalue reference to MethodBinding, forwarding the object and
     *   bound arguments as lvalue references.
     * - The method may be invoked on a const lvalue reference to MethodBinding, treating all
     *   arguments as const lvalue references.
     * - The method may be invoked on an rvalue reference to MethodBinding, forwarding the object and
     *   bound arguments as rvalue references.
     *
     * @remark These invocations are noexcept if the provided Functor is nothrow-invocable in the
     * respective context.
     */
    template <typename C, auto Functor, typename A, typename B>
        requires CanBindMethod<C, decltype(Functor)>
    struct MethodBinding<C, Functor, A, B> {
        using F = decltype(Functor);

        /**
         * @brief Constructs a MethodBinding instance by binding the given object and arguments.
         *
         * This constructor initializes a MethodBinding struct by binding an object and two arguments,
         * allowing a method associated with the object to be called later with additional arguments.
         *
         * @tparam T The type of the object to be bound. Must be convertible to the template parameter C.
         * @tparam U The type of the first argument to be bound. Must be convertible to the template parameter A.
         * @tparam W The type of the second argument to be bound. Must be convertible to the template parameter B.
         *
         * @param object The object to which the method is bound.
         * @param arg1 The first argument to be bound to the method.
         *
         * @note The constructor is explicit and constexpr, ensuring that the binding operation is executed at
         * compile-time for qualified contexts and preventing unintended implicit conversions.
         */
        template <typename T, typename U, typename W>
            requires std::convertible_to<T, C> && std::convertible_to<U, A> && std::convertible_to<W, B>
        constexpr explicit MethodBinding(T &&object, U &&arg1, W &&arg2)
            : object(std::forward<T>(object)), arg1(std::forward<U>(arg1)), arg2(std::forward<W>(arg2)) {
        }

        /**
         * @brief Invokes the stored functor with the specified call-time arguments
         *        followed by the pre-bound arguments.
         *
         * This invocable operator enables the MethodBinding to be called with
         * additional arguments that are further processed and combined with the
         * pre-bound arguments. It performs the invocation ensuring the combination
         * is safe, considering exception specifications and argument forwarding.
         *
         * @tparam T The types of the additional arguments to be passed at call-time.
         * @param call_args The arguments provided during the invocation.
         * @return The result of invoking the functor with both pre-bound and additional arguments.
         *
         * @noexcept Conditional noexcept specification based on the nothrow
         *          invocability of the functor with the provided and bound arguments.
         */
        template <typename... T>
            requires std::invocable<F, C &, T..., A &, B &>
        constexpr decltype(auto)
        operator()(T &&...call_args) & noexcept(std::is_nothrow_invocable_v<F, C &, T..., A &, B &>) {
            return invoke<Functor>(object, std::forward<T>(call_args)..., arg1, arg2);
        }

        /**
         * @brief Invokes the stored functor with the specified call-time arguments
         *        followed by the pre-bound arguments.
         *
         * This invocable operator enables the MethodBinding to be called with
         * additional arguments that are further processed and combined with the
         * pre-bound arguments. It performs the invocation ensuring the combination
         * is safe, considering exception specifications and argument forwarding.
         *
         * @tparam T The types of the additional arguments to be passed at call-time.
         * @param call_args The arguments provided during the invocation.
         * @return The result of invoking the functor with both pre-bound and additional arguments.
         *
         * @noexcept Conditional noexcept specification based on the nothrow
         *          invocability of the functor with the provided and bound arguments.
         */
        template <typename... T>
            requires std::invocable<F, C, T..., const A &, const B &>
        constexpr decltype(auto) operator()(T &&...call_args) const & noexcept(
            std::is_nothrow_invocable_v<F, const C &, T..., const A &, const B &>) {
            return invoke<Functor>(object, std::forward<T>(call_args)..., arg1, arg2);
        }

        /**
         * @brief Invokes the stored functor with the specified call-time arguments
         *        followed by the pre-bound arguments.
         *
         * This invocable operator enables the MethodBinding to be called with
         * additional arguments that are further processed and combined with the
         * pre-bound arguments. It performs the invocation ensuring the combination
         * is safe, considering exception specifications and argument forwarding.
         *
         * @tparam T The types of the additional arguments to be passed at call-time.
         * @param call_args The arguments provided during the invocation.
         * @return The result of invoking the functor with both pre-bound and additional arguments.
         *
         * @noexcept Conditional noexcept specification based on the nothrow
         *          invocability of the functor with the provided and bound arguments.
         */
        template <typename... T>
            requires std::invocable<F, C, T..., A, B>
        constexpr decltype(auto)
        operator()(T &&...call_args) && noexcept(std::is_nothrow_invocable_v<F, C, T..., A, B>) {
            return invoke<Functor>(std::move(object), std::forward<T>(call_args)..., std::move(arg1),
                               std::move(arg2));
        }

      private:
        C object;
        A arg1;
        B arg2;
    };

    /**
     * Binds a method to an object and optional arguments.
     *
     * This function template allows for binding a method referenced by the Functor
     * to a specific object and optionally additional arguments. If no additional
     * arguments are provided, it returns a callable object which, when called,
     * will invoke the method on the object with the provided arguments.
     *
     * The function has two distinct behaviors:
     * 1. If no additional arguments are provided, it uses `std::bind_front` to
     *    bind the method to the specified object, creating a new callable object
     *    with the bound object as its fixed first argument.
     * 2. If additional arguments are provided, it returns an instance of
     *    MethodBinding, which combines the object, method, and additional
     *    arguments into a new callable object optimized for the specified `C`
     *    and `A` types.
     *
     * The method to bind is required to satisfy the `Method` concept, which
     * ensures that the provided Functor is a valid type suitable for binding
     * using this utility.
     *
     * @tparam Functor The type of the member function pointer to bind.
     * @tparam C The type of the object to which the method will be bound.
     * @tparam A A variadic template parameter pack representing the types of
     *           additional arguments to bind to the method.
     *
     * @param object The object instance to bind the method to.
     * @param args Optional additional arguments to the method.
     * @return A callable object that binds the specified method to
     *         the provided object and arguments.
     */
    RETROLIB_EXPORT template <auto Functor, typename C, typename... A>
        requires Method<decltype(Functor)>
    constexpr auto bind_method(C &&object, A &&...args) {
        if constexpr (sizeof...(A) == 0) {
            return bind_front<Functor>(std::forward<C>(object));
        } else {
            return MethodBinding<std::decay_t<C>, Functor, std::decay_t<A>...>(std::forward<C>(object),
                                                                               std::forward<A>(args)...);
        }
    }
} // namespace retro