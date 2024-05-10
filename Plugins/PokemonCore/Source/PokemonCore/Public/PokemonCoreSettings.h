// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Pokemon/Abilities/DefaultAbilityBlock.h"
#include "Pokemon/GamePokemon.h"
#include "Pokemon/Moves/DefaultMoveBlock.h"
#include "Pokemon/Stats/DefaultStatBlock.h"

#include "PokemonCoreSettings.generated.h"

/**
 * Configuration class for the core Pokémon data
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Pokémon Core Settings"))
class POKEMONCORE_API UPokemonCoreSettings : public UDeveloperSettings {
    GENERATED_BODY()

  public:
    /**
     * The the ID of the HP stat
     * @return The stat used referring to a Pokémon's HP
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Display Names")
    FName GetHPStat() const;

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Display Names")
    FText GetNoAbilityText() const;

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Display Names")
    FText GetNoAbilityDescription() const;

    /**
     * The maximum number of Pokémon a trainer can carry at any given time
     * @return The maximum number of Pokémon a trainer can carry at any given time
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Party)
    int32 GetMaxPartySize() const;

    /**
     * The maximum number of moves a Pokémon can know.
     * @return The maximum number of moves a Pokémon can know.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Party)
    int32 GetMaxMoves() const;

    /**
     * The class used for all Pokémon objects
     * @return The class used for all Pokémon objects
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Data Classes")
    TSubclassOf<UObject> GetPokemonClass() const;

    /**
     * The class used for all Stat Block objects
     * @return The class used for all Stat Block objects
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Data Classes")
    TSubclassOf<UObject> GetStatBlockClass() const;

    /**
     * The class used for all Move Block objects
     * @return The class used for all Move Block objects
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Data Classes")
    TSubclassOf<UObject> GetMoveBlockClass() const;

    /**
     * The class used for all Ability Block objects
     * @return The class used for all Ability Block objects
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Data Classes")
    TSubclassOf<UObject> GetAbilityBlockClass() const;

    /**
     * Get the odds of a Pokémon being shiny.
     * @return The odds of a newly generated Pokémon being shiny (out of 65536).
     */
    uint32 GetShinyChance() const;

  private:
    /**
     * The stat used referring to a Pokémon's HP
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetHPStat, Config, DisplayName = "HP Stat", Category = "Display Names",
              meta = (GetOptions = "PokemonData.StatHelper.GetMainStatNames"))
    FName HPStat;

    UPROPERTY(EditAnywhere, BlueprintGetter = GetNoAbilityText, Config, Category = "Display Names")
    FText NoAbilityText;

    UPROPERTY(EditAnywhere, BlueprintGetter = GetNoAbilityDescription, Config, Category = "Display Names")
    FText NoAbilityDescription;

    /**
     * The maximum number of Pokémon a trainer can carry at any given time
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetMaxPartySize, Config, Category = Party,
              meta = (UIMin = 1, ClampMin = 1))
    int32 MaxPartySize = 6;

    /**
     * The maximum number of moves a Pokémon can know.
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetMaxMoves, Config, Category = Moves)
    int32 MaxMoves = 4;

    /**
     * The odds of a newly generated Pokémon being shiny (out of 65536).
     */
    UPROPERTY(EditAnywhere, Config, Category = "Pokémon")
    uint32 ShinyChance = 16;

    /**
     * The class used for all Pokémon objects
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetPokemonClass, Config, AdvancedDisplay, Category = "Data Classes",
              meta = (MustImplement = Pokemon))
    TSubclassOf<UObject> PokemonClass = UGamePokemon::StaticClass();

    /**
     * The class used for all Stat Block objects
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetStatBlockClass, Config, AdvancedDisplay, Category = "Data Classes",
              meta = (MustImplement = StatBlock))
    TSubclassOf<UObject> StatBlockClass = UDefaultStatBlock::StaticClass();

    /**
     * The class used for all Move Block objects
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetMoveBlockClass, Config, AdvancedDisplay, Category = "Data Classes",
              meta = (MustImplement = MoveBlock))
    TSubclassOf<UObject> MoveBlockClass = UDefaultMoveBlock::StaticClass();

    /**
     * The class used for all Ability Block objects
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetAbilityBlockClass, Config, AdvancedDisplay, Category = "Data Classes",
              meta = (MustImplement = AbilityBlock))
    TSubclassOf<UObject> AbilityBlockClass = UDefaultAbilityBlock::StaticClass();

    
};
