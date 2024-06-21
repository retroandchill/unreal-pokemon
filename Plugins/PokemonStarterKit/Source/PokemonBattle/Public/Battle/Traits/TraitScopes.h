// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TraitScopes.generated.h"

UENUM(BlueprintType)
enum class EIndividualTraitScope : uint8 {
    Global,
    User,
    UserAlly,
    Target,
    TargetAlly
};