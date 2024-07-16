﻿// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BattleOpponentInfo.h"
#include "UObject/Object.h"

#include "BattleInfo.generated.h"

/**
 * Handle that wraps around a battle transition's information
 */
USTRUCT(BlueprintType)
struct POKEMONBATTLE_API FBattleInfo {
    GENERATED_BODY()

    /**
     * How many Pokémon are sent out on the player's side of battle
     */
    UPROPERTY(BlueprintReadWrite, Category = Battle)
    int32 PlayerSideCount = 1;

    /**
     * How many Pokémon are sent on the opponent's side of battle
     */
    UPROPERTY(BlueprintReadWrite, Category = Battle)
    int32 OpponentSideCount = 1;

    /**
     * Is the player allowed to lose this battle
     */
    UPROPERTY(BlueprintReadWrite, Category = Battle)
    bool bLossAllowed = false;

    /**
     * The information about the opponent in battle
     */
    UPROPERTY(BlueprintReadWrite, Category = Battle)
    FBattleOpponentInfoHandle OpponentInfo;

    TScriptInterface<IBattleSide> CreatePlayerSide(const TScriptInterface<IBattle> &Battle,
                                                   const TSubclassOf<AActor> &SideClass,
                                                   const FTransform &Transform) const;

    /**
     * Create the opposing side of battle with this information
     * @param Battle The battle that will take ownership of the side
     * @param SideClass The class for the side that is being spawned in
     * @param Transform The transform to spawn the side at
     * @return The created side
     */
    FORCEINLINE TScriptInterface<IBattleSide> CreateOpposingSide(const TScriptInterface<IBattle> &Battle,
                                                                 const TSubclassOf<AActor> &SideClass,
                                                                 const FTransform &Transform) const {
        return OpponentInfo.CreateOpposingSide(Battle, SideClass, Transform, OpponentSideCount);
    }
};