// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Service.generated.h"

/**
 * Base class for all injectable service objects created by the system.
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class UNREALINJECTOR_API UService : public UObject {
    GENERATED_BODY()


};
