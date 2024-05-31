// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "BagSettings.generated.h"

/**
 * Settings related to the player inventory management.
 */
UCLASS()
class POKEMONCORE_API UBagSettings : public UDeveloperSettings {
    GENERATED_BODY()

  public:
    /**
     * Get the maximum number of items per slot.
     * @return The maximum quantity of a single item the player can hold.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Player|Inventory")
    int32 GetMaxItemsPerSlot() const;

  private:
    /**
     * The maximum quantity of a single item the player can hold.
     */
    UPROPERTY(BlueprintGetter = GetMaxItemsPerSlot, Category = "Player|Inventory")
    int32 MaxItemsPerSlot = 999;
};
