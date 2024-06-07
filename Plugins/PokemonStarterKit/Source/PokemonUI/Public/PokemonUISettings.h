// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "PokemonUISettings.generated.h"

class UTextDisplayScreen;

/**
 * Global configuration settings for the Pokémon UI classes
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Pokémon UI Settings"))
class POKEMONUI_API UPokemonUISettings : public UDeveloperSettings {
    GENERATED_BODY()

  public:
    /**
     * Get the screen class use for the text screen
     * @return The screen class use for the text screen
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Menus")
    const TArray<TSubclassOf<UTextDisplayScreen>> &GetTextScreenClasses() const;

  private:
    /**
     * The screen class use for the text screen
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintGetter = GetTextScreenClasses, Category = "Menus")
    TArray<TSubclassOf<UTextDisplayScreen>> TextScreenClasses;
};
