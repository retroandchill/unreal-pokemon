/**
 * @file WrapArg.h
 * @brief Contains a functor used for wrapping reference params in reference_wrapper.
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
     * Wraps the provided argument in a reference wrapper if it is an lvalue reference,
     * otherwise forwards the argument as is.
     *
     * @param arg The argument to be potentially wrapped or forwarded. The type T
     *            can either be an lvalue reference or a rvalue reference.
     * @return If the argument is an lvalue reference, returns a std::reference_wrapper
     *         around the argument. If it is not an lvalue reference, forwards the
     *         argument as is.
     */
    RETROLIB_EXPORT template <typename T>
    constexpr decltype(auto) wrap_arg(T &&arg) {
        if constexpr (std::is_lvalue_reference_v<T> &&
                      (sizeof(std::decay_t<T>) > sizeof(void *) || !std::is_trivially_copyable_v<std::decay_t<T>>)) {
            return std::reference_wrapper(arg);
        } else {
            return std::forward<T>(arg);
        }
    }
} // namespace retro