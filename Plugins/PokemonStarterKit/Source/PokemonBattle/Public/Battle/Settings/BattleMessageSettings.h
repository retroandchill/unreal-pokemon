// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "BattleMessageSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, DisplayName = "Battle System Messages")
class POKEMONBATTLE_API UBattleMessageSettings : public UDeveloperSettings {
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, DisplayName = "Critical Hit Message (Single Target)", Config, Category = Battle)
    FText CriticalHitMessage = NSLOCTEXT("PokemonBattle", "CriticalHitMessage", "A critical hit!");

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, DisplayName = "Critical Hit Message (Multi Target)", Config, Category = Battle)
    FText CriticalHitMessageMulti = NSLOCTEXT("PokemonBattle", "CriticalHitMessageMulti", "A critical hit on {Pkmn}!");

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, DisplayName = "Super Effective Message (Single Target)", Config, Category = Battle)
    FText SuperEffectiveMessage = NSLOCTEXT("PokemonBattle", "SuperEffectiveMMessage", "It's super effective!");

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, DisplayName = "Super Effective Message (Multi Target)", Config, Category = Battle)
    FText SuperEffectiveMessageMulti = NSLOCTEXT("PokemonBattle", "SuperEffectiveMMessageMulti", "It's super effective on {Pkmn}!");

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, DisplayName = "Not Very Effective Message (Single Target)", Config, Category = Battle)
    FText NotVeryEffectiveMessage = NSLOCTEXT("PokemonBattle", "NotVeryEffectiveMessage", "It's not very effective...");

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, DisplayName = "Not Very Effective Message (Multi Target)", Config, Category = Battle)
    FText NotVeryEffectiveMessageMulti = NSLOCTEXT("PokemonBattle", "NotVeryEffectiveMessageMulti", "It's not very effective on {Pkmn}...");
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = "Battle|Stats")
    FText MaxStatMessage = NSLOCTEXT("PokemonBattle", "MaxStatMessage", "{Pkmn}'s {Stat} won't go any higher!");

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = "Battle|Stats")
    FText MinStatMessage = NSLOCTEXT("PokemonBattle", "MinStatMessage", "{Pkmn}'s {Stat} won't go any lower!");

};
