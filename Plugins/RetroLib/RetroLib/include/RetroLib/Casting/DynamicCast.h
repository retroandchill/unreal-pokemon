/**
 * @file DynamicCast.h
 * @brief Functor for performing runtime conversions between types.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/Concepts/Inheritance.h"
#include "RetroLib/Optionals/Optional.h"
#include "RetroLib/Utils/Polymorphic.h"
#include "RetroLib/Utils/ValidPtr.h"

#include <type_traits>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro {

    /**
     * Attempts to dynamically cast a reference of type U to a reference of type T.
     * If U is a base of T, a static cast is performed. Otherwise, a dynamic cast is attempted.
     * If Checked is true, an assertion is made to ensure the cast pointer is valid.
     * If the cast is possible, the result is returned as a reference to T.
     * If the cast is not possible and Checked is false, an Optional reference to T is returned.
     *
     * @tparam T The target type to which the reference should be cast.
     * @tparam U The source type of the reference that should be cast.
     * @tparam Checked A boolean flag indicating whether the operation should assert the validity of the cast.
     *
     * @param value The reference to the object of type U that is to be cast.
     * @return A reference of type T if the cast is successful. If Checked is false and the cast is unsuccessful, an
     * Optional reference to T is returned.
     */
    RETROLIB_EXPORT template <Class T, Class U, bool Checked = false>
    constexpr decltype(auto) DynCastRef(U &Value) {
        if constexpr (std::is_base_of_v<T, U>) {
            return static_cast<T &>(Value);
        } else {
            auto Ptr = dynamic_cast<T *>(&Value);
            if constexpr (Checked) {
                RETROLIB_ASSERT(valid_ptr(Ptr));
                return dynamic_cast<T &>(Value);
            } else {
                return Ptr != nullptr ? Optional<T &>(*Ptr) : Optional<T &>();
            }
        }
    }

    /**
     * @brief ClassCast is a utility structure for performing dynamic casting and validation of references and pointers.
     *
     * The ClassCast utility provides functionality for safely casting between class types and validating the integrity
     * of pointers and references at runtime. Casting is performed with optional checking, ensuring safety and
     * correctness when converting between types.
     *
     * @tparam T The target type to which the value will be dynamically casted.
     * @tparam Checked A boolean template parameter to indicate whether to perform runtime checks during casting.
     */
    template <Class T, bool Checked = false>
    struct DynamicCastFunction {
        /**
         * Overloaded function call operator which performs a dynamic cast of a reference type.
         * This function attempts to cast a reference of type U to a reference of type T.
         * If the Checked parameter is considered, it might involve runtime checks for safety.
         *D
         * @param Value A reference to a value of type U that is to be casted.
         * @return The result of the dynamic cast to type T. The return is of the same reference type,
         * which is determined by the use of `decltype(auto)`.
         */
        template <Class U>
        constexpr decltype(auto) operator()(U &Value) {
            return DynCastRef<T, U, Checked>(Value);
        }

        /**
         * Operator that performs a conditional transformation or validity check on the provided pointer-like value.
         *
         * This operator function checks if the provided value is a valid pointer, and depending on compile-time
         * conditions, it performs a dynamic cast or derives reference conversion.
         *
         * @param Value A universal reference to a value that will be checked for validity and conditionally
         * transformed. This can be a pointer or a reference to an object.
         *
         * @return An Optional containing a reference to the object of type T if the pointer is valid and the
         * cast/conversion succeeds. If the pointer is invalid, an empty Optional is returned.
         */
        template <PointerType U>
        constexpr decltype(auto) operator()(U &&Value) const {
            if constexpr (Checked) {
                RETROLIB_ASSERT(valid_ptr(std::forward<U>(Value)));
                return DynCastRef<T, std::remove_pointer_t<U>, Checked>(*std::forward<U>(Value));
            } else if constexpr (std::derived_from<std::decay_t<DereferencedType<U>>, std::decay_t<T>>) {
                return ValidPtr(std::forward<U>(Value)) ? Optional<T &>(*std::forward<U>(Value)) : Optional<T &>();
            } else {
                if (!ValidPtr(std::forward<U>(Value))) {
                    return Optional<T &>();
                }

                return DynCastRef<T, std::decay_t<DereferencedType<U>>, Checked>(*std::forward<U>(Value));
            }
        }

        /**
         * This operator function call overload provides a way to call the functor with a reference
         * to a Polymorphic object, effectively de-referencing it and invoking the functor on the
         * underlying object that the Polymorphic reference points to.
         *
         * @param Value A reference to a Polymorphic<U> object, where U must be a type that supports
         *              the operations defined within the functor.
         * @return The result of invoking the functor on the de-referenced object. The return type is
         *         deduced using decltype(auto), meaning it will preserve the value category and type
         *         of the final operation's result.
         */
        template <Class U>
        constexpr decltype(auto) operator()(Polymorphic<U> &Value) const {
            return (*this)(*Value);
        }

        /**
         * Function call operator to apply the functor on a polymorphic value.
         *
         * This operator allows the functor to be called on a given polymorphic object,
         * effectively dereferencing it and calling the functor with its underlying value.
         *
         * @param Value A constant reference to a Polymorphic object containing the value.
         * @return The result of applying the functor to the dereferenced value stored in the Polymorphic object.
         */
        template <Class U>
        constexpr decltype(auto) operator()(const Polymorphic<U> &Value) const {
            return (*this)(*Value);
        }

        constexpr Optional<T &> operator()(std::nullptr_t) const
            requires(!Checked)
        {
            return Optional<T &>();
        }
    };

    /**
     * @variable class_cast
     * @brief A compile-time constant representing a type-casting operation.
     *
     * This variable provides a mechanism for casting between different
     * class types in a type-safe manner using template metaprogramming.
     * The cast is performed at compile-time, ensuring no runtime overhead.
     * It is intended for use in scenarios where explicit class type conversion
     * is required, while maintaining strict type-checking and safety.
     *
     * @tparam T The target class type to which the casting is to be performed.
     */
    RETROLIB_EXPORT template <Class T>
    constexpr DynamicCastFunction<T> DynamicCast;

    /**
     * @brief A compile-time constant expression representing a checked class cast operation.
     *
     * This variable is part of the ClassCast mechanism utilized for safe casting of objects
     * between classes in a type-safe manner at compile time. The `true` template parameter
     * indicates that this version of class casting will perform an assertion to ensure the cast
     * is valid during runtime.
     *
     * @tparam T The type to which the object will be cast. This template parameter should
     *           be specified to indicate the destination class type for the casting operation.
     */
    RETROLIB_EXPORT template <Class T>
    constexpr DynamicCastFunction<T, true> ClassCastChecked;

} // namespace retro