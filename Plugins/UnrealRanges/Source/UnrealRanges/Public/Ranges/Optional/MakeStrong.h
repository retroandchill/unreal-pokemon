// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Map.h"
#include "Ranges/Concepts/WeakReferencable.h"

namespace UE::Optionals {

    struct FMakeStrong {

        template <typename O, typename T = TContainedOptionalType<O>>
            requires UEOptional<O> && Ranges::WeakReference<T>
        auto operator()(O &&Optional) const {
            return Optional | Map([](T Ptr) { return Ranges::ToStrongRef<T>(Ptr); });
        }
    };

    /**
     * Extract a weakly referenceable type (mainly a shared pointer or a UObject pointer) from the weak version of
     * that pointer type.
     */
    constexpr TOptionalClosure<FMakeStrong> MakeStrong;

} // namespace UE::Ranges