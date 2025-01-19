// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "BattleMessageSettings.generated.h"

USTRUCT(BlueprintType)
struct POKEMONBATTLE_API FStatChangeSettings {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, Category = "Battle|Stats")
    FText StatChangeMessage = NSLOCTEXT("PokemonBattle", "StatChangeMessage", "{Pkmn}'s {Stat} {Change}!");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, Category = "Battle|Stats")
    FText ModifiedChangeFormat = NSLOCTEXT("PokemonBattle", "StatChangeFormat", "{Change} {Modifier}");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, Category = "Battle|Stats")
    FText IncreaseText = NSLOCTEXT("PokemonBattle", "StatIncreaseText", "rose");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, Category = "Battle|Stats")
    FText DecreaseText = NSLOCTEXT("PokemonBattle", "StatDecreaseText", "fell");

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, Category = "Battle|Stats")
    TArray<FText> Modifiers = {
        FText::GetEmpty(),
        NSLOCTEXT("PokemonBattle", "ChangedSharply", "sharply"),
        NSLOCTEXT("PokemonBattle", "ChangedDrastically", "drastically")
    };
};

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, DisplayName = "Battle System Messages")
class POKEMONBATTLE_API UBattleMessageSettings : public UDeveloperSettings {
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Battle)
    FText NoTargetMessage = NSLOCTEXT("PokemonBattle", "NoTargetMessage", "But there was no target...");

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Battle)
    FText MoveMissedMessage = NSLOCTEXT("BattleMove", "HitCheckFailed", "But it missed!");
    
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

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = "Battle|Stats")
    FStatChangeSettings StatChangeMessage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, DisplayName = "HP Restored Message", Config, Category = "Battle|Effects")
    FText HPRestoredMessage = NSLOCTEXT("PokemonBattle", "HPRestoredMessage", "{Pkmn}'s HP was restored!");

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = "Battle|Effects")
    FText NoEffectMessage = NSLOCTEXT("PokemonBattle", "NoEffectMessage", "But it had no effect on {Pkmn}!");

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = "Battle|Switching")
    FText PlayerSendOutMessage = NSLOCTEXT("PokemonBattle", "PlayerSendOutMessage", "Go {Pkmn}!");

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = "Battle|Switching")
    FText OpponentSendOutMessage = NSLOCTEXT("PokemonBattle", "OpponentSendOutMessage", "{Trainer} send out {Pkmn}!");

};
