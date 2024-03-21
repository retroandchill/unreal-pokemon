// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

/**
 * Enumeration for the various type of possible interactivity for the object.
 */
UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EInteractionType : uint8 {
 /**
  * No interaction types
  */
 None = 0 UMETA(Hidden),
 /**
  * The object must be talked to be interacted with
  */
 Talk = 1 << 1,

 /**
  * The object must be hit to be interacted with
  */
 Hit = 1 << 2
};

ENUM_CLASS_FLAGS(EInteractionType)