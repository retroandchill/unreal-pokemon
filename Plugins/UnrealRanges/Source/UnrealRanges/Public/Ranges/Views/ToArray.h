// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

namespace UE::Ranges {

struct FToArray {

    template <typename R, typename T = ranges::range_value_t<R>>
        requires ranges::input_range<R>
    friend auto operator|(R &&Range, const FToArray&) {
        TArray<T> Ret;
    
        if constexpr (ranges::sized_range<T>) {
            Ret.Reserve(ranges::size(Range));
        }

        if constexpr (std::movable<T>) {
            for (auto &&Value : Range) {
                Ret.Emplace(MoveTempIfPossible(Value));
            }
        } else {
            for (const auto &Value : Range) {
                Ret.Emplace(Value);
            }
        }
    
        return Ret;
    }
};

constexpr FToArray ToArray;

}
