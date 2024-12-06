/**
 * @file Pointers.h
 * @brief Pointer related concepts
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/Optionals/OptionalOperations.h"
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace retro {
    /**
     * Checks if the given type is a pointer type.
     *
     * @tparam T The type to check
     */
    template <typename T>
    concept PointerType = Dereferenceable<T> && !optionals::OptionalType<T> && requires(T&& ptr) {
        { std::forward<T>(ptr) == nullptr } -> std::same_as<bool>;
        { std::forward<T>(ptr) != nullptr } -> std::same_as<bool>;
    };
}