// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

namespace UE::Ranges {
    template <typename T, typename F>
    concept CanApply = requires(T&& Tuple, F&& Functor) {
        std::apply(Forward<F>(Functor), Forward<T>(Tuple));
    };
}
