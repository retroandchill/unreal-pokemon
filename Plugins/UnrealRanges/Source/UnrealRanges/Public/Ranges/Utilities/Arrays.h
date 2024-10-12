// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Concepts/Arrays.h"

namespace UE::Ranges {
    template <typename T, typename N, typename R = decltype(std::declval<T>()[std::declval<N>()])>
        requires Indexable<T, N>
    constexpr auto GetWithIndex(T&& Array, N Index) {
        return TPair<N, R>(Index, Array[Index]);
    }
}
