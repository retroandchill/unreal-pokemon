/**
 * @file ForwardLike.h
 * @brief Contains the definition of C++23's forward_like function.
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

namespace retro {
    /**
     * @brief Forwards the given argument with type preservation analogous to std::forward,
     *        emulating the cv-qualifiers of a specified type.
     *
     * This function template conditionally casts and forwards an argument in a manner
     * that emulates the reference and const qualifications of another specified type `T`.
     *
     * @tparam T The type whose cv-qualifications are to be emulated.
     * @tparam U The type of the argument being forwarded.
     * @param x The argument to be conditionally forwarded.
     * @return The forwarded argument, potentially with const qualification and/or
     *         moved semantics based on the cv-qualifications of type `T`.
     *
     * @note The behavior of the function mimics the effects of std::forward but extends
     *       it by adding the ability to replicate cv-qualifiers from a template type `T`.
     */
    RETROLIB_EXPORT template <class T, class U>
    constexpr auto &&forward_like(U &&x) noexcept {
        constexpr bool is_adding_const = std::is_const_v<std::remove_reference_t<T>>;
        if constexpr (std::is_lvalue_reference_v<T &&>) {
            if constexpr (is_adding_const) {
                return std::as_const(x);
            } else {
                return static_cast<U &>(x);
            }
        } else {
            if constexpr (is_adding_const) {
                return std::move(std::as_const(x));
            } else {
                return std::move(x);
            }
        }
    }
} // namespace retro
