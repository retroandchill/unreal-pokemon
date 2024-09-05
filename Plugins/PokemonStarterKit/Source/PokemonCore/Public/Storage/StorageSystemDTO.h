// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "StorageBoxDTO.h"
#include "UObject/Object.h"
#include "StorageSystemDTO.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct POKEMONCORE_API FStorageSystemDTO {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    TArray<FStorageBoxDTO> Boxes;

    UPROPERTY(EditAnywhere)
    int32 CurrentBoxIndex = 0;

};
