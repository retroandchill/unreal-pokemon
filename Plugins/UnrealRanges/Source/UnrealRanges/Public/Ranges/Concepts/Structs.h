﻿// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

namespace UE::Ranges {
    template <typename T>
    concept CoreStructType = requires {
        { TBaseStructure<std::remove_cvref_t<T>>::Get() } -> std::same_as<UScriptStruct *>;
    };

    /**
     * Concept for any USTRUCT in the editor.
     */
    template <typename T>
    concept DeclaredStruct = requires {
        { std::remove_cvref_t<T>::StaticStruct() } -> std::same_as<UScriptStruct *>;
    };

    template <typename T>
    concept UEStruct = CoreStructType<T> || DeclaredStruct<T>;

    template <typename T>
        requires UEStruct<T>
    constexpr UScriptStruct *GetScriptStruct() {
        if constexpr (UE::Ranges::DeclaredStruct<T>) {
            return T::StaticStruct();
        } else {
            return TBaseStructure<T>::Get();
        }
    }
} // namespace UE::Ranges
