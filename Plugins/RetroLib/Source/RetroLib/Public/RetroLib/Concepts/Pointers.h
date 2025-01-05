/**
 * @file Pointers.h
 * @brief Pointer related concepts
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#include "RetroLib/Optionals/OptionalOperations.h"
#include "RetroLib/Utils/Unreachable.h"

#if !RETROLIB_WITH_MODULES
#include "Templates/NonNullSubclassOf.h"
#include <concepts>
#endif

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

    RETROLIB_EXPORT template <typename T>
    struct TPointerGetter {
        static constexpr T *Get(T &Value) {
            return &Value;
        }

        static constexpr const T *Get(const T &Value) {
            return &Value;
        }

        static constexpr const T *Get(T &&Value) {
            static_assert(false, "Can't apply to an r-value reference");
            Unreachable();
        }
    };

#ifdef __UNREAL__
    RETROLIB_EXPORT template <typename T>
        requires std::derived_from<T, UObject>
    struct TPointerGetter<TNonNullSubclassOf<T>> {
        static constexpr TSubclassOf<T> Get(const TNonNullSubclassOf<T> &Value) {
            return Value;
        }
    };
#endif

    RETROLIB_EXPORT template <typename T>
    constexpr auto GetPtrFromRef(T &&Ref) {
        return TPointerGetter<std::remove_cvref_t<T>>::Get(Ref);
    }
} // namespace Retro