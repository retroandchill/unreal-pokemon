// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "OptionalClosure.h"
#include "Ranges/Functional/Bindings.h"
#include "Traits.h"

namespace UE::Optionals {

    struct FGetValue {
        template <typename O>
            requires UEOptional<O>
        constexpr decltype(auto) operator()(O &&Optional) const {
            return std::forward<O>(Optional).GetValue();
        }
    };

    constexpr TOptionalClosure<FGetValue> GetValue;

} // namespace UE::Optionals