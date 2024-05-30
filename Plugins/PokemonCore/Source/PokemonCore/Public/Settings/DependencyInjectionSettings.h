// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Player/DefaultBag.h"
#include "Pokemon/Abilities/DefaultAbilityBlock.h"
#include "Pokemon/GamePokemon.h"
#include "Pokemon/Moves/DefaultMoveBlock.h"
#include "Pokemon/Stats/DefaultStatBlock.h"
#include "Pokemon/TrainerMemo/DefaultObtainedBlock.h"

#include "DependencyInjectionSettings.generated.h"

/**
 * The settings related to dependency injection related tasks in Pokémon Core.
 */
UCLASS(Config = Game, DefaultConfig)
class POKEMONCORE_API UDependencyInjectionSettings : public UDeveloperSettings {
    GENERATED_BODY()

  public:
    UDependencyInjectionSettings(const FObjectInitializer& ObjectInitializer);
    
    /**
     * The class used for all Pokémon objects
     * @return The class used for all Pokémon objects
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Dependency Injection|Pokémon")
    TSubclassOf<UObject> GetPokemonClass() const;

    /**
     * The class used for all Stat Block objects
     * @return The class used for all Stat Block objects
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Dependency Injection|Pokémon")
    TSubclassOf<UObject> GetStatBlockClass() const;

    /**
     * The class used for all Move Block objects
     * @return The class used for all Move Block objects
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Dependency Injection|Pokémon")
    TSubclassOf<UObject> GetMoveBlockClass() const;

    /**
     * The class used for all Ability Block objects
     * @return The class used for all Ability Block objects
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Dependency Injection|Pokémon")
    TSubclassOf<UObject> GetAbilityBlockClass() const;

    /**
     * The class used for all Obtained Block objects
     * @return The class used for all Obtained Block objects
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Dependency Injection|Pokémon")
    TSubclassOf<UObject> GetObtainedBlockClass() const;

    /**
     * The class used for the player's bag
     * @return The class used for the player's bag
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Dependency Injection|Player")
    TSubclassOf<UObject> GetBagClass() const;

  private:
    void AddInjection(const IRegisteredInjection& Injection);
    
    UPROPERTY(EditAnywhere, Category = "Dependency Injection")
    TMap<UClass*, TSubclassOf<UObject>> TargetInjections; 
    
    /**
     * The class used for all Pokémon objects
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetPokemonClass, Config, Category = "Dependency Injection|Pokémon",
              meta = (MustImplement = Pokemon))
    TSubclassOf<UObject> PokemonClass = UGamePokemon::StaticClass();

    /**
     * The class used for all Stat Block objects
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetStatBlockClass, Config, Category = "Dependency Injection|Pokémon",
              meta = (MustImplement = StatBlock))
    TSubclassOf<UObject> StatBlockClass = UDefaultStatBlock::StaticClass();

    /**
     * The class used for all Move Block objects
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetMoveBlockClass, Config, Category = "Dependency Injection|Pokémon",
              meta = (MustImplement = MoveBlock))
    TSubclassOf<UObject> MoveBlockClass = UDefaultMoveBlock::StaticClass();

    /**
     * The class used for all Ability Block objects
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetAbilityBlockClass, Config, Category = "Dependency Injection|Pokémon",
              meta = (MustImplement = AbilityBlock))
    TSubclassOf<UObject> AbilityBlockClass = UDefaultAbilityBlock::StaticClass();

    /**
     * The class used for all Obtained Block objects
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetObtainedBlockClass, Config, Category = "Dependency Injection|Pokémon",
              meta = (MustImplement = ObtainedBlock))
    TSubclassOf<UObject> ObtainedBlockClass = UDefaultObtainedBlock::StaticClass();

    /**
     * The class used for the player's bag
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetBagClass, Config, Category = "Dependency Injection|Player",
              meta = (MustImplement = Bag))
    TSubclassOf<UObject> BagClass = UDefaultBag::StaticClass();
};
