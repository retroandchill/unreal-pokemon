// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BattleSettings.generated.h"

/**
 * Represents the side of the battlefield for a created battle.
 */
USTRUCT(BlueprintType)
struct POKEMONBATTLE_API FBattlefieldSize {
    GENERATED_BODY()

    /**
     * How many Pokémon are on the player's side
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Battle, meta = (UIMin = 1, ClampMin = 1))
    uint8 PlayerSide = 1;

    /**
     * How many Pokémon are on the opponent's side.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Battle, meta = (UIMin = 1, ClampMin = 1))
    uint8 OpponentSide = 1;
};

/**
 * Represents the various settings for a battle.
 */
USTRUCT(BlueprintType)
struct POKEMONBATTLE_API FBattleSettings {
    GENERATED_BODY()

    /**
     * The side of the battlefield for the battle.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FBattlefieldSize BattlefieldSize;
    
};
