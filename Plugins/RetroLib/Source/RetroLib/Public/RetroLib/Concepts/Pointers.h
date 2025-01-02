/**
 * @file Pointers.h
 * @brief Pointer related concepts
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#include "RetroLib/Optionals/OptionalOperations.h"

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro {
    /**
     * Checks if the given type is a pointer type.
     *
     * @tparam T The type to check
     */
    template <typename T>
    concept PointerType = Dereferenceable<T> && !Optionals::OptionalType<T> && requires(T &&ptr) {
        { std::forward<T>(ptr) == nullptr } -> std::same_as<bool>;
        { std::forward<T>(ptr) != nullptr } -> std::same_as<bool>;
    };
} // namespace retro