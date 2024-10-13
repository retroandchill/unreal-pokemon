// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include <vector>

namespace UE::Ranges {

    template <typename T, typename N>
    concept Indexable = requires(T Container, N Index) { Container[Index]; };

    static_assert(Indexable<int32[5], size_t>);
    static_assert(Indexable<TArray<int32>, int32>);

} // namespace UE::Ranges
