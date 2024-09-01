// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

namespace UE::Ranges {
    /**
     * Concept for any USTRUCT in the editor.
     */
    template <typename T>
    concept UEStruct = requires(T&& Struct) {
        { std::remove_cvref_t<T>::StaticStruct() } -> std::same_as<UScriptStruct*>;
    };
}
