// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "BagSettings.generated.h"

/**
 * Settings related to the player inventory management.
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Bag Settings"))
class POKEMONCORE_API UBagSettings : public UDeveloperSettings {
    GENERATED_BODY()

  public:
    
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Player|Inventory")
    const FSoftClassPath &GetItemUtilitiesClass() const;

  private:
    UPROPERTY(EditAnywhere, BlueprintGetter = GetItemUtilitiesClass, Config, Category = "Player|Inventory",
              meta = (MustImplement = "ItemUtilities"))
    FSoftClassPath ItemUtilitiesClass;
};
