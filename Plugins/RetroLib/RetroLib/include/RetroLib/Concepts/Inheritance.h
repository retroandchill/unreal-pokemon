/**
 * @file Inheritance.h
 * @brief Contains concept definitions for polymorphic classes.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include <type_traits>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro {
    /**
     * A concept that checks if a given type `T` is a class type,
     * excluding union types.
     *
     * This concept combines two type traits from the standard type
     * traits library to determine the eligibility of the type `T`.
     *
     * - `std::is_class_v<T>`: Evaluates to true if `T` is a class type.
     * - `!std::is_union_v<T>`: Evaluates to true if `T` is not a union type.
     *
     * The conjunction of these two conditions ensures that `ClassType`
     * only evaluates to true for class types that are not unions.
     *
     * @tparam T The type to be checked.
     */
    RETROLIB_EXPORT template <typename T>
    concept Class = std::is_class_v<T> && (!std::is_union_v<T>);

    /**
     * @brief Concept to check if a type is a polymorphic class type.
     *
     * This concept checks whether a given type T is a class that has
     * at least one virtual function, indicated by the presence of a
     * virtual destructor. A type satisfying this concept is considered
     * to be polymorphic, meaning it supports dynamic polymorphism
     * through virtual functions.
     *
     * @tparam T The type to be checked.
     */
    RETROLIB_EXPORT template <typename T>
    concept PolymorphicType = std::is_class_v<T> && std::has_virtual_destructor_v<T>;

} // namespace retro
