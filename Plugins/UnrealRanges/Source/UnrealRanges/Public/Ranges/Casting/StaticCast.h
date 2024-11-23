// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

namespace UE::Ranges {
    /**
     * Checks if you can cast from T to U.
     *
     * @tparam T The type of the source parameter
     * @tparam U The target type
     */
    template <typename T, typename U>
    concept CanStaticCast = requires(T &&Input) {
        { static_cast<U>(std::forward<T>(Input)) } -> std::same_as<U>;
    };

    /**
     * @brief A functor struct to perform static_cast on a given value.
     *
     * This struct provides a callable object that casts the passed value to the specified type T using static_cast.
     *
     * @tparam T The target type to cast to.
     */
    template <typename T>
    struct TStaticCast {
        /**
         * @brief Performs a static_cast on the given value.
         *
         * This operator casts the input value to the specified type T using static_cast.
         *
         * @param Value The value to be casted to type T.
         * @return The value casted to type T.
         */
        template <typename U>
            requires CanStaticCast<U, T>
        constexpr T operator()(U &&Value) const {
            return static_cast<T>(std::forward<U>(Value));
        }
    };

    /**
     * @brief A static constant instance of TStaticCast.
     *
     * This constant provides a convenient way to perform a `static_cast` on values to the specified type T
     * using the callable `TStaticCast` object.
     *
     * @tparam T The target type to cast to.
     */
    template <typename T>
    constexpr TStaticCast<T> StaticCast;
} // namespace UE::Ranges
