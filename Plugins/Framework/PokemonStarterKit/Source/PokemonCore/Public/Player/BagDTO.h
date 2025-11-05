// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "ItemSlot.h"
#include "UObject/Object.h"

#include "BagDTO.generated.h"

/**
 * Wrapper around the items in a pocket in the game.
 */
USTRUCT(BlueprintType)
struct POKEMONCORE_API FPocket {
    GENERATED_BODY()

    /**
     * The items that have been placed inside that pocket.
     */
    UPROPERTY(SaveGame)
    TArray<FItemSlot> Items;
};

/**
 *
 */
USTRUCT(BlueprintType)
struct POKEMONCORE_API FBagDTO {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data)
    TMap<FName, FPocket> ItemSlots;
};
