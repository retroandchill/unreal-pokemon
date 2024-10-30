// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

namespace UE::Ranges {

    /**
     * Check to see if the type is an Unreal Engine 5 enumeration type.
     * @tparam T The type to check
     */
    template <typename T>
    concept UEEnum = std::is_enum_v<T> && requires {
        { StaticEnum<T> } -> std::same_as<UEnum*>;
    };
    
}
