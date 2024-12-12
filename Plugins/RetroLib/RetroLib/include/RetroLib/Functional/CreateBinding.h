/**
 * @file CreateBinding.h
 * @brief Creating a binding from generic arguments.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/Functional/BindBack.h"
#include "RetroLib/Functional/BindFunctor.h"
#include "RetroLib/Functional/BindMethod.h"
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace retro {
    /**
     * @brief Struct tag to signify the usage of the `this` parameter in a functional type.
     */
    struct ThisType {};

    /**
     * @brief Constant value that represents the `this` parameter in functional types or type traits.
     */
    RETROLIB_EXPORT constexpr ThisType this_type;

    /**
     * @brief Creates a binding for the given functor with optional arguments.
     *
     * This method conditionally creates a binding based on the presence of arguments.
     * If no arguments are provided, it initializes a binding for the functor.
     * If arguments are supplied, it binds the functor with the provided arguments.
     *
     * @tparam A Types of the arguments to bind to the functor.
     * @param args Arguments to bind to the functor.
     * @return A callable object that represents the bound functor.
     */
    RETROLIB_EXPORT template <auto Functor, typename... A>
        requires HasFunctionCallOperator<decltype(Functor)>
    constexpr auto create_binding(A&&... args) {
        if constexpr (sizeof...(A) == 0) {
            return bind_functor<Functor>();
        } else {
            return bind_back<Functor>(std::forward<A>(args)...);
        }
    }

    /**
     * @brief Creates a binding by forwarding the provided object and arguments to bind a method.
     *
     * @tparam Functor The callable object type that is used internally.
     * @tparam C The type of the object to be bound.
     * @tparam A The types of additional arguments.
     * @param obj The object that the method will be bound to.
     * @param args The additional arguments to be forwarded to the binding function.
     * @return A callable object created by binding the specified method.
     */
    RETROLIB_EXPORT template <auto Functor, typename C, typename... A>
        requires Member<decltype(Functor)>
    constexpr auto create_binding(ThisType, C&& obj, A&&... args) {
        return bind_method<Functor>(std::forward<C>(obj), std::forward<A>(args)...);
    }


}