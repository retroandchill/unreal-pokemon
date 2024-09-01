// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Concepts/Types.h"
#include "Ranges/RangeConcepts.h"
#include "Ranges/TerminalClosure.h"

THIRD_PARTY_INCLUDES_START
#include <range/v3/range/concepts.hpp>
THIRD_PARTY_INCLUDES_END

namespace UE::Ranges {
    /**
     * Terminal operator for converting a range into a TArray object.
     */
    struct FToArray {

        /**
         * Invocation operation for the closure. Converts the range into an array of the target type.
         * @tparam R The range to pass in.
         * @tparam T The value type of the range
         * @param Range The range to convert into an array.
         * @return The result of the array
         */
        template <typename R, typename T = TRangeValue<R>>
            requires ranges::input_range<R> || UEContainer<T>
        constexpr auto operator()(R &&Range) {
            TArray<T> Ret;

            if constexpr (ranges::sized_range<T>) {
                Ret.Reserve(ranges::size(Range));
            } else if constexpr (UEContainer<T>) {
                Ret.Reserve(Range.Num());
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

    /**
     * Terminal operand for converting a view into an array. When using it on a range with known size it will reserve
     * the size of the range to avoid having to resize the array while iterating.
     */
    constexpr TTerminalClosure<FToArray> ToArray;

} // namespace UE::Ranges