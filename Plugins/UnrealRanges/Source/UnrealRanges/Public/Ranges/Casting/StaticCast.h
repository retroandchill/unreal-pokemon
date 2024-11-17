// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

namespace UE::Ranges {
    template <typename T>
    struct TStaticCast {
        template <typename U>
            requires std::is_convertible_v<U, T>
        constexpr T operator()(U&& Value) const {
            return static_cast<T>(std::forward<U>(Value));
        }
    };

    template <typename T>
    constexpr TStaticCast<T> StaticCast;
}
