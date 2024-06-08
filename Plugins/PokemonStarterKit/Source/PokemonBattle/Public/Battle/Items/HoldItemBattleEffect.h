// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HoldItemBattleEffect.generated.h"

class IBattleMove;
struct FDamageMultipliers;
class IBattler;
// This class does not need to be modified.
UINTERFACE()
class UHoldItemBattleEffect : public UInterface {
    GENERATED_BODY()
};

/**
 * The effect of a hold item in battle.
 */
class POKEMONBATTLE_API IHoldItemBattleEffect {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    /**
         * Trigger a damage related modification for this item for the user
         * @param Multipliers The multipliers for the move damage
         * @param User The user of the move
         * @param Target The target of the move
         * @param Move The move itself
         * @param BaseDamage The base damage of the move in question
         * @param MoveType The type of the move in question
         */
    UFUNCTION(BlueprintNativeEvent, Category = Damage)
    void TriggerDamageCalcFromUser(FDamageMultipliers& Multipliers, const TScriptInterface<IBattler>& User,
        const TScriptInterface<IBattler>& Target, const TScriptInterface<IBattleMove>&  Move, int32 BaseDamage,
        FName MoveType);

    /**
     * Trigger a damage related modification for this item for the target
     * @param Multipliers The multipliers for the move damage
     * @param User The user of the move
     * @param Target The target of the move
     * @param Move The move itself
     * @param BaseDamage The base damage of the move in question
     * @param MoveType The type of the move in question
     */
    UFUNCTION(BlueprintNativeEvent, Category = Damage)
    void TriggerDamageCalcFromTarget(FDamageMultipliers& Multipliers, const TScriptInterface<IBattler>& User,
        const TScriptInterface<IBattler>& Target, const TScriptInterface<IBattleMove>&  Move, int32 BaseDamage,
        FName MoveType);

};