// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SecondaryEffects.generated.h"

class IBattler;

/**
 * Contains any additional effects from a move that need to be applied.
 */
USTRUCT(BlueprintType)
struct POKEMONBATTLE_API FAdditionalMoveEffects {
    GENERATED_BODY()

    /**
     * The changes to any stat stages from this effect
     */
    UPROPERTY(BlueprintReadOnly, Category = "Moves|Stats")
    TMap<FName, int32> StatStageChanges;

    /**
     * Alter the value of a stat stage
     * @param Battler
     * @param Stat The stat that is being altered
     * @param StatChange The change to the stat value
     */
    void AlterStatStage(const IBattler& Battler, FName Stat, int32 StatChange);
};

/**
 * Thin wrapper around a secondary effect used to alter its value
 */
USTRUCT(BlueprintType)
struct POKEMONBATTLE_API FSecondaryEffectHandle {
    GENERATED_BODY()

    TSharedPtr<FAdditionalMoveEffects> Effects;
};