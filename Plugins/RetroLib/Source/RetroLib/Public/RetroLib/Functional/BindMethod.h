/**
 * @file BindMethod.h
 * @brief Special functional binding methods for usage with methods.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#include "RetroLib/Functional/BindFront.h"
#include "RetroLib/FunctionTraits.h"

#if !RETROLIB_WITH_MODULES
#include <functional>
#include <tuple>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro {
    /**
     * Checks if the given method can be bound to the given object type.
     *
     * @tparam T The object type
     * @tparam M The method type
     */
    RETROLIB_EXPORT template <typename T, typename M>
    concept CanBindMethod =
        Method<M> && (std::convertible_to<T, TRefQualifiedClassType<M>> ||
                      (Dereferenceable<T> && std::convertible_to<TDereferencedType<T>, TRefQualifiedClassType<M>>));

    /**
     * @brief A utility structure for method binding with pre-bound arguments and invocability.
     *
     * The MethodBinding class allows the creation of a callable object that binds a functor (e.g., a member function
     * pointer, callable object, or lambda) with pre-bound arguments. It enables subsequent invocation with additional
     * arguments, combining them with the bound arguments to perform the final function call.
     *
     * @tparam C Type of the object to which the method belongs.
     * @tparam F Type of the functor or callable object.
     * @tparam A Types of the pre-bound arguments to the functor.
     */
    template <typename C, Method F, typename... A>
        requires CanBindMethod<C, F>
    struct TMethodBinding {
        using ArgsTuple = std::tuple<A...>;

        /**
         * @brief Constructs a MethodBinding object with an object instance, a functor, and optional arguments.
         *
         * Initializes the MethodBinding with a provided object, functor or callable, and additional arguments that can
         * be forwarded.
         *
         * @param Object The instance of the object associated with the binding.
         * @param Functor The callable or functor to be executed with the binding.
         * @param Args Additional arguments to be forwarded to the functor.
         */
        template <typename T, typename G, typename... U>
            requires std::convertible_to<C, T> && std::constructible_from<F, G> &&
                         std::constructible_from<ArgsTuple, U...>
        constexpr TMethodBinding(T &&Object, G &&Functor, U &&...Args)
            : Object(std::forward<T>(Object)), Functor(std::forward<G>(Functor)), Args(std::forward<U>(Args)...) {
        }

        /**
         * @brief Invokes the stored functor with the specified call-time arguments
         *        followed by the pre-bound arguments.
         *
         * This invocable operator enables the MethodConstBinding to be called with
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
            requires std::invocable<F &, C &, T..., A &...>
        constexpr decltype(auto)
        operator()(T &&...CallArgs) & noexcept(std::is_nothrow_invocable_v<F, C &, T..., A &...>) {
            return std::apply(
                [&]<typename... U>(U &&...FinalArgs) -> decltype(auto) {
                    return std::invoke(Functor, Object, std::forward<T>(CallArgs)..., std::forward<U>(FinalArgs)...);
                },
                Args);
        }

        /**
         * @brief Invokes the stored functor with the specified call-time arguments
         *        followed by the pre-bound arguments.
         *
         * This invocable operator enables the MethodConstBinding to be called with
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
            requires std::invocable<const F &, const C &, T..., const A &...>
        constexpr decltype(auto)
        operator()(T &&...CallArgs) const & noexcept(std::is_nothrow_invocable_v<F, const C &, T..., const A &...>) {
            return std::apply(
                [&]<typename... U>(U &&...FinalArgs) -> decltype(auto) {
                    return std::invoke(Functor, Object, std::forward<T>(CallArgs)..., std::forward<U>(FinalArgs)...);
                },
                Args);
        }

        /**
         * @brief Invokes the stored functor with the specified call-time arguments
         *        followed by the pre-bound arguments.
         *
         * This invocable operator enables the MethodConstBinding to be called with
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
            requires std::invocable<F, C, T..., A...>
        constexpr decltype(auto)
        operator()(T &&...CallArgs) && noexcept(std::is_nothrow_invocable_v<F, C, T..., A...>) {
            return std::apply(
                [&]<typename... U>(U &&...FinalArgs) -> decltype(auto) {
                    return std::invoke(std::move(Functor), std::move(Object), std::forward<T>(CallArgs)...,
                                       std::forward<U>(FinalArgs)...);
                },
                std::move(Args));
        }

      private:
        C Object;
        F Functor;
        ArgsTuple Args;
    };

    /**
     * @struct TMethodBinding
     * @brief A utility that binds an object, a callable (functor), and an additional pre-bound argument
     *        into a single callable entity.
     *
     * This struct provides a mechanism to associate a callable (functor) with a specific object and
     * pre-bound arguments, enabling partial application and method-like invocation semantics. It supports
     * safe forwarding and invocation according to the callable's exception specifications.
     *
     * @tparam C The type of the object bound to the callable.
     * @tparam F The type of the callable to be invoked.
     * @tparam A The type of the pre-bound argument.
     */
    template <typename C, Method F, typename A>
        requires CanBindMethod<C, F>
    struct TMethodBinding<C, F, A> {
        /**
         * @brief Constructs a MethodBinding object with the given parameters.
         *
         * This constructor initializes the MethodBinding object by forwarding the provided
         * callable object, functor, and additional argument. It ensures the proper binding
         * of a method with its related argument for later invocation.
         *
         * @tparam T The type of the callable object, typically the object containing the method to be bound.
         * @tparam G The type of the functor, representing the callable method or function.
         * @tparam U The type of the argument to be forwarded and associated with the method.
         * @param Object The callable object or instance whose method is to be bound.
         * @param Functor The callable functor, method, or function to bind.
         * @param Arg The argument to be forwarded and used during method invocation.
         */
        template <typename T, typename G, typename U>
            requires std::convertible_to<T, C> && std::constructible_from<F, G> && std::convertible_to<U, A>
        constexpr TMethodBinding(T &&Object, G &&Functor, U &&Arg)
            : Object(std::forward<T>(Object)), Functor(std::forward<G>(Functor)), Arg(std::forward<U>(Arg)) {
        }

        /**
         * @brief Invokes the stored functor with the specified call-time arguments
         *        followed by the pre-bound arguments.
         *
         * This invocable operator enables the MethodConstBinding to be called with
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
            requires std::invocable<F &, C &, T..., A &>
        constexpr decltype(auto)
        operator()(T &&...CallArgs) & noexcept(std::is_nothrow_invocable_v<F &, C &, T..., A &>) {
            return std::invoke(Functor, Object, std::forward<T>(CallArgs)..., Arg);
        }

        /**
         * @brief Invokes the stored functor with the specified call-time arguments
         *        followed by the pre-bound arguments.
         *
         * This invocable operator enables the MethodConstBinding to be called with
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
            requires std::invocable<const F &, const C &, T..., const A &>
        constexpr decltype(auto) operator()(T &&...CallArgs) const & noexcept(
            std::is_nothrow_invocable_v<const F &, const C &, T..., const A &>) {
            return std::invoke(Functor, Object, std::forward<T>(CallArgs)..., Arg);
        }

        /**
         * @brief Invokes the stored functor with the specified call-time arguments
         *        followed by the pre-bound arguments.
         *
         * This invocable operator enables the MethodConstBinding to be called with
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
            requires std::invocable<F, C, T..., A>
        constexpr decltype(auto) operator()(T &&...CallArgs) && noexcept(std::is_nothrow_invocable_v<F, C, T..., A>) {
            return std::invoke(std::move(Functor), std::move(Object), std::forward<T>(CallArgs)..., std::move(Arg));
        }

      private:
        C Object;
        F Functor;
        A Arg;
    };

    /**
     * @brief A structure that encapsulates a callable object (functor) with an associated
     *        context object and two pre-bound arguments.
     *
     * The `MethodBinding` class template provides functionality to bind a callable object
     * (such as a member function or functor) with a specific object and two pre-determined
     * arguments. It allows for later invocation with additional call-time arguments, while
     * seamlessly integrating the pre-bound arguments during the call.
     *
     * @tparam C The type of the context object to which the callable object is bound.
     * @tparam F The type of the callable object (e.g., a functor or callable function).
     * @tparam A The type of the first pre-bound argument.
     * @tparam B The type of the second pre-bound argument.
     */
    template <typename C, Method F, typename A, typename B>
        requires CanBindMethod<C, F>
    struct TMethodBinding<C, F, A, B> {

        /**
         * @brief Constructs a MethodBinding object with specified object, functor, and arguments.
         *
         * Initializes the MethodBinding object using the provided callable object,
         * the functor, and two arguments. The parameters are passed using forwarding
         * references to allow for perfect forwarding of their values.
         *
         * @tparam T Type of the callable object.
         * @tparam G Type of the functor.
         * @tparam U Type of the first argument.
         * @tparam W Type of the second argument.
         *
         * @param Object The callable object that will own the operation.
         * @param Functor A callable entity (e.g., member function pointer) to be invoked.
         * @param Arg1 The first argument to be passed to the functor.
         * @param Arg2 The second argument to be passed to the functor.
         */
        template <typename T, typename G, typename U, typename W>
            requires std::convertible_to<T, C> && std::constructible_from<F, G> && std::convertible_to<U, A> &&
                         std::convertible_to<W, B>
        constexpr explicit TMethodBinding(T &&Object, G &&Functor, U &&Arg1, W &&Arg2)
            : Object(std::forward<T>(Object)), Functor(std::forward<G>(Functor)), Arg1(std::forward<U>(Arg1)),
              Arg2(std::forward<W>(Arg2)) {
        }

        /**
         * @brief Invokes the stored functor with the specified call-time arguments
         *        followed by the pre-bound arguments.
         *
         * This invocable operator enables the MethodConstBinding to be called with
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
            requires std::invocable<F &, C &, T..., A &, B &>
        constexpr decltype(auto)
        operator()(T &&...CallArgs) & noexcept(std::is_nothrow_invocable_v<F &, C &, T..., A &, B &>) {
            return std::invoke(Functor, Object, std::forward<T>(CallArgs)..., Arg1, Arg2);
        }

        /**
         * @brief Invokes the stored functor with the specified call-time arguments
         *        followed by the pre-bound arguments.
         *
         * This invocable operator enables the MethodConstBinding to be called with
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
            requires std::invocable<const F &, C, T..., const A &, const B &>
        constexpr decltype(auto) operator()(T &&...CallArgs) const & noexcept(
            std::is_nothrow_invocable_v<const F &, const C &, T..., const A &, const B &>) {
            return std::invoke(Functor, Object, std::forward<T>(CallArgs)..., Arg1, Arg2);
        }

        /**
         * @brief Invokes the stored functor with the specified call-time arguments
         *        followed by the pre-bound arguments.
         *
         * This invocable operator enables the MethodConstBinding to be called with
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
            requires std::invocable<F, C, T..., A, B>
        constexpr decltype(auto)
        operator()(T &&...CallArgs) && noexcept(std::is_nothrow_invocable_v<F, C, T..., A, B>) {
            return std::invoke(std::move(Functor), std::move(Object), std::forward<T>(CallArgs)..., std::move(Arg1),
                               std::move(Arg2));
        }

      private:
        C Object;
        F Functor;
        A Arg1;
        B Arg2;
    };

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
     */
    template <typename C, auto Functor, typename... A>
        requires CanBindMethod<C, decltype(Functor)>
    struct TMethodConstBinding {
        using F = decltype(Functor);
        using ArgsTuple = std::tuple<A...>;

        /**
         * @brief Constructs a MethodConstBinding object with a specified object and arguments.
         *
         * @tparam T The type of the object being bound. Must be convertible to the type C in the MethodConstBinding.
         * @tparam U Variadic template arguments representing types for additional arguments passed during construction.
         *
         * @param Object The object to which the method is bound.
         * @param Args Additional arguments to be bound, used in conjunction with the member function.
         *
         * @requires The constructor requires that the type T is not the same as a decayed MethodConstBinding type,
         *           ensuring that the object type is convertible to C, and the additional arguments can be
         *           constructed into ArgsTuple.
         */
        template <typename T, typename... U>
            requires std::convertible_to<C, T> && std::constructible_from<ArgsTuple, U...> &&
                         (!std::same_as<std::decay_t<T>, TMethodConstBinding>)
        constexpr explicit TMethodConstBinding(T &&Object, U &&...Args)
            : Object(std::forward<T>(Object)), Args(std::forward<U>(Args)...) {
        }

        /**
         * @brief Invokes the stored functor with the specified call-time arguments
         *        followed by the pre-bound arguments.
         *
         * This invocable operator enables the MethodConstBinding to be called with
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
        constexpr decltype(auto)
        operator()(T &&...CallArgs) & noexcept(std::is_nothrow_invocable_v<F, C &, T..., A &...>) {
            return std::apply(
                [&]<typename... U>(U &&...FinalArgs) -> decltype(auto) {
                    return std::invoke(Functor, Object, std::forward<T>(CallArgs)..., std::forward<U>(FinalArgs)...);
                },
                Args);
        }

        /**
         * @brief Invokes the stored functor with the specified call-time arguments
         *        followed by the pre-bound arguments.
         *
         * This invocable operator enables the MethodConstBinding to be called with
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
        constexpr decltype(auto)
        operator()(T &&...CallArgs) const & noexcept(std::is_nothrow_invocable_v<F, const C &, T..., const A &...>) {
            return std::apply(
                [&]<typename... U>(U &&...FinalArgs) -> decltype(auto) {
                    return std::invoke(Functor, Object, std::forward<T>(CallArgs)..., std::forward<U>(FinalArgs)...);
                },
                Args);
        }

        /**
         * @brief Invokes the stored functor with the specified call-time arguments
         *        followed by the pre-bound arguments.
         *
         * This invocable operator enables the MethodConstBinding to be called with
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
        constexpr decltype(auto)
        operator()(T &&...CallArgs) && noexcept(std::is_nothrow_invocable_v<F, C, T..., A...>) {
            return std::apply(
                [&]<typename... U>(U &&...FinalArgs) -> decltype(auto) {
                    return std::invoke(Functor, std::move(Object), std::forward<T>(CallArgs)...,
                                       std::forward<U>(FinalArgs)...);
                },
                std::move(Args));
        }

      private:
        C Object;
        ArgsTuple Args;
    };

    /**
     * @brief A utility structure to encapsulate the binding of a specific method
     *        to a class instance with predefined arguments.
     *
     * The MethodConstBinding struct is designed to wrap a method (Functor) from a class (C)
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
    struct TMethodConstBinding<C, Functor, A> {
        using F = decltype(Functor);

        /**
         * @brief Constructs a MethodConstBinding instance, initializing the object and argument
         *        that will be used during method invocation.
         *
         * This constructor ensures that the provided object and argument are suitable types
         * for the method being bound. The arguments are forwarded to ensure they are
         * perfectly captured.
         *
         * @tparam T The type of the object that the method will be invoked on.
         * @tparam U The type of the argument to be pre-bound to the method.
         * @param Object An instance of the class containing the method to be bound.
         * @param Arg The argument to be used when invoking the method.
         *
         * @note The constructor uses concepts to ensure the provided object and argument
         * types are convertible to the expected types for the class and argument.
         */
        template <typename T, typename U>
            requires std::convertible_to<T, C> && std::convertible_to<U, A>
        constexpr explicit TMethodConstBinding(T &&Object, U &&Arg)
            : Object(std::forward<T>(Object)), Arg(std::forward<U>(Arg)) {
        }

        /**
         * @brief Invokes the stored functor with the specified call-time arguments
         *        followed by the pre-bound arguments.
         *
         * This invocable operator enables the MethodConstBinding to be called with
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
            requires std::invocable<F, C &, T..., A &>
        constexpr decltype(auto)
        operator()(T &&...CallArgs) & noexcept(std::is_nothrow_invocable_v<F, C &, T..., A &>) {
            return std::invoke(Functor, Object, std::forward<T>(CallArgs)..., Arg);
        }

        /**
         * @brief Invokes the stored functor with the specified call-time arguments
         *        followed by the pre-bound arguments.
         *
         * This invocable operator enables the MethodConstBinding to be called with
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
            requires std::invocable<F, const C &, T..., const A &>
        constexpr decltype(auto)
        operator()(T &&...CallArgs) const & noexcept(std::is_nothrow_invocable_v<F, const C &, T..., const A &>) {
            return std::invoke(Functor, Object, std::forward<T>(CallArgs)..., Arg);
        }

        /**
         * @brief Invokes the stored functor with the specified call-time arguments
         *        followed by the pre-bound arguments.
         *
         * This invocable operator enables the MethodConstBinding to be called with
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
            requires std::invocable<F, C, T..., A>
        constexpr decltype(auto) operator()(T &&...CallArgs) && noexcept(std::is_nothrow_invocable_v<F, C, T..., A>) {
            return std::invoke(Functor, std::move(Object), std::forward<T>(CallArgs)..., std::move(Arg));
        }

      private:
        C Object;
        A Arg;
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
     * - The method may be invoked on an lvalue reference to MethodConstBinding, forwarding the object and
     *   bound arguments as lvalue references.
     * - The method may be invoked on a const lvalue reference to MethodConstBinding, treating all
     *   arguments as const lvalue references.
     * - The method may be invoked on an rvalue reference to MethodConstBinding, forwarding the object and
     *   bound arguments as rvalue references.
     *
     * @remark These invocations are noexcept if the provided Functor is nothrow-invocable in the
     * respective context.
     */
    template <typename C, auto Functor, typename A, typename B>
        requires CanBindMethod<C, decltype(Functor)>
    struct TMethodConstBinding<C, Functor, A, B> {
        using F = decltype(Functor);

        /**
         * @brief Constructs a MethodConstBinding instance by binding the given object and arguments.
         *
         * This constructor initializes a MethodConstBinding struct by binding an object and two arguments,
         * allowing a method associated with the object to be called later with additional arguments.
         *
         * @tparam T The type of the object to be bound. Must be convertible to the template parameter C.
         * @tparam U The type of the first argument to be bound. Must be convertible to the template parameter A.
         * @tparam W The type of the second argument to be bound. Must be convertible to the template parameter B.
         *
         * @param Object The object to which the method is bound.
         * @param Arg1 The first argument to be bound to the method.
         * @param Arg2
         *
         * @note The constructor is explicit and constexpr, ensuring that the binding operation is executed at
         * compile-time for qualified contexts and preventing unintended implicit conversions.
         */
        template <typename T, typename U, typename W>
            requires std::convertible_to<T, C> && std::convertible_to<U, A> && std::convertible_to<W, B>
        constexpr explicit TMethodConstBinding(T &&Object, U &&Arg1, W &&Arg2)
            : Object(std::forward<T>(Object)), Arg1(std::forward<U>(Arg1)), Arg2(std::forward<W>(Arg2)) {
        }

        /**
         * @brief Invokes the stored functor with the specified call-time arguments
         *        followed by the pre-bound arguments.
         *
         * This invocable operator enables the MethodConstBinding to be called with
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
            requires std::invocable<F, C &, T..., A &, B &>
        constexpr decltype(auto)
        operator()(T &&...CallArgs) & noexcept(std::is_nothrow_invocable_v<F, C &, T..., A &, B &>) {
            return std::invoke(Functor, Object, std::forward<T>(CallArgs)..., Arg1, Arg2);
        }

        /**
         * @brief Invokes the stored functor with the specified call-time arguments
         *        followed by the pre-bound arguments.
         *
         * This invocable operator enables the MethodConstBinding to be called with
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
            requires std::invocable<F, C, T..., const A &, const B &>
        constexpr decltype(auto) operator()(T &&...CallArgs) const & noexcept(
            std::is_nothrow_invocable_v<F, const C &, T..., const A &, const B &>) {
            return std::invoke(Functor, Object, std::forward<T>(CallArgs)..., Arg1, Arg2);
        }

        /**
         * @brief Invokes the stored functor with the specified call-time arguments
         *        followed by the pre-bound arguments.
         *
         * This invocable operator enables the MethodConstBinding to be called with
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
            requires std::invocable<F, C, T..., A, B>
        constexpr decltype(auto)
        operator()(T &&...CallArgs) && noexcept(std::is_nothrow_invocable_v<F, C, T..., A, B>) {
            return std::invoke(Functor, std::move(Object), std::forward<T>(CallArgs)..., std::move(Arg1),
                               std::move(Arg2));
        }

      private:
        C Object;
        A Arg1;
        B Arg2;
    };

    /**
     * Binds a method or functor to an object with optional arguments.
     *
     * If there are no additional arguments, it uses std::bind_front to bind the object and functor.
     * Otherwise, it creates a MethodBinding with the provided object, functor, and arguments.
     *
     * @param Object The object to which the method or functor will be bound.
     * @param Functor The method or callable to bind to the object.
     * @param Args Optional additional arguments to pass to the callable.
     * @return A callable object that can be invoked later with the given object, functor, and arguments bound.
     */
    RETROLIB_EXPORT template <typename C, Member F, typename... A>
    constexpr auto BindMethod(C &&Object, F &&Functor, A &&...Args) {
        if constexpr (sizeof...(A) == 0) {
            return std::bind_front(std::forward<F>(Functor), std::forward<C>(Object));
        } else {
            return TMethodBinding<std::decay_t<C>, std::decay_t<F>, std::decay_t<A>...>(
                std::forward<C>(Object), std::forward<F>(Functor), std::forward<A>(Args)...);
        }
    }

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
     *    MethodConstBinding, which combines the object, method, and additional
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
     * @param Object The object instance to bind the method to.
     * @param Args Optional additional arguments to the method.
     * @return A callable object that binds the specified method to
     *         the provided object and arguments.
     */
    RETROLIB_EXPORT template <auto Functor, typename C, typename... A>
        requires Method<decltype(Functor)>
    constexpr auto BindMethod(C &&Object, A &&...Args) {
        if constexpr (sizeof...(A) == 0) {
            return BindFront<Functor>(std::forward<C>(Object));
        } else {
            return TMethodConstBinding<std::decay_t<C>, Functor, std::decay_t<A>...>(std::forward<C>(Object),
                                                                                    std::forward<A>(Args)...);
        }
    }
} // namespace retro