// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "OptionalClosure.h"
#include "Ranges/Functional/Bindings.h"
#include "Traits.h"

namespace UE::Optionals {

    struct FGetPtrOrNull {
        template <typename O>
            requires UEOptional<O>
        constexpr auto operator()(O &&Optional) const {
            if constexpr (SubclassOptional<O>) {
                return Optional.Get(nullptr);
            } else {
                return Optional.GetPtrOrNull();
            }
        }
    };

    constexpr TOptionalClosure<FGetPtrOrNull> GetPtrOrNull;

} // namespace UE::Optionals