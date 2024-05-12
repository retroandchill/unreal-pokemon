// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Pokemon/GamePokemon.h"
#include "Pokemon/Abilities/DefaultAbilityBlock.h"
#include "Pokemon/Moves/DefaultMoveBlock.h"
#include "Pokemon/Stats/DefaultStatBlock.h"

#include "DependencyInjectionSettings.generated.h"

/**
 * The settings related to dependency injection related tasks in Pokémon Core.
 */
UCLASS(Config = Game, DefaultConfig)
class POKEMONCORE_API UDependencyInjectionSettings : public UDeveloperSettings {
    GENERATED_BODY()

public:
    /**
     * The class used for all Pokémon objects
     * @return The class used for all Pokémon objects
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Dependency Injection")
    TSubclassOf<UObject> GetPokemonClass() const;

    /**
     * The class used for all Stat Block objects
     * @return The class used for all Stat Block objects
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Dependency Injection")
    TSubclassOf<UObject> GetStatBlockClass() const;

    /**
     * The class used for all Move Block objects
     * @return The class used for all Move Block objects
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Dependency Injection")
    TSubclassOf<UObject> GetMoveBlockClass() const;

    /**
     * The class used for all Ability Block objects
     * @return The class used for all Ability Block objects
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Dependency Injection")
    TSubclassOf<UObject> GetAbilityBlockClass() const;

private:
    /**
     * The class used for all Pokémon objects
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetPokemonClass, Config, Category = "Dependency Injection",
              meta = (MustImplement = Pokemon))
    TSubclassOf<UObject> PokemonClass = UGamePokemon::StaticClass();

    /**
     * The class used for all Stat Block objects
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetStatBlockClass, Config, Category = "Dependency Injection",
              meta = (MustImplement = StatBlock))
    TSubclassOf<UObject> StatBlockClass = UDefaultStatBlock::StaticClass();

    /**
     * The class used for all Move Block objects
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetMoveBlockClass, Config, Category = "Dependency Injection",
              meta = (MustImplement = MoveBlock))
    TSubclassOf<UObject> MoveBlockClass = UDefaultMoveBlock::StaticClass();

    /**
     * The class used for all Ability Block objects
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetAbilityBlockClass, Config, Category = "Dependency Injection",
              meta = (MustImplement = AbilityBlock))
    TSubclassOf<UObject> AbilityBlockClass = UDefaultAbilityBlock::StaticClass();

};
