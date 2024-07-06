// "Unreal Pokémon" created by Retro & Chill.

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
     * The information about the opponent in battle
     */
    UPROPERTY(BlueprintReadWrite, Category = Battle)
    FBattleOpponentInfoHandle OpponentInfo;

    /**
     * Create the opposing side of battle with this information
     * @tparam A The argument types that are passed to the proxied object
     * @param Args The arguments that are passed to the proxied object
     * @return The created side
     */
    template <typename... A>
    FORCEINLINE TScriptInterface<IBattleSide> CreateOpposingSide(A&&... Args) const {
        return OpponentInfo.CreateOpposingSide<A...>(Forward<A...>(Args)..., OpponentSideCount);
    }
};