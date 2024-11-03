// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/RangeConcepts.h"

namespace UE::Ranges {
    /**
     * Get the underlying type held within a specific interface
     *
     * @tparam T The type to get the interface information from.
     */
    template <typename T>
        requires std::derived_from<std::decay_t<T>, FScriptInterface> || UnrealInterface<std::remove_pointer_t<T>>
    using TInterfaceType = std::decay_t<decltype(*std::declval<T>())>;
}
