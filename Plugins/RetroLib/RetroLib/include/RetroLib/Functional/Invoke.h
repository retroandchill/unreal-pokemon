/**
 * @file Invoke.h
 * @brief Invoke specialization to allow for constexpr function binding and dispatch.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/FunctionTraits.h"
#include "RetroLib/Utils/ForwardLike.h"

#include <tuple>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace retro {

    /**
     * Returns a reference to the object of type T or a derived type.
     * If the object is of type T or derived from T, it will be forwarded.
     * Otherwise, the function attempts to convert from std::reference_wrapper.
     *
     * @param object The object that will be checked and potentially forwarded or converted.
     *
     * @return A reference to the object if it is of the expected type or convertible.
     */
    template <typename T, typename O>
        requires std::assignable_from<T&, std::decay_t<O>>
    constexpr decltype(auto) get_type_reference(O&& object) {
        if constexpr (std::same_as<std::decay_t<O>, T> || std::derived_from<std::decay_t<O>, T>) {
            return std::forward<O>(object);
        } else if constexpr (std::assignable_from<T&, O>) {
            return static_cast<T&>(std::forward<O>(object));
        } else if constexpr (std::assignable_from<const T&, O>) {
            return static_cast<const T&>(std::forward<O>(object));
        }
    }

    /**
     * Returns a reference to the underlying type of the given object.
     *
     * @param object An object of any type which will be forwarded. The object must be dereferenceable.
     * @return A reference to the type pointed to by the passed object.
     */
    template <typename T, typename O>
        requires Dereferenceable<O> && std::assignable_from<T&, std::decay_t<DereferencedType<O>>>
    constexpr decltype(auto) get_type_reference(O&& object) {
        return *std::forward<O>(object);
    }

    /**
     * Invokes a member pointer on the provided object with optional arguments.
     *
     * This function checks if the provided member pointer is a member function pointer.
     * - If it is, it invokes the member function on the given object with the specified arguments.
     * - If it is not, it accesses the member variable from the object.
     *
     * @tparam O The type of the object.
     * @tparam A The types of the arguments to pass to the member function.
     * @param object The instance of the object on which to invoke the member pointer.
     * @param args Arguments to pass to the member function, if applicable.
     * @return The result of the member function call or the value of the member variable.
     */
    template<auto Mem, class O, class... A>
        requires Member<decltype(Mem)>
    constexpr decltype(auto) invoke_mem_ptr(O&& object, A&&... args) {
        using M = std::remove_reference_t<decltype(Mem)>;
        using Type = MemberClassType<M>;
        if constexpr (std::is_member_function_pointer_v<M>) {
            return (get_type_reference<Type>(std::forward<O>(object)).*Mem)(std::forward<A>(args)...);
        } else {
            return get_type_reference<Type>(std::forward<O>(object)).*Mem;
        }
    }

    /**
     * Invokes a callable object or a member function pointer with the specified arguments.
     *
     * This function checks if the Functor is a member pointer and invokes the appropriate call
     * to either a member function or a free function/callable object accordingly, while preserving
     * the noexcept guarantee based on the Functor and the arguments provided.
     *
     * @param args The arguments to be passed to the callable object or member function pointer.
     * These arguments are perfect-forwarded to maintain their value category.
     *
     * @return The result of invoking the callable object or member function with the given arguments.
     * The exact return type depends on the type of the Functor being invoked.
     */
    RETROLIB_EXPORT template <auto Functor, typename... A>
        requires HasFunctionCallOperator<decltype(Functor)> || Member<decltype(Functor)>
    constexpr decltype(auto) invoke(A&&... args) noexcept(std::is_nothrow_invocable_v<decltype(Functor), A...>) {
        using F = decltype(Functor);
        if constexpr (Member<std::remove_cvref_t<F>>) {
            return invoke_mem_ptr<Functor>(std::forward<A>(args)...);
        } else {
            return Functor(std::forward<A>(args)...);
        }
    }
}
