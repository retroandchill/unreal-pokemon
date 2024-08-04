// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Pokemon/PokemonDTO.h"
#include "UObject/Object.h"
#include "TrainerDTO.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct POKEMONCORE_API FTrainerDTO {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
    FGuid InternalID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
    FName TrainerType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
    FText Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
    TArray<FPokemonDTO> Party;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
    int32 ID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
    int32 SecretID;

};
