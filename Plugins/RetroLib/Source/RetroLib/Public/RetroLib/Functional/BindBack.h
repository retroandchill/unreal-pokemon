/**
 * @file BindBack.h
 * @brief Contains the definitions for the bind_back method, which exists in the STL in C++23, but is needed here as
 * well.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#include "RetroLib/Concepts/ParameterPacks.h"
#include "RetroLib/FunctionTraits.h"

#if !RETROLIB_WITH_MODULES
#include <tuple>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro {

    /**
     * @brief A utility class that facilitates binding additional arguments to the end of a callable's argument list.
     *
     * This class allows deferring the invocation of a callable while pre-binding a set of arguments that will
     * always be appended to the argument list provided at the time of invocation. It supports perfect forwarding
     * of both the callable and the arguments and ensures proper handling of const, lvalue, and rvalue semantics.
     *
     * @tparam F The callable type to be wrapped.
     * @tparam A Types of the arguments to be pre-bound to the callable.
     */
    template <HasFunctionCallOperator F, typename... A>
    struct TBindBackInvoker {
        using ArgsTuple = std::tuple<A...>;

        /**
         * Constructs a BindBackInvoker instance by forwarding the given functor
         * and arguments to initialize the internal storage for later invocation.
         *
         * @param Functor The callable object or functor to store internally.
         * @param Args The arguments to bind to the functor, forwarded as rvalue references.
         */
        template <typename G, typename... T>
            requires std::constructible_from<F, G> && std::constructible_from<ArgsTuple, T...> &&
                         (!std::same_as<TBindBackInvoker, std::decay_t<G>>)
        constexpr explicit TBindBackInvoker(G &&Functor, T &&...Args)
            : Functor(std::forward<G>(Functor)), Args(std::forward<T>(Args)...) {
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
         * @param CallArgs The arguments provided during the invocation.
         * @return The result of invoking the functor with both pre-bound and additional arguments.
         *
         * @noexcept Conditional noexcept specification based on the nothrow
         *          invocability of the functor with the provided and bound arguments.
         */
        template <typename... T>
            requires std::invocable<F &, T..., A &...>
        constexpr decltype(auto)
        operator()(T &&...CallArgs) & noexcept(std::is_nothrow_invocable_v<F &, T..., A &...>) {
            return std::apply(
                [&]<typename... U>(U &&...FinalArgs) -> decltype(auto) {
                    return std::invoke(Functor, std::forward<T>(CallArgs)..., std::forward<U>(FinalArgs)...);
                },
                Args);
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
         * @param CallArgs The arguments provided during the invocation.
         * @return The result of invoking the functor with both pre-bound and additional arguments.
         *
         * @noexcept Conditional noexcept specification based on the nothrow
         *          invocability of the functor with the provided and bound arguments.
         */
        template <typename... T>
            requires std::invocable<const F &, T..., const A &...>
        constexpr decltype(auto)
        operator()(T &&...CallArgs) const & noexcept(std::is_nothrow_invocable_v<const F &, T..., const A &...>) {
            return std::apply(
                [&]<typename... U>(U &&...FinalArgs) -> decltype(auto) {
                    return std::invoke(Functor, std::forward<T>(CallArgs)..., std::forward<U>(FinalArgs)...);
                },
                Args);
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
         * @param CallArgs The arguments provided during the invocation.
         * @return The result of invoking the functor with both pre-bound and additional arguments.
         *
         * @noexcept Conditional noexcept specification based on the nothrow
         *          invocability of the functor with the provided and bound arguments.
         */
        template <typename... T>
            requires std::invocable<F, T..., A...>
        constexpr decltype(auto) operator()(T &&...CallArgs) && noexcept(std::is_nothrow_invocable_v<F, T..., A...>) {
            return std::apply(
                [&]<typename... U>(U &&...FinalArgs) -> decltype(auto) {
                    return std::invoke(std::move(Functor), std::forward<T>(CallArgs)...,
                                       std::forward<U>(FinalArgs)...);
                },
                std::move(Args));
        }

      private:
        F Functor;
        ArgsTuple Args;
    };

    /**
     * @brief A struct that enables partial application of a function-like object,
     *        binding an argument to its last parameter position.
     *
     * This utility creates a callable object that binds a specific argument to
     * the last parameter of the provided functor, enabling future invocations with
     * additional arguments. The BindBackInvoker ensures proper forwarding of arguments
     * and handles exception specifications for safe, efficient invocation.
     *
     * @tparam F The type of the callable object or functor to be stored and invoked.
     * @tparam A The type of the argument to be bound to the functor's last parameter.
     */
    template <HasFunctionCallOperator F, typename A>
    struct TBindBackInvoker<F, A> {

        /**
         * @brief Constructs a BindBackInvoker object with a specified functor and argument.
         *
         * This constructor initializes the BindBackInvoker object by forwarding the provided
         * functor and argument to their respective member variables. The use of `constexpr`
         * ensures that the construction process can occur at compile-time if possible.
         *
         * @tparam G The type of the functor to be stored.
         * @tparam T The type of the argument to be bound to the functor.
         * @param Functor The callable object or function to be stored and invoked.
         * @param Arg The argument that will be bound to the functor for invocation.
         */
        template <typename G, typename T>
            requires std::constructible_from<F, G> && std::convertible_to<T, A>
        constexpr TBindBackInvoker(G &&Functor, T &&Arg) : Functor(std::forward<G>(Functor)), Arg(std::forward<T>(Arg)) {
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
         * @param CallArgs The arguments provided during the invocation.
         * @return The result of invoking the functor with both pre-bound and additional arguments.
         *
         * @noexcept Conditional noexcept specification based on the nothrow
         *          invocability of the functor with the provided and bound arguments.
         */
        template <typename... T>
            requires std::invocable<F &, T..., A &>
        constexpr decltype(auto) operator()(T &&...CallArgs) & noexcept(std::is_nothrow_invocable_v<F &, T..., A &>) {
            return std::invoke(Functor, std::forward<T>(CallArgs)..., Arg);
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
         * @param CallArgs The arguments provided during the invocation.
         * @return The result of invoking the functor with both pre-bound and additional arguments.
         *
         * @noexcept Conditional noexcept specification based on the nothrow
         *          invocability of the functor with the provided and bound arguments.
         */
        template <typename... T>
            requires std::invocable<const F &, T..., const A &>
        constexpr decltype(auto)
        operator()(T &&...CallArgs) const & noexcept(std::is_nothrow_invocable_v<const F &, T..., const A &>) {
            return std::invoke(Functor, std::forward<T>(CallArgs)..., Arg);
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
         * @param CallArgs The arguments provided during the invocation.
         * @return The result of invoking the functor with both pre-bound and additional arguments.
         *
         * @noexcept Conditional noexcept specification based on the nothrow
         *          invocability of the functor with the provided and bound arguments.
         */
        template <typename... T>
            requires std::invocable<F, T..., A>
        constexpr decltype(auto) operator()(T &&...CallArgs) && noexcept(std::is_nothrow_invocable_v<F, T..., A>) {
            return std::invoke(std::move(Functor), std::forward<T>(CallArgs)..., std::move(Arg));
        }

      private:
        F Functor;
        A Arg;
    };

    /**
     * @brief A utility structure for binding the last two arguments of a callable to predefined values.
     *
     * This class facilitates delayed invocation of a callable object with some arguments pre-bound
     * at the end of its argument list. It provides an invocable interface that allows additional
     * arguments to be supplied at the time of invocation, while the pre-bound arguments are appended
     * to the input argument list.
     *
     * @tparam F The type of the callable object to be invoked.
     * @tparam A The type of the first pre-bound argument.
     * @tparam B The type of the second pre-bound argument.
     */
    template <HasFunctionCallOperator F, typename A, typename B>
    struct TBindBackInvoker<F, A, B> {
        /**
         * @brief Constructs a BindBackInvoker instance with the provided functor and arguments.
         *
         * @param Functor The callable object to be invoked later. It is perfect-forwarded to preserve its value
         * category.
         * @param Arg1 The first argument to bind to the invoker. It is perfect-forwarded.
         * @param Arg2 The second argument to bind to the invoker. It is perfect-forwarded.
         */
        template <typename G, typename T, typename U>
            requires std::constructible_from<F, G> && std::convertible_to<T, A> && std::convertible_to<U, B>
        constexpr TBindBackInvoker(G &&Functor, T &&Arg1, U &&Arg2)
            : Functor(std::forward<G>(Functor)), Arg1(std::forward<T>(Arg1)), Arg2(std::forward<U>(Arg2)) {
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
         * @param CallArgs The arguments provided during the invocation.
         * @return The result of invoking the functor with both pre-bound and additional arguments.
         *
         * @noexcept Conditional noexcept specification based on the nothrow
         *          invocability of the functor with the provided and bound arguments.
         */
        template <typename... T>
            requires std::invocable<F &, T..., A &, B &>
        constexpr decltype(auto)
        operator()(T &&...CallArgs) & noexcept(std::is_nothrow_invocable_v<F &, T..., A &, B &>) {
            return std::invoke(Functor, std::forward<T>(CallArgs)..., Arg1, Arg2);
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
         * @param CallArgs The arguments provided during the invocation.
         * @return The result of invoking the functor with both pre-bound and additional arguments.
         *
         * @noexcept Conditional noexcept specification based on the nothrow
         *          invocability of the functor with the provided and bound arguments.
         */
        template <typename... T>
            requires std::invocable<const F &, T..., const A &, const B &>
        constexpr decltype(auto) operator()(T &&...CallArgs) const & noexcept(
            std::is_nothrow_invocable_v<const F &, T..., const A &, const B &>) {
            return std::invoke(Functor, std::forward<T>(CallArgs)..., Arg1, Arg2);
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
         * @param CallArgs The arguments provided during the invocation.
         * @return The result of invoking the functor with both pre-bound and additional arguments.
         *
         * @noexcept Conditional noexcept specification based on the nothrow
         *          invocability of the functor with the provided and bound arguments.
         */
        template <typename... T>
            requires std::invocable<F, T..., A, B>
        constexpr decltype(auto) operator()(T &&...CallArgs) && noexcept(std::is_nothrow_invocable_v<F, T..., A, B>) {
            return std::invoke(std::move(Functor), std::forward<T>(CallArgs)..., std::move(Arg1), std::move(Arg2));
        }

      private:
        F Functor;
        A Arg1;
        B Arg2;
    };

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
     * @require `IsValidFunctorObject(Functor)` ensures that the functor is a valid callable entity.
     *
     * The class provides several operator() overloads to allow invocation with
     * different const correctness and value category of `args`, reflecting the
     * logic based on left-value, const-left-value, and right-value contexts.
     */
    template <auto Functor, typename... A>
        requires(IsValidFunctorObject(Functor))
    struct TBindBackConstInvoker {
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
         * @param Args The arguments to be forwarded and stored within the instance.
         *
         * @require The parameter pack T must be constructible into an ArgsTuple. The
         *          types of T must not match the BindBackConstInvoker itself to avoid
         *          recursive template instantiation issues.
         */
        template <typename... T>
            requires std::constructible_from<ArgsTuple, T...> && (!PackSameAs<TBindBackConstInvoker, T...>)
        constexpr explicit TBindBackConstInvoker(T &&...Args) : Args(std::forward<T>(Args)...) {
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
         * @param CallArgs The arguments provided during the invocation.
         * @return The result of invoking the functor with both pre-bound and additional arguments.
         *
         * @noexcept Conditional noexcept specification based on the nothrow
         *          invocability of the functor with the provided and bound arguments.
         */
        template <typename... T>
            requires std::invocable<F, T..., A &...>
        constexpr decltype(auto) operator()(T &&...CallArgs) & noexcept(std::is_nothrow_invocable_v<F, T..., A &...>) {
            return std::apply(
                [&]<typename... U>(U &&...FinalArgs) -> decltype(auto) {
                    return std::invoke(Functor, std::forward<T>(CallArgs)..., std::forward<U>(FinalArgs)...);
                },
                Args);
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
         * @param CallArgs The arguments provided during the invocation.
         * @return The result of invoking the functor with both pre-bound and additional arguments.
         *
         * @noexcept Conditional noexcept specification based on the nothrow
         *          invocability of the functor with the provided and bound arguments.
         */
        template <typename... T>
            requires std::invocable<F, T..., const A &...>
        constexpr decltype(auto)
        operator()(T &&...CallArgs) const & noexcept(std::is_nothrow_invocable_v<F, T..., const A &...>) {
            return std::apply(
                [&]<typename... U>(U &&...FinalArgs) -> decltype(auto) {
                    return std::invoke(Functor, std::forward<T>(CallArgs)..., std::forward<U>(FinalArgs)...);
                },
                Args);
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
         * @param CallArgs The arguments provided during the invocation.
         * @return The result of invoking the functor with both pre-bound and additional arguments.
         *
         * @noexcept Conditional noexcept specification based on the nothrow
         *          invocability of the functor with the provided and bound arguments.
         */
        template <typename... T>
            requires std::invocable<F, T..., A...>
        constexpr decltype(auto) operator()(T &&...CallArgs) && noexcept(std::is_nothrow_invocable_v<F, T..., A...>) {
            return std::apply(
                [&]<typename... U>(U &&...FinalArgs) -> decltype(auto) {
                    return std::invoke(Functor, std::forward<T>(CallArgs)..., std::forward<U>(FinalArgs)...);
                },
                std::move(Args));
        }

      private:
        ArgsTuple Args;
    };

    /**
     * @class TBindBackConstInvoker
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
     * @requires The functor must satisfy the `IsValidFunctorObject` requirement
     *           to ensure it can be invoked safely and correctly.
     */
    template <auto Functor, typename A>
        requires(IsValidFunctorObject(Functor))
    struct TBindBackConstInvoker<Functor, A> {
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
         * @param Arg The argument that will be bound to the last position of the functor's
         * invocation signature.
         *
         * @requires The template parameter T must be convertible to A, and T must not decay
         * to the same type as A to prevent ambiguity in binding.
         */
        template <typename T>
            requires std::convertible_to<T, A> && (!std::same_as<std::decay_t<T>, TBindBackConstInvoker>)
        constexpr explicit TBindBackConstInvoker(T &&Arg) : arg(std::forward<T>(Arg)) {
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
         * @param CallArgs The arguments provided during the invocation.
         * @return The result of invoking the functor with both pre-bound and additional arguments.
         *
         * @noexcept Conditional noexcept specification based on the nothrow
         *          invocability of the functor with the provided and bound arguments.
         */
        template <typename... T>
            requires std::invocable<F, T..., A &>
        constexpr decltype(auto) operator()(T &&...CallArgs) & noexcept(std::is_nothrow_invocable_v<F, T..., A &>) {
            return std::invoke(Functor, std::forward<T>(CallArgs)..., arg);
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
         * @param CallArgs The arguments provided during the invocation.
         * @return The result of invoking the functor with both pre-bound and additional arguments.
         *
         * @noexcept Conditional noexcept specification based on the nothrow
         *          invocability of the functor with the provided and bound arguments.
         */
        template <typename... T>
            requires std::invocable<F, T..., const A &>
        constexpr decltype(auto)
        operator()(T &&...CallArgs) const & noexcept(std::is_nothrow_invocable_v<F, T..., const A &>) {
            return std::invoke(Functor, std::forward<T>(CallArgs)..., arg);
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
         * @param CallArgs The arguments provided during the invocation.
         * @return The result of invoking the functor with both pre-bound and additional arguments.
         *
         * @noexcept Conditional noexcept specification based on the nothrow
         *          invocability of the functor with the provided and bound arguments.
         */
        template <typename... T>
            requires std::invocable<F, T..., A>
        constexpr decltype(auto) operator()(T &&...CallArgs) && noexcept(std::is_nothrow_invocable_v<F, T..., A>) {
            return std::invoke(Functor, std::forward<T>(CallArgs)..., std::move(arg));
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
     *         the IsValidFunctorObject requirements.
     * @tparam A The type of the first argument to bind.
     * @tparam B The type of the second argument to bind.
     */
    template <auto Functor, typename A, typename B>
        requires(IsValidFunctorObject(Functor))
    struct TBindBackConstInvoker<Functor, A, B> {
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
         * @param Arg1 The first argument to be bound.
         * @param Arg2 The second argument to be bound.
         */
        template <typename T, typename U>
            requires std::convertible_to<T, A> && std::convertible_to<U, B>
        constexpr TBindBackConstInvoker(T &&Arg1, U &&Arg2) : Arg1(std::forward<T>(Arg1)), Arg2(std::forward<U>(Arg2)) {
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
         * @param CallArgs The arguments provided during the invocation.
         * @return The result of invoking the functor with both pre-bound and additional arguments.
         *
         * @noexcept Conditional noexcept specification based on the nothrow
         *          invocability of the functor with the provided and bound arguments.
         */
        template <typename... T>
            requires std::invocable<F, T..., A &, B &>
        constexpr decltype(auto)
        operator()(T &&...CallArgs) & noexcept(std::is_nothrow_invocable_v<F, T..., A &, B &>) {
            return std::invoke(Functor, std::forward<T>(CallArgs)..., Arg1, Arg2);
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
         * @param CallArgs The arguments provided during the invocation.
         * @return The result of invoking the functor with both pre-bound and additional arguments.
         *
         * @noexcept Conditional noexcept specification based on the nothrow
         *          invocability of the functor with the provided and bound arguments.
         */
        template <typename... T>
            requires std::invocable<F, T..., const A &, const B &>
        constexpr decltype(auto)
        operator()(T &&...CallArgs) const & noexcept(std::is_nothrow_invocable_v<F, T..., const A &, const B &>) {
            return std::invoke(Functor, std::forward<T>(CallArgs)..., Arg1, Arg2);
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
         * @param CallArgs The arguments provided during the invocation.
         * @return The result of invoking the functor with both pre-bound and additional arguments.
         *
         * @noexcept Conditional noexcept specification based on the nothrow
         *          invocability of the functor with the provided and bound arguments.
         */
        template <typename... T>
            requires std::invocable<F, T..., A, B>
        constexpr decltype(auto) operator()(T &&...CallArgs) && noexcept(std::is_nothrow_invocable_v<F, T..., A, B>) {
            return std::invoke(Functor, std::forward<T>(CallArgs)..., std::move(Arg1), std::move(Arg2));
        }

      private:
        A Arg1;
        B Arg2;
    };

    /**
     * Binds a set of arguments to the back of the provided callable object, creating a new callable object with the
     * bound arguments.
     *
     * @tparam F The type of the callable object.
     * @tparam A The types of the arguments to bind.
     * @param Functor The callable object to bind the arguments to.
     * @param Args The arguments to bind to the callable object.
     * @return A new callable object with the specified arguments bound to the back.
     */
    RETROLIB_EXPORT template <typename F, typename... A>
        requires HasFunctionCallOperator<std::decay_t<F>>
    constexpr auto BindBack(F &&Functor, A &&...Args) {
        return TBindBackInvoker<std::decay_t<F>, std::decay_t<A>...>(std::forward<F>(Functor), std::forward<A>(Args)...);
    }

    /**
     * @brief Binds arguments to the back of a functor for future invocation.
     *
     * This function template returns a callable object that, when invoked, calls
     * the provided `Functor` object with the stored arguments appended after any
     * new arguments provided at invocation time. It ensures that `Functor` is a
     * valid callable object by using the `IsValidFunctorObject` constraint.
     *
     * @tparam Functor The callable type to which the arguments will be bound.
     *                 It must fulfill the `IsValidFunctorObject` constraint.
     * @tparam A Parameter pack representing the types of arguments to be bound
     *           to the functor.
     *
     * @param Args Arguments to bind to the back of the functor invocation.
     *
     * @return A BindBackConstInvoker instance configured with the deferred
     *         invocation logic.
     */
    RETROLIB_EXPORT template <auto Functor, typename... A>
        requires(IsValidFunctorObject(Functor))
    constexpr auto BindBack(A &&...Args) {
        return TBindBackConstInvoker<Functor, std::decay_t<A>...>(std::forward<A>(Args)...);
    }

} // namespace retro