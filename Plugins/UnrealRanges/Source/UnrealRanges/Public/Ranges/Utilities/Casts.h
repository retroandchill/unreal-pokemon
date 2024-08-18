// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

namespace UE::Ranges {

/**
 * Cast the underlying object of an interface into the specified object type. Will return nullptr if the cast is invalid.
 * @tparam T The type to cast to.
 * @param Interface The interface to perform the cast on
 * @return The result of the cast.
 */
template <typename T>
    requires std::is_base_of_v<UObject, T>
T* CastInterface(const FScriptInterface& Interface) {
    return Cast<T>(Interface.GetObject());
}

/**
 * Cast the underlying object of an interface into the specified object type. Will assert that the cast does not result
 * in a null.
 * @tparam T The type to cast to.
 * @param Interface The interface to perform the cast on
 * @return The result of the cast.
 */
template <typename T>
    requires std::is_base_of_v<UObject, T>
T* CastInterfaceChecked(const FScriptInterface& Interface) {
    return CastChecked<T>(Interface.GetObject());
}

}