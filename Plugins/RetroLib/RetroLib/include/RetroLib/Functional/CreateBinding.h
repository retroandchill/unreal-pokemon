/**
 * @file CreateBinding.h
 * @brief Creating a binding from generic arguments.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/Concepts/Tuples.h"
#include "RetroLib/Functional/BindBack.h"
#include "RetroLib/Functional/BindFunctor.h"
#include "RetroLib/Functional/BindMethod.h"
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif
#include <RetroLib/Concepts/Tuples.h>

namespace Retro {
    /**
     * @brief Struct tag to signify the usage of the `this` parameter in a functional type.
     */
    struct ThisType {};

    /**
     * @brief Constant value that represents the `this` parameter in functional types or type traits.
     */
    RETROLIB_EXPORT constexpr ThisType This;

    /**
     * @brief A utility class that wraps a callable object, enabling invocation and optional state management.
     *
     * The WrappedFunctor class serves as a mechanism to encapsulate a callable object, such as a lambda,
     * function pointer, or function object, allowing the object to be managed and invoked with ease. This
     * abstraction is particularly useful for scenarios requiring dynamic handling of callables or delayed
     * execution.
     */
    template <HasFunctionCallOperator F>
    struct WrappedFunctor {
        /**
         * @brief Constructs a WrappedFunctor by forwarding the given functor.
         *
         * @param Functor The functor object to be wrapped, which will be forwarded.
         */
        template <typename T>
            requires std::constructible_from<F, T> && (!std::same_as<std::decay_t<T>, WrappedFunctor>)
        constexpr explicit WrappedFunctor(T &&Functor) : Functor(std::forward<T>(Functor)) {
        }

        /**
         * @brief Invokes the stored functor with the provided arguments.
         *
         * The operator performs the invocation of the stored callable object
         * using the provided arguments, ensuring that the invocation conforms
         * to the noexcept condition based on the properties of the functor and arguments.
         *
         * @param Args The arguments to be forwarded to the stored functor.
         * @return The result of invoking the stored functor with the forwarded arguments.
         *         The exact return type depends on the functor and the arguments.
         */
        template <typename... A>
            requires std::invocable<F &, A...>
        constexpr decltype(auto) operator()(A &&...Args) & noexcept(std::is_nothrow_invocable_v<F &, A...>) {
            return std::invoke(Functor, std::forward<A>(Args)...);
        }

        /**
         * @brief Invokes the stored functor with the provided arguments.
         *
         * The operator performs the invocation of the stored callable object
         * using the provided arguments, ensuring that the invocation conforms
         * to the noexcept condition based on the properties of the functor and arguments.
         *
         * @param Args The arguments to be forwarded to the stored functor.
         * @return The result of invoking the stored functor with the forwarded arguments.
         *         The exact return type depends on the functor and the arguments.
         */
        template <typename... A>
            requires std::invocable<const F &, A...>
        constexpr decltype(auto)
        operator()(A &&...Args) const & noexcept(std::is_nothrow_invocable_v<const F &, A...>) {
            return std::invoke(Functor, std::forward<A>(Args)...);
        }

        /**
         * @brief Invokes the stored functor with the provided arguments.
         *
         * The operator performs the invocation of the stored callable object
         * using the provided arguments, ensuring that the invocation conforms
         * to the noexcept condition based on the properties of the functor and arguments.
         *
         * @param Args The arguments to be forwarded to the stored functor.
         * @return The result of invoking the stored functor with the forwarded arguments.
         *         The exact return type depends on the functor and the arguments.
         */
        template <typename... A>
            requires std::invocable<F, A...>
        constexpr decltype(auto) operator()(A &&...Args) && noexcept(std::is_nothrow_invocable_v<F, A...>) {
            return std::invoke(std::move(Functor), std::forward<A>(Args)...);
        }

        /**
         * @brief Invokes the functor with the given arguments using `std::apply`.
         *
         * This function invokes the stored functor with the provided arguments, forwarding them appropriately.
         * The operation adheres to the noexcept specification determined by the `NoThrowApplicable` trait.
         *
         * @param Args The arguments to be forwarded to the functor.
         * @return The result of invoking the functor with the forwarded arguments.
         */
        template <typename U>
            requires CanApply<F &, U>
        constexpr decltype(auto) operator()(U &&Args) & noexcept(NoThrowApplicable<F &, U>) {
            return std::apply(Functor, std::forward<U>(Args));
        }

        /**
         * @brief Invokes the functor with the given arguments using `std::apply`.
         *
         * This function invokes the stored functor with the provided arguments, forwarding them appropriately.
         * The operation adheres to the noexcept specification determined by the `NoThrowApplicable` trait.
         *
         * @param Args The arguments to be forwarded to the functor.
         * @return The result of invoking the functor with the forwarded arguments.
         */
        template <typename U>
            requires CanApply<const F &, U>
        constexpr decltype(auto) operator()(U &&Args) const & noexcept(NoThrowApplicable<const F &, U>) {
            return std::apply(Functor, std::forward<U>(Args));
        }

        /**
         * @brief Invokes the functor with the given arguments using `std::apply`.
         *
         * This function invokes the stored functor with the provided arguments, forwarding them appropriately.
         * The operation adheres to the noexcept specification determined by the `NoThrowApplicable` trait.
         *
         * @param Args The arguments to be forwarded to the functor.
         * @return The result of invoking the functor with the forwarded arguments.
         */
        template <typename U>
            requires CanApply<F, U>
        constexpr decltype(auto) operator()(U &&Args) && noexcept(NoThrowApplicable<F, U>) {
            return std::apply(std::move(Functor), std::forward<U>(Args));
        }

      private:
        F Functor;
    };

    /**
     * Creates a WrappedFunctor object by deducing and decaying the type of the input functor.
     *
     * @tparam F The input functor to be wrapped.
     * @return A WrappedFunctor object with the decayed type of the input functor.
     */
    template <HasFunctionCallOperator F>
    WrappedFunctor(F) -> WrappedFunctor<std::decay_t<F>>;

    /**
     * @brief Creates a binding by wrapping a functor and optional arguments into a WrappedFunctor object.
     *
     * This function generates a WrappedFunctor either directly from the provided functor if no
     * additional arguments are supplied or by binding the provided arguments to the functor using `bind_back`.
     *
     * @tparam F The type of the functor to be wrapped.
     * @tparam A The types of the additional arguments to bind to the functor.
     * @param Functor The callable object to be wrapped.
     * @param Args The additional arguments to bind to the callable object (if any).
     * @return A WrappedFunctor containing the provided functor and any bound arguments.
     */
    RETROLIB_EXPORT template <typename F, typename... A>
        requires HasFunctionCallOperator<std::decay_t<F>>
    constexpr auto CreateBinding(F &&Functor, A &&...Args) {
        if constexpr (sizeof...(A) == 0) {
            return WrappedFunctor(std::forward<F>(Functor));
        } else {
            return WrappedFunctor(BindBack(std::forward<F>(Functor), std::forward<A>(Args)...));
        }
    }

    /**
     * @brief Creates a binding by wrapping a functor and its associated arguments.
     *
     * This function binds a callable object along with its arguments and wraps it into
     * a WrappedFunctor for further use.
     *
     * @param Obj The object to which the functor is bound.
     * @param Functor The callable to be executed.
     * @param Args Additional arguments to be passed to the functor.
     * @return A WrappedFunctor object encapsulating the bound functor and its arguments.
     */
    RETROLIB_EXPORT template <typename C, typename F, typename... A>
        requires Member<F>
    constexpr auto CreateBinding(C &&Obj, F &&Functor, A &&...Args) {
        return WrappedFunctor(BindMethod(std::forward<C>(Obj), std::forward<F>(Functor), std::forward<A>(Args)...));
    }

    /**
     * @brief Creates a binding for the given functor with optional arguments.
     *
     * This method conditionally creates a binding based on the presence of arguments.
     * If no arguments are provided, it initializes a binding for the functor.
     * If arguments are supplied, it binds the functor with the provided arguments.
     *
     * @tparam A Types of the arguments to bind to the functor.
     * @param Args Arguments to bind to the functor.
     * @return A callable object that represents the bound functor.
     */
    RETROLIB_EXPORT template <auto Functor, typename... A>
        requires HasFunctionCallOperator<decltype(Functor)>
    constexpr auto CreateBinding(A &&...Args) {
        if constexpr (sizeof...(A) == 0) {
            return WrappedFunctor(BindFunctor<Functor>());
        } else {
            return WrappedFunctor(BindBack<Functor>(std::forward<A>(Args)...));
        }
    }

    /**
     * @brief Creates a binding by forwarding the provided object and arguments to bind a method.
     *
     * @tparam Functor The callable object type that is used internally.
     * @tparam C The type of the object to be bound.
     * @tparam A The types of additional arguments.
     * @param Obj The object that the method will be bound to.
     * @param Args The additional arguments to be forwarded to the binding function.
     * @return A callable object created by binding the specified method.
     */
    RETROLIB_EXPORT template <auto Functor, typename C, typename... A>
        requires Member<decltype(Functor)>
    constexpr auto CreateBinding(ThisType, C &&Obj, A &&...Args) {
        return WrappedFunctor(BindMethod<Functor>(std::forward<C>(Obj), std::forward<A>(Args)...));
    }

    RETROLIB_EXPORT template <typename>
    struct AdditionalBindingTypes : InvalidType {};

    RETROLIB_EXPORT template <typename F, typename... A>
    concept HasExtenededBinding = AdditionalBindingTypes<std::decay_t<F>>::IsValid && requires(F&& Functor, A&&... Args) {
        { AdditionalBindingTypes<std::decay_t<F>>::Bind(std::forward<F>(Functor), std::forward<A>(Args)...) } -> HasFunctionCallOperator;
    };

    RETROLIB_EXPORT template <typename F, typename... A>
        requires HasExtenededBinding<F, A...>
    constexpr auto CreateBinding(F&& Functor, A &&...Args) {
        return AdditionalBindingTypes<std::decay_t<F>>::Bind(std::forward<F>(Functor), std::forward<A>(Args)...);
    }

    RETROLIB_EXPORT template <typename... A>
    using BindingType = decltype(CreateBinding(std::declval<A>()...));

    RETROLIB_EXPORT template <auto Functor, typename... A>
        requires HasFunctionCallOperator<decltype(Functor)>
    using ConstBindingType = decltype(CreateBinding<Functor>(std::declval<A>()...));

    struct DynamicFunctorTag {};

    /**
     * Constant tag used to signify that functional object is bounding by taking in a functor as its argument.
     */
    RETROLIB_EXPORT constexpr DynamicFunctorTag DynamicFunctor;

    template <auto Functor>
    concept DynamicFunctorBinding = std::same_as<std::decay_t<decltype(Functor)>, DynamicFunctorTag>;

    template <auto Functor>
    concept ValidFunctorParameter = DynamicFunctorBinding<Functor> || IsValidFunctorObject(Functor);

} // namespace retro
