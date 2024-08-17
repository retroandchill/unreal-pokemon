// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

namespace UE::Ranges {

template <typename T>
    requires std::is_base_of_v<UObject, T>
T* CastInterface(const FScriptInterface& Interface) {
    return Cast<T>(Interface.GetObject());
}

template <typename T>
    requires std::is_base_of_v<UObject, T>
T* CastInterfaceChecked(const FScriptInterface& Interface) {
    return CastChecked<T>(Interface.GetObject());
}

}