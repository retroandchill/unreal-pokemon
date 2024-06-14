// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Moves/BattleDamage.h"

#include "ActionResult.generated.h"

class IBattler;

/**
 * The result of the action on a single target
 */
USTRUCT(BlueprintType)
struct FTargetResult {
    GENERATED_BODY()

    /**
     * The actual target of the move
     */
    UPROPERTY()
    TScriptInterface<IBattler> Target;

    /**
     * Did the move actually hit its target
     */
    UPROPERTY(BlueprintReadWrite, Category = "Battle|Actions")
    bool bHit;

    /**
     * Information about the move's damage
     */
    UPROPERTY(BlueprintReadWrite, Category = "Battle|Actions")
    FBattleDamage Damage;
};


/**
 * Represents the information about an action.
 */
USTRUCT(BlueprintType)
struct POKEMONBATTLE_API FActionResult {
    GENERATED_BODY()

    /**
     * The results in each individual target
     */
    UPROPERTY(BlueprintReadWrite, Category = "Battle|Actions")
    TArray<FTargetResult> TargetResults;
    
};