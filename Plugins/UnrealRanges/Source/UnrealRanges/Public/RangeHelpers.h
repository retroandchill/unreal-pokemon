﻿// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "range/v3/view/all.hpp"
#include "range/v3/view/span.hpp"
#include <range/v3/view/filter.hpp>
#include <range/v3/view/single.hpp>
#include <range/v3/view/transform.hpp>

namespace RangeHelpers {

/**
 * Create a new range from the provided array.
 * @tparam T The type of data the array holds
 * @param View The array view to create the view from.
 * @return The created view
 */
template <typename T, typename Alloc>
auto CreateRange(const TArray<T, Alloc> &View) {
    ranges::span Span(View.GetData(), View.Num());
    return ranges::views::all(Span);
}

/**
 * Create a new range from the provided array.
 * @tparam T The type of data the array holds
 * @param View The array view to create the view from.
 * @return The created view
 */
template <typename T>
auto CreateRange(TConstArrayView<T> &View) {
    ranges::span Span(View.GetData(), View.Num());
    return ranges::views::all(Span);
}

/**
 * Convert the elements in the range into an array
 * @tparam T The type of data the array will hold
 * @tparam RangeType The input range type
 * @param Range The range to process
 * @return The range values as an array
 */
template <typename T, typename RangeType>
    requires std::is_copy_constructible_v<RangeType>
TArray<T> ToArray(RangeType Range) {
    TArray<T> Ret;
    for (const auto &Value : Range) {
        Ret.Emplace(Value);
    }
    return Ret;
}

/**
 * Convert the elements in the range into an array
 * @tparam T The type of data the array will hold
 * @tparam RangeType The input range type
 * @param Range The range to process
 * @return The range values as an array
 */
template <typename T, typename RangeType>
    requires(!std::is_copy_constructible_v<RangeType>)
TArray<T> ToArray(RangeType &Range) {
    TArray<T> Ret;
    for (auto &&Value : Range) {
        Ret.Emplace(Forward(Value));
    }
    return Ret;
}

/**
 * Converter object used to transform a range into an array.
 * @tparam T The type of data the array will hold
 */
template <typename T>
struct TToArray {

    /**
     * Operator overload so that converting to an Unreal data type can be chained into the ranges stream operator
     * @tparam T The type of data the array will hold
     * @tparam RangeType The input range type
     * @param Range The range to process
     * @return The range values as an array
     */
    template <typename RangeType>
    friend TArray<T> operator|(RangeType Range, TToArray) {
        return ToArray<T, RangeType>(Range);
    }
};

} // namespace RangeHelpers