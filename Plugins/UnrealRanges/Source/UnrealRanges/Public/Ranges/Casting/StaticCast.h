// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

namespace UE::Ranges {
    template <typename T, typename U>
    concept CanStaticCast = requires(T &&Input) {
       { static_cast<U>(std::forward<T>(Input)) } -> std::same_as<U>; 
    };
    
    template <typename T>
    struct TStaticCast {
        template <typename U>
            requires CanStaticCast<U, T>
        constexpr T operator()(U&& Value) const {
            return static_cast<T>(std::forward<U>(Value));
        }
    };

    template <typename T>
    constexpr TStaticCast<T> StaticCast;
}
