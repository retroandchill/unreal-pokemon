﻿// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Bag/Item.h"

#include "ItemSlot.generated.h"

/**
 * Struct for a single item in the player's inventory.
 */
USTRUCT(BlueprintType)
struct POKEMONCORE_API FItemSlot {
    GENERATED_BODY()

    /**
     * The ID of the item that is in this slot.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Player|Inventory")
    FName Item;

    /**
     * The number of items that are possessed
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = "Player|Inventory")
    int32 Quantity = 1;

    const FItem &GetItem() const;
};
