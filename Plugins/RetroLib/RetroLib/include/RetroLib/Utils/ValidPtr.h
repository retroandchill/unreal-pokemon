/**
 * @file ValidPtr.h
 * @brief Check function to see if a type is a valid pointer.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/Concepts/Pointers.h"
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro {

    /**
     * @brief Checks if a given pointer is valid (non-null).
     *
     * This function takes a pointer and checks whether it is valid by determining if
     * it is not equal to `nullptr`.
     *
     * @param Ptr A pointer of any type that will be checked for validity.
     *
     * @return True if the pointer is valid (not null), otherwise false.
     */
    RETROLIB_EXPORT template <PointerType T>
    constexpr bool IsValidPtr(T &&Ptr) {
        return std::forward<T>(Ptr) != nullptr;
    }

    /**
     * @struct ValidPtrFunction
     * @brief A functor for checking the validity of a pointer.
     *
     * The ValidPtr struct provides a function call operator that determines
     * if a given pointer is valid, utilizing the is_valid_ptr function.
     */
    struct ValidPtrFunction {
        template <PointerType T>
        constexpr bool operator()(T &&Ptr) const {
            return IsValidPtr(std::forward<T>(Ptr));
        }

        /**
         * @brief Checks the validity of a nullptr.
         *
         * This constexpr function call operator always returns false
         * when invoked with a std::nullptr_t, indicating that a nullptr
         * is not considered a valid pointer.
         *
         * @param ptr A nullptr to be checked.
         * @return Returns false as nullptr is always considered invalid.
         */
        constexpr bool operator()(std::nullptr_t) const {
            return false;
        }
    };

    /**
     * @brief A constant instance of the ValidPtr functor.
     *
     * The constexpr variable valid_ptr serves as a constant instance of the
     * ValidPtr struct, enabling the checking for pointer validity within
     * the scope it is utilized.
     */
    RETROLIB_EXPORT constexpr ValidPtrFunction ValidPtr;

    /**
     * @struct InvalidPtrFunction
     * @brief A functor for checking the invalidity of a pointer.
     *
     * The InvalidPtr struct provides a function call operator that determines
     * if a given pointer is invalid, utilizing the is_valid_ptr function by
     * negating its result.
     */
    struct InvalidPtrFunction {
        template <PointerType T>
        /**
         * @brief Function call operator that checks the invalidity of a pointer.
         *
         * This operator utilizes the is_valid_ptr function to determine if a given
         * pointer is invalid by negating the result of is_valid_ptr.
         *
         * @param ptr The pointer to be checked for invalidity. It is a forwarding
         * reference to handle both lvalue and rvalue pointers.
         * @return Returns true if the pointer is invalid, false otherwise.
         */
        constexpr bool operator()(T &&ptr) const {
            return !IsValidPtr(std::forward<T>(ptr));
        }

        /**
         * @brief Evaluates whether a null pointer is considered valid.
         *
         * This operator function is specifically designed to evaluate
         * a nullptr, returning true to indicate that a null pointer is
         * inherently valid.
         *
         * @param nullptr_t A null pointer of type std::nullptr_t.
         * @return A boolean value indicating the validity of the null pointer, always true.
         */
        constexpr bool operator()(std::nullptr_t) const {
            return true;
        }
    };

    /**
     * @brief A functor for checking the invalidity of a pointer.
     *
     * The InvalidPtr struct provides a function call operator that determines
     * if a given pointer is invalid, usually utilized in conjunction with pointer checks.
     * The constant `invalid_ptr` is an instance of this struct, readily available
     * for use where pointer invalidity needs to be assessed.
     */
    RETROLIB_EXPORT constexpr InvalidPtrFunction InvalidPtr;

} // namespace retro
