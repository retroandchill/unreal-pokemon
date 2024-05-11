// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

#include <ranges>

namespace RangeHelpers {

    /**
     * Create a new range from the provided array.
     * @tparam T The type of data the array holds
     * @param View The array view to create the view from.
     * @return The created view
     */
    template <typename T>
    auto CreateRange(const TArray<T> &View) {
        std::span Span(View.GetData(), View.Num());
        return std::views::all(Span);
    }

    template <typename T, typename RangeType>
    TArray<T> ToArray(RangeType Range) {
        TArray<T> Ret;
        for (const auto &Value : Range) {
            Ret.Add(Value);
        }
        return Ret;
    }

    template <typename T>
    struct TToArray {};

    template <typename T, typename RangeType>
    TArray<T> operator|(RangeType Range, TToArray<T>) {
        return ToArray<T, RangeType>(Range);
    }
}