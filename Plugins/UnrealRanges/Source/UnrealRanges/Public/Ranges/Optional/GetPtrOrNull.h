// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "OptionalClosure.h"
#include "Traits.h"
#include "Ranges/Functional/Bindings.h"

namespace UE::Optionals {

    struct FGetPtrOrNull {
        template <typename O>
            requires UEOptional<O>
        constexpr auto operator()(O&& Optional) const {
            return Optional.GetPtrOrNull();
        }
    };


    constexpr TOptionalClosure<FGetPtrOrNull> GetPtrOrNull;
    
}