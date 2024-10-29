// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

namespace UE::Ranges {
    template <typename T>
    concept UEEnum = std::is_enum_v<T> && requires {
        { StaticEnum<T>() } -> std::same_as<UEnum*>;
    };
}
