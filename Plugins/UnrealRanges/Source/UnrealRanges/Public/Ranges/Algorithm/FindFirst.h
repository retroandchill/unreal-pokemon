// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "ForEach.h"
#include "Ranges/Optional/Types.h"

THIRD_PARTY_INCLUDES_START
#include <range/v3/algorithm/find_if.hpp>
THIRD_PARTY_INCLUDES_END

namespace UE::Ranges {

    /**
     * Terminal invoker for finding the first element in a range, returning an empty optional if nothing is found.
     */
    struct FFindFirst {

        /**
         * Check for the first element in the range, returning it if found.
         * @tparam R The type of the range to search.
         * @param Range The range to find the first element from.
         * @return The value of the first element, or empty if the range is empty.
         */
        template <typename R>
            requires ranges::input_range<R>
        auto operator()(R&& Range) {
            using ResultType = Optionals::TOptionalType<ranges::range_common_reference_t<R>>;
            auto Result = Range.begin();
            return Result != Range.end() ? TOptional<ResultType>(*Result) : TOptional<ResultType>();
        }
        
    };

    /**
     * Terminal operand for getting the first element of a range. Will create an Optional to a reference if the
     * range contains pointers.
     */
    constexpr TTerminalClosure<FFindFirst> FindFirst;
    
}