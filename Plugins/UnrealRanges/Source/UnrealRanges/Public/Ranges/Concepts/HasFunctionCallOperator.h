// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include <array>

namespace UE::Ranges {
    namespace Details {
        template <typename>
        struct THasOneFunctionCallOperator : std::false_type {};

        template <typename T>
            requires std::is_member_function_pointer_v<decltype(&T::operator())>
        struct THasOneFuctionCallOperator : std::true_type {};

        struct FWithOp {
            void operator()() const;
        };

        template <typename T>
        struct TMixin : T, FWithOp {};
    } // namespace Details

    template <typename T>
    concept HasFunctionCallOperator = !Details::THasOneFunctionCallOperator<Details::TMixin<T>>::value;
} // namespace UE::Ranges