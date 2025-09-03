// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "PokemonStorageSystemSettings.generated.h"

/**
 * The settings for the Pokémon storage system.
 */
UCLASS(Config = Game, DefaultConfig, DisplayName = "Pokémon Storage System")
class POKEMONCORE_API UPokemonStorageSystemSettings : public UDeveloperSettings {
    GENERATED_BODY()

  public:
    /**
     * The total number of boxes that the player has available to them.
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = StorageSystem, meta = (ClampMin = 1, UIMin = 1))
    int32 TotalBoxes = 32;

    /**
     * The capacity of a box when it's created.
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = StorageSystem, meta = (ClampMin = 1, UIMin = 1))
    int32 BoxCapacity = 30;

    /**
     * The capacity of a box when it's created.
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = StorageSystem, meta = (ClampMin = 1, UIMin = 1))
    FText BoxNameFormat = NSLOCTEXT("PokemonCore", "BoxNameFormat", "Box {Index}");
};
