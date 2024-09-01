// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/TerminalClosure.h"

THIRD_PARTY_INCLUDES_START
#include <range/v3/range/concepts.hpp>
THIRD_PARTY_INCLUDES_END

namespace UE::Ranges {
    /**
     * Terminal operator for converting a range into a TSet object.
     */
    struct FToSet {

        /**
         * Invocation operation for the closure. Converts the range into an array of the target type.
         * @tparam R The range to pass in.
         * @tparam T The value type of the range
         * @param Range The range to convert into an array.
         * @return The result of the array
         */
        template <typename R, typename T = ranges::range_value_t<R>>
            requires ranges::input_range<R> || UEContainer<T>
        constexpr auto operator()(R &&Range) {
            TSet<T> Ret;

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
     * Terminal operand for converting a view into a set.
     */
    constexpr TTerminalClosure<FToSet> ToSet;

} // namespace UE::Ranges