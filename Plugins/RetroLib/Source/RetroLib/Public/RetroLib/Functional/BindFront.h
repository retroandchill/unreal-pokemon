/**
 * @file BindFront.h
 * @brief Contains the definitions for the constexpr bind_front method, which exists in the STL in C++26, but is needed
 * here as
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
     * @brief A structure that binds a set of arguments (ArgsTuple) to the front of a functor, enabling calls with
     * additional arguments.
     *
     * The BindFrontConstInvoker template structure allows for permanent binding of arguments to the front of a
     * specified functor, enabling invocation with additional arguments at a later point. It leverages template
     * metaprogramming to ensure safe and efficient forwarding and invocation of the combined arguments. This invoker is
     * particularly useful in scenarios that require binding known arguments in advance while preserving the flexibility
     * to specify additional call-time arguments dynamically.
     *
     * @tparam Functor The functor to be invoked with both bound and additional arguments. Must satisfy
     * IsValidFunctorObject.
     * @tparam A Types of the arguments that are to be bound to the front of the Functor.
     */
    template <auto Functor, typename... A>
        requires(IsValidFunctorObject(Functor))
    struct TBindFrontConstInvoker {
        using F = decltype(Functor);
        using ArgsTuple = std::tuple<A...>;

        /**
         * @brief Constructs a BindFrontConstInvoker with bound arguments.
         *
         * This constructor initializes a BindFrontConstInvoker by binding a set of arguments,
         * provided as a parameter pack, to the front of the invoker. The constructor utilizes
         * template metaprogramming to ensure that the types of arguments can construct an
         * ArgsTuple and verifies that the argument pack does not match a specific constraint
         * represented by the PackSameAs concept. It forwards these arguments to construct
         * them efficiently without making unnecessary copies.
         *
         * @tparam T Variadic template parameter pack representing the types of arguments to be bound.
         * @param Args A set of arguments to be bound at construction. They are forwarded to
         *             an ArgsTuple for storage.
         */
        template <typename... T>
            requires std::constructible_from<ArgsTuple, T...> && (!PackSameAs<TBindFrontConstInvoker, T...>)
        constexpr explicit TBindFrontConstInvoker(T &&...Args) : args(std::forward<T>(Args)...) {
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
            requires std::invocable<F, A &..., T...>
        constexpr decltype(auto) operator()(T &&...CallArgs) & noexcept(std::is_nothrow_invocable_v<F, A &..., T...>) {
            return std::apply(
                [&]<typename... U>(U &&...FinalArgs) -> decltype(auto) {
                    return std::invoke(Functor, std::forward<U>(FinalArgs)..., std::forward<T>(CallArgs)...);
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
         * @param CallArgs The arguments provided during the invocation.
         * @return The result of invoking the functor with both pre-bound and additional arguments.
         *
         * @noexcept Conditional noexcept specification based on the nothrow
         *          invocability of the functor with the provided and bound arguments.
         */
        template <typename... T>
            requires std::invocable<F, const A &..., T...>
        constexpr decltype(auto)
        operator()(T &&...CallArgs) const & noexcept(std::is_nothrow_invocable_v<F, const A &..., T...>) {
            return std::apply(
                [&]<typename... U>(U &&...FinalArgs) -> decltype(auto) {
                    return std::invoke(Functor, std::forward<U>(FinalArgs)..., std::forward<T>(CallArgs)...);
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
         * @param CallArgs The arguments provided during the invocation.
         * @return The result of invoking the functor with both pre-bound and additional arguments.
         *
         * @noexcept Conditional noexcept specification based on the nothrow
         *          invocability of the functor with the provided and bound arguments.
         */
        template <typename... T>
            requires std::invocable<F, A..., T...>
        constexpr decltype(auto) operator()(T &&...CallArgs) && noexcept(std::is_nothrow_invocable_v<F, A..., T...>) {
            return std::apply(
                [&]<typename... U>(U &&...FinalArgs) -> decltype(auto) {
                    return std::invoke(Functor, std::forward<U>(FinalArgs)..., std::forward<T>(CallArgs)...);
                },
                std::move(args));
        }

      private:
        ArgsTuple args;
    };

    /**
     * @class TBindFrontConstInvoker
     * @brief A utility class to bind an argument to the front of a callable object or functor.
     *
     * The BindFrontConstInvoker template class is used to create objects that bind a specified
     * argument to be used as the first argument in the invocation of a functor. This binding
     * is done at the time of construction, allowing the functor to be invoked later with
     * additional arguments.
     *
     * @tparam Functor The callable object or functor to which the argument is to be bound.
     *          This functor must satisfy specific conditions to be considered valid
     *          as checked by the `IsValidFunctorObject`.
     *
     * @tparam A The type of the argument to be bound to the functor. The bound argument
     *          will be passed as the first parameter during invocation.
     */
    template <auto Functor, typename A>
        requires(IsValidFunctorObject(Functor))
    struct TBindFrontConstInvoker<Functor, A> {
        using F = decltype(Functor);

        /**
         * @brief Constructs a BindFrontConstInvoker object by binding a specified argument.
         *
         * The constructor template allows creating a BindFrontConstInvoker object with
         * an argument that will be bound as the first argument during the invocation
         * of a functor. The bound argument is stored within the object to enable
         * call-time combination with additional arguments.
         *
         * @tparam T The type of the argument being bound. This type must be convertible
         *           to the type A, which is specified at the class level. Additionally,
         *           the template enforces that T is not the same as BindFrontConstInvoker
         *           after type decay, preventing recursive instantiation.
         *
         * @param Arg The argument to bind to the functor. This argument is forwarded
         *            and stored in the BindFrontConstInvoker object to be used during
         *            subsequent invocations of the associated functor.
         */
        template <typename T>
            requires std::convertible_to<T, A> && (!std::same_as<std::decay_t<T>, TBindFrontConstInvoker>)
        constexpr explicit TBindFrontConstInvoker(T &&Arg) : Arg(std::forward<T>(Arg)) {
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
            requires std::invocable<F, A &, T...>
        constexpr decltype(auto) operator()(T &&...CallArgs) & noexcept(std::is_nothrow_invocable_v<F, A &, T...>) {
            return std::invoke(Functor, Arg, std::forward<T>(CallArgs)...);
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
            requires std::invocable<F, const A &, T...>
        constexpr decltype(auto)
        operator()(T &&...CallArgs) const & noexcept(std::is_nothrow_invocable_v<F, const A &, T...>) {
            return std::invoke(Functor, Arg, std::forward<T>(CallArgs)...);
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
            requires std::invocable<F, A, T...>
        constexpr decltype(auto) operator()(T &&...CallArgs) && noexcept(std::is_nothrow_invocable_v<F, A, T...>) {
            return std::invoke(Functor, std::move(Arg), std::forward<T>(CallArgs)...);
        }

      private:
        A Arg;
    };

    /**
     * @brief A utility structure that binds the first two arguments to a functor and provides multiple callable
     * operators.
     *
     * The `BindFrontConstInvoker` template binds the first two arguments to a specified functor at the time of its
     * creation. This allows the invoker object to be called later with additional arguments, combining them with the
     * pre-bound ones for the final invocation.
     *
     * @tparam Functor The functor object or function that forms the basis of the invocation.
     * @tparam A The type of the first argument to be bound to the functor.
     * @tparam B The type of the second argument to be bound to the functor.
     *
     * @requires The `IsValidFunctorObject(Functor)` constraint ensures that the provided Functor is a valid callable
     * entity.
     *
     * This struct provides a means to partially apply a functor with a fixed set of initial arguments and ensure
     * that the object is still callable with additional arguments that can complete the function call.
     *
     * The callable operators support different value categories (lvalue, const lvalue, rvalue) of the invoker object,
     * ensuring flexibility in how the bound arguments are passed or moved during invocation.
     *
     */
    template <auto Functor, typename A, typename B>
        requires(IsValidFunctorObject(Functor))
    struct TBindFrontConstInvoker<Functor, A, B> {
        using F = decltype(Functor);

        /**
         * @brief Constructs a BindFrontConstInvoker object with the first two arguments pre-bound.
         *
         * This constructor initializes the BindFrontConstInvoker with two arguments that are bound
         * to the functor to be invoked later. The arguments must be convertible to the specified
         * types A and B respectively.
         *
         * @tparam T The type of the first argument to be bound, which must be convertible to type A.
         * @tparam U The type of the second argument to be bound, which must be convertible to type B.
         *
         * @param Arg1 The first argument to be bound to the functor.
         * @param Arg2 The second argument to be bound to the functor.
         *
         * @requires std::convertible_to<T, A> Ensures that arg1 is convertible to type A.
         * @requires std::convertible_to<U, B> Ensures that arg2 is convertible to type B.
         */
        template <typename T, typename U>
            requires std::convertible_to<T, A> && std::convertible_to<U, B>
        constexpr TBindFrontConstInvoker(T &&Arg1, U &&Arg2)
            : Arg1(std::forward<T>(Arg1)), Arg2(std::forward<U>(Arg2)) {
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
            requires std::invocable<F, A &, B &, T...>
        constexpr decltype(auto)
        operator()(T &&...CallArgs) & noexcept(std::is_nothrow_invocable_v<F, A &, B &, T...>) {
            return std::invoke(Functor, Arg1, Arg2, std::forward<T>(CallArgs)...);
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
            requires std::invocable<F, const A &, const B &, T...>
        constexpr decltype(auto)
        operator()(T &&...CallArgs) const & noexcept(std::is_nothrow_invocable_v<F, const A &, const B &, T...>) {
            return std::invoke(Functor, Arg1, Arg2, std::forward<T>(CallArgs)...);
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
            requires std::invocable<F, A, B, T...>
        constexpr decltype(auto) operator()(T &&...CallArgs) && noexcept(std::is_nothrow_invocable_v<F, A, B, T...>) {
            return std::invoke(Functor, std::move(Arg1), std::move(Arg2), std::forward<T>(CallArgs)...);
        }

      private:
        A Arg1;
        B Arg2;
    };

    /**
     * @brief Binds a set of arguments to the front of a specified functor.
     *
     * The `bind_front` function template creates a `BindFrontConstInvoker` by capturing
     * a given set of arguments and associating them with the front of the specified functor.
     * This function template ensures that the provided functor is valid using the
     * `IsValidFunctorObject` constraint. It leverages perfect forwarding to
     * efficiently bind arguments without unnecessary copies.
     *
     * @tparam Functor The type of the functor to which the arguments will be bound.
     *                 This template parameter must satisfy `IsValidFunctorObject`.
     * @tparam A Variadic template parameters representing the types of the arguments
     *           to be bound to the front of the functor.
     * @param Args Arguments to be bound and forwarded to the invoker.
     * @return An instance of `BindFrontConstInvoker` with the specified functor
     *         and bound arguments.
     */
    RETROLIB_EXPORT template <auto Functor, typename... A>
        requires(IsValidFunctorObject(Functor))
    constexpr auto BindFront(A &&...Args) {
        return TBindFrontConstInvoker<Functor, std::decay_t<A>...>(std::forward<A>(Args)...);
    }
} // namespace Retro