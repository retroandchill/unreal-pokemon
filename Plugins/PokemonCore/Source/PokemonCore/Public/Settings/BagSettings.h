// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "BagSettings.generated.h"

/**
 * Meta info about a pocket in the bag.
 */
USTRUCT(BlueprintType)
struct FPocketInfo {
    GENERATED_BODY()

    /**
     * The name of the pocket as displayed to the player.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Inventory")
    FText DisplayName;

    /**
     * The maximum number of items that can be held within a given pocket.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Inventory", meta = (UIMin = 1, ClampMin = 1))
    TOptional<int32> MaxPocketSize;

    /**
     * Should the pocket be automatically sorted when items are added
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Inventory")
    bool bAutoSort;
};

/**
 * Settings related to the player inventory management.
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Bag Settings"))
class POKEMONCORE_API UBagSettings : public UDeveloperSettings {
    GENERATED_BODY()

  public:
    /**
     * Get the maximum number of items per slot.
     * @return The maximum quantity of a single item the player can hold.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Player|Inventory")
    int32 GetMaxItemsPerSlot() const;

    /**
     * Get important information about the various bag pockets.
     * @return Information about the various Bag Pockets.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Player|Inventory")
    const TMap<FName, FPocketInfo>& GetPocketInfo() const;

  private:
    /**
     * The maximum quantity of a single item the player can hold.
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetMaxItemsPerSlot, Config, Category = "Player|Inventory")
    int32 MaxItemsPerSlot = 999;

    /**
     * Information about the various Bag Pockets.
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetPocketInfo, Config, Category = "Player|Inventory",
        meta = (GetKeyOptions = "PokemonData.ItemHelper.GetPocketNames"))
    TMap<FName, FPocketInfo> PocketInfo;
};
