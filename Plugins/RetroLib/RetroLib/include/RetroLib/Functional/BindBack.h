/**
 * @file BindBack.h
 * @brief Contains the definitions for the bind_back method, which exists in the STL in C++23, but is needed here as
 * well.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/Concepts/ParameterPacks.h"
#include "RetroLib/FunctionTraits.h"
#include "RetroLib/Utils/ForwardLike.h"

#include <tuple>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace retro {

    /**
     * @brief A structure that binds additional arguments to a functor and provides
     * invocable operators to invoke the functor with pre-bound and new arguments.
     *
     * This template struct allows binding of arguments to the back of the argument
     * list of a given functor. It checks at compile-time that the provided functor
     * is valid and constructs an invocable object that accepts additional call-time
     * arguments.
     *
     * @tparam Functor The functor to be invoked. This can be a function pointer,
     * member function pointer, or a callable object.
     * @tparam A Types of the arguments to be bound to the functor.
     *
     * @require `is_valid_functor_object(Functor)` ensures that the functor is a valid callable entity.
     *
     * The class provides several operator() overloads to allow invocation with
     * different const correctness and value category of `args`, reflecting the
     * logic based on left-value, const-left-value, and right-value contexts.
     */
    template <auto Functor, typename... A>
        requires(is_valid_functor_object(Functor))
    struct BindBackConstInvoker {
        using F = decltype(Functor);
        using ArgsTuple = std::tuple<A...>;

        /**
         * @brief Constructs a BindBackConstInvoker object with a set of arguments.
         *
         * This constructor initializes a BindBackConstInvoker instance by forwarding the
         * provided arguments into a tuple (ArgsTuple) that will be used to store them.
         * The constructor employs several constraints to ensure that the arguments are
         * compatible with the intended use of the BindBackConstInvoker.
         *
         * @tparam T The types of the arguments to be stored in the ArgsTuple.
         * @param args The arguments to be forwarded and stored within the instance.
         *
         * @require The parameter pack T must be constructible into an ArgsTuple. The
         *          types of T must not match the BindBackConstInvoker itself to avoid
         *          recursive template instantiation issues.
         */
        template <typename... T>
            requires std::constructible_from<ArgsTuple, T...> && (!PackSameAs<BindBackConstInvoker, T...>)
        constexpr explicit BindBackConstInvoker(T &&...args) : args(std::forward<T>(args)...) {
        }

        /**
         * @brief Invokes the stored functor with the specified call-time arguments
         *        followed by the pre-bound arguments.
         *
         * This invocable operator enables the BindBackConstInvoker to be called with
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
        constexpr decltype(auto) operator()(T &&...call_args) & noexcept(std::is_nothrow_invocable_v<F, T..., A &...>) {
            return std::apply(
                [&]<typename... U>(U &&...final_args) -> decltype(auto) {
                    return std::invoke(Functor, std::forward<T>(call_args)..., std::forward<U>(final_args)...);
                },
                args);
        }

        /**
         * @brief Invokes the stored functor with the specified call-time arguments
         *        followed by the pre-bound arguments.
         *
         * This invocable operator enables the BindBackConstInvoker to be called with
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
        operator()(T &&...call_args) const & noexcept(std::is_nothrow_invocable_v<F, T..., const A &...>) {
            return std::apply(
                [&]<typename... U>(U &&...final_args) -> decltype(auto) {
                    return std::invoke(Functor, std::forward<T>(call_args)..., std::forward<U>(final_args)...);
                },
                args);
        }

        /**
         * @brief Invokes the stored functor with the specified call-time arguments
         *        followed by the pre-bound arguments.
         *
         * This invocable operator enables the BindBackConstInvoker to be called with
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
        constexpr decltype(auto) operator()(T &&...call_args) && noexcept(std::is_nothrow_invocable_v<F, T..., A...>) {
            return std::apply(
                [&]<typename... U>(U &&...final_args) -> decltype(auto) {
                    return std::invoke(Functor, std::forward<T>(call_args)..., std::forward<U>(final_args)...);
                },
                std::move(args));
        }

      private:
        ArgsTuple args;
    };

    /**
     * @class BindBackConstInvoker
     *
     * @brief A template structure that enables partial application of a functor by
     *        binding one of its arguments, allowing subsequent invocations with
     *        additional arguments.
     *
     * This class provides a mechanism to bind the last argument of a functor
     * at construction time, enabling the resulting callable object to be
     * invoked with the remaining leading arguments. It supports multiple
     * invocation overloads to handle different const and value qualifiers.
     *
     * @tparam Functor The type of the functor to be wrapped and invoked.
     * @tparam A The type of the argument to be bound to the functor.
     *
     * @requires The functor must satisfy the `is_valid_functor_object` requirement
     *           to ensure it can be invoked safely and correctly.
     */
    template <auto Functor, typename A>
        requires(is_valid_functor_object(Functor))
    struct BindBackConstInvoker<Functor, A> {
        using F = decltype(Functor);

        /**
         * @brief Constructs a BindBackConstInvoker by binding a specified argument to the functor.
         *
         * This constructor enables creating an instance of the BindBackConstInvoker by
         * forwarding an argument that will be bound to the functor. The argument is stored
         * within the invoker and applied during function invocation.
         *
         * @tparam T The type of the argument to bind, which must be convertible to the type A.
         *
         * @param arg The argument that will be bound to the last position of the functor's
         * invocation signature.
         *
         * @requires The template parameter T must be convertible to A, and T must not decay
         * to the same type as A to prevent ambiguity in binding.
         */
        template <typename T>
            requires std::convertible_to<A, T> && (!std::same_as<std::decay_t<T>, BindBackConstInvoker>)
        constexpr explicit BindBackConstInvoker(T &&arg) : arg(std::forward<T>(arg)) {
        }

        /**
         * @brief Invokes the stored functor with the specified call-time arguments
         *        followed by the pre-bound arguments.
         *
         * This invocable operator enables the BindBackConstInvoker to be called with
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
        constexpr decltype(auto) operator()(T &&...call_args) & noexcept(std::is_nothrow_invocable_v<F, T..., A &>) {
            return std::invoke(Functor, std::forward<T>(call_args)..., arg);
        }

        /**
         * @brief Invokes the stored functor with the specified call-time arguments
         *        followed by the pre-bound arguments.
         *
         * This invocable operator enables the BindBackConstInvoker to be called with
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
        operator()(T &&...call_args) const & noexcept(std::is_nothrow_invocable_v<F, T..., const A &>) {
            return std::invoke(Functor, std::forward<T>(call_args)..., arg);
        }

        /**
         * @brief Invokes the stored functor with the specified call-time arguments
         *        followed by the pre-bound arguments.
         *
         * This invocable operator enables the BindBackConstInvoker to be called with
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
        constexpr decltype(auto) operator()(T &&...call_args) && noexcept(std::is_nothrow_invocable_v<F, T..., A>) {
            return std::invoke(Functor, std::forward<T>(call_args)..., std::move(arg));
        }

      private:
        A arg;
    };

    /**
     * @brief A struct that binds the specified arguments at the back of the functor's argument list and provides
     * multiple invocable interfaces.
     *
     * The BindBackConstInvoker is a utility designed to store a functor along
     * with two arguments that are appended to any additional arguments upon
     * invocation. This allows for versatile, pre-configured functor invocations
     * by effectively binding trailing arguments and enabling various const and
     * mutable invocation scenarios.
     *
     * @tparam Functor The functor object type to be invoked, which must fulfill
     *         the is_valid_functor_object requirements.
     * @tparam A The type of the first argument to bind.
     * @tparam B The type of the second argument to bind.
     */
    template <auto Functor, typename A, typename B>
        requires(is_valid_functor_object(Functor))
    struct BindBackConstInvoker<Functor, A, B> {
        using F = decltype(Functor);

        /**
         * @brief Constructs a BindBackConstInvoker with arguments to be bound.
         *
         * This constructor initializes a BindBackConstInvoker by storing
         * the provided arguments for later use during call-time invocation.
         * The arguments are bound to the end of the argument list of the functor
         * to be invoked.
         *
         * @tparam T Type of the first argument to bind, which must be convertible to type A.
         * @tparam U Type of the second argument to bind.
         * @param arg1 The first argument to be bound.
         * @param arg2 The second argument to be bound.
         */
        template <typename T, typename U>
            requires std::convertible_to<A, T>
        constexpr BindBackConstInvoker(T &&arg1, U &&arg2) : arg1(std::forward<T>(arg1)), arg2(std::forward<U>(arg2)) {
        }

        /**
         * @brief Invokes the stored functor with the specified call-time arguments
         *        followed by the pre-bound arguments.
         *
         * This invocable operator enables the BindBackConstInvoker to be called with
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
        operator()(T &&...call_args) & noexcept(std::is_nothrow_invocable_v<F, T..., A &, B &>) {
            return std::invoke(Functor, std::forward<T>(call_args)..., arg1, arg2);
        }

        /**
         * @brief Invokes the stored functor with the specified call-time arguments
         *        followed by the pre-bound arguments.
         *
         * This invocable operator enables the BindBackConstInvoker to be called with
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
        operator()(T &&...call_args) const & noexcept(std::is_nothrow_invocable_v<F, T..., const A &, const B &>) {
            return std::invoke(Functor, std::forward<T>(call_args)..., arg1, arg2);
        }

        /**
         * @brief Invokes the stored functor with the specified call-time arguments
         *        followed by the pre-bound arguments.
         *
         * This invocable operator enables the BindBackConstInvoker to be called with
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
        constexpr decltype(auto) operator()(T &&...call_args) && noexcept(std::is_nothrow_invocable_v<F, T..., A, B>) {
            return std::invoke(Functor, std::forward<T>(call_args)..., std::move(arg1), std::move(arg2));
        }

      private:
        A arg1;
        B arg2;
    };

    /**
     * @brief Binds arguments to the back of a functor for future invocation.
     *
     * This function template returns a callable object that, when invoked, calls
     * the provided `Functor` object with the stored arguments appended after any
     * new arguments provided at invocation time. It ensures that `Functor` is a
     * valid callable object by using the `is_valid_functor_object` constraint.
     *
     * @tparam Functor The callable type to which the arguments will be bound.
     *                 It must fulfill the `is_valid_functor_object` constraint.
     * @tparam A Parameter pack representing the types of arguments to be bound
     *           to the functor.
     *
     * @param args Arguments to bind to the back of the functor invocation.
     *
     * @return A BindBackConstInvoker instance configured with the deferred
     *         invocation logic.
     */
    RETROLIB_EXPORT template <auto Functor, typename... A>
        requires(is_valid_functor_object(Functor))
    constexpr auto bind_back(A &&...args) {
        return BindBackConstInvoker<Functor, std::decay_t<A>...>(std::forward<A>(args)...);
    }

} // namespace retro