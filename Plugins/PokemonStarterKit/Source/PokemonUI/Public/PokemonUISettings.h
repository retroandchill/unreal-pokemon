// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "PokemonUISettings.generated.h"

/**
 * Settings for all the Pokémon UI related windows.
 */
UCLASS(Config = Game, DefaultConfig, DisplayName = "Pokémon UI")
class POKEMONUI_API UPokemonUISettings : public UDeveloperSettings {
    GENERATED_BODY()

  public:
    /**
     * The screen displayed when showing a messages
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = UI, meta = (MetaClass = "TextDisplayScreen"))
    FSoftClassPath TextScreenClass;

    /**
     * The screen displayed when showing the party screen
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = UI, meta = (MetaClass = "PokemonSelectScreen"))
    FSoftClassPath PartyScreenClass;

    /**
     * The screen displayed when showing the party screen
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = UI, meta = (MetaClass = "BagScreen"))
    FSoftClassPath BagScreenClass;

    /**
     * The screen displayed when showing the party screen
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = UI, meta = (MetaClass = "MoveForgetScreen"))
    FSoftClassPath MoveForgetScreenClass;
};
