// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "NamingSettings.generated.h"

/**
 * Settings related to the naming of things when they are not set.
 */
UCLASS(Config = Game, DefaultConfig)
class POKEMONCORE_API UNamingSettings : public UDeveloperSettings {
    GENERATED_BODY()

  public:
    /**
     * The text displayed for the name when a Pokémon has no ability.
     * @return The text displayed for the name when a Pokémon has no ability.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Pokémon|Abilities")
    FText GetNoAbilityName() const;

    /**
     * The text displayed for the description when a Pokémon has no ability.
     * @return The text displayed for the description when a Pokémon has no ability.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Pokémon|Abilities")
    FText GetNoAbilityDescription() const;

  private:
    /**
     * The text displayed for the name when a Pokémon has no ability.
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetNoAbilityName, Config, Category = "Pokémon|Abilities")
    FText NoAbilityName;

    /**
     * The text displayed for the description when a Pokémon has no ability.
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetNoAbilityDescription, Config, Category = "Pokémon|Abilities")
    FText NoAbilityDescription;
};
