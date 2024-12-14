/**
 * @file InstanceOf.h
 * @brief Functional object for checking if something is an instance of another object.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/Concepts/Inheritance.h"
#include "RetroLib/Utils/Polymorphic.h"
#include "RetroLib/Utils/ValidPtr.h"

#include <type_traits>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace retro {

    /**
     * Checks if the given instance of type U is a valid instance of the desired base type T.
     *
     * @param value The instance of type U to be checked for validity.
     * @return True if the instance is valid, meaning that U is derived from T or is an instance
     *         that can be dynamically cast to T. Otherwise, returns false.
     */
    RETROLIB_EXPORT template <Class T, Class U>
    constexpr bool is_valid_instance(const U &value) {
        if constexpr (std::derived_from<U, T>) {
            // Trivial case, U is derived from T, so we know with certainty that this is valid
            return true;
        } else {
#ifndef RTTI_ENABLED
            static_assert(false, "RTTI is disabled, but the type is not derived from T")
#endif
                auto casted = dynamic_cast<const T *>(&value);
            return casted != nullptr;
        }
    }

    /**
     * @struct InstanceOf
     * @brief A utility to check if an object or a pointer is a valid instance of a specified type.
     *
     * The InstanceOf struct provides overloaded function call operators to determine if a given
     * object or pointer is a valid instance of a specified type using compile-time checks.
     *
     * @tparam T The type against which the object's type is being checked.
     */
    template <Class T>
    struct InstanceOf {
        /**
         * Checks if the given value is a valid instance of type U.
         *
         * @param value The value to be checked against the type U.
         * @return A boolean indicating whether the value is a valid instance of type U.
         */
        template <Class U>
        constexpr bool operator()(const U &value) const {
            return is_valid_instance<T, U>(value);
        }

        /**
         * @brief Checks the validity of a pointer and applies the functor to its dereferenced value.
         *
         * This function call operator evaluates whether the given pointer is both valid according
         * to the `valid_ptr` function and whether the functor applied to the value pointed to by
         * the pointer is true.
         *
         * @param ptr Pointer to the object that needs to be checked.
         * @return true if the pointer is valid and the functor evaluates to true for the
         * dereferenced value, otherwise false.
         */
        template <PointerType U>
        constexpr bool operator()(U &&ptr) const {
            return valid_ptr(std::forward<U>(ptr)) && (*this)(*std::forward<U>(ptr));
        }

        /**
         * Operator function for evaluating a polymorphic object.
         *
         * This function is a callable operator that takes a polymorphic object
         * and applies this operator to its dereferenced value.
         *
         * @param polymorphic A Polymorphic object which contains a type U of
         *        a certain size. The object will be dereferenced and the
         *        result will be passed to the operator function.
         * @return A boolean value indicating the result of applying the operator
         *         to the dereferenced value of the polymorphic object.
         */
        template <Class U, size_t Size>
        constexpr bool operator()(const Polymorphic<U, Size> &polymorphic) const {
            return (*this)(*polymorphic);
        }

        /**
         * Determines the boolean value for a nullptr.
         *
         * @param A parameter of type std::nullptr_t representing a null pointer.
         * @return Always returns false as null pointers are considered false.
         */
        constexpr bool operator()(std::nullptr_t) const {
            return false;
        }
    };

    /**
     * @brief A constexpr instance of the `InstanceOf` template for a given type `T`.
     *
     * This variable acts as a compile-time instance representing the type `T` in
     * the `InstanceOf` type system. It can be used for compile-time type checks,
     * meta-programming, or any operation that benefits from type reflection or
     * identification within the constraints of the `InstanceOf` framework.
     *
     * @tparam T The type for which the instance is created.
     *
     * The `instance_of` variable is intended to be used where a constant representation
     * of a type is needed without instantiation at runtime. This can be particularly
     * useful in template metaprogramming and scenarios where type safety and checking
     * are performed at compile-time.
     */
    RETROLIB_EXPORT template <Class T>
    constexpr InstanceOf<T> instance_of;
} // namespace retro