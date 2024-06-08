// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AbilityBattleEffect.generated.h"

class IBattleMove;
class IBattler;
struct FDamageMultipliers;
// This class does not need to be modified.
UINTERFACE()
class UAbilityBattleEffect : public UInterface {
    GENERATED_BODY()
};

/**
 * 
 */
class POKEMONBATTLE_API IAbilityBattleEffect {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    /**
     * Trigger a damage related modification for this ability for the global scope
     * @param Multipliers The multipliers for the move damage
     * @param User The user of the move
     * @param Target The target of the move
     * @param Move The move itself
     * @param BaseDamage The base damage of the move in question
     * @param MoveType The type of the move in question
     */
    UFUNCTION(BlueprintNativeEvent, Category = Damage)
    void TriggerDamageCalcFromGlobal(FDamageMultipliers& Multipliers, const TScriptInterface<IBattler>& User,
        const TScriptInterface<IBattler>& Target, const TScriptInterface<IBattleMove>& Move, int32 BaseDamage,
        FName MoveType);
    
    /**
     * Trigger a damage related modification for this ability for the user
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
     * Trigger a damage related modification for this ability for an ally to the user
     * @param Multipliers The multipliers for the move damage
     * @param User The user of the move
     * @param Target The target of the move
     * @param Move The move itself
     * @param BaseDamage The base damage of the move in question
     * @param MoveType The type of the move in question
     */
    UFUNCTION(BlueprintNativeEvent, Category = Damage)
    void TriggerDamageCalcFromAlly(FDamageMultipliers& Multipliers, const TScriptInterface<IBattler>& User,
        const TScriptInterface<IBattler>& Target, const TScriptInterface<IBattleMove>& Move, int32 BaseDamage,
        FName MoveType);
    
    /**
     * Trigger a damage related modification for this ability for the target
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

    /**
     * Trigger a damage related modification for this ability for the target that cannot be ignored
     * @param Multipliers The multipliers for the move damage
     * @param User The user of the move
     * @param Target The target of the move
     * @param Move The move itself
     * @param BaseDamage The base damage of the move in question
     * @param MoveType The type of the move in question
     */
    UFUNCTION(BlueprintNativeEvent, DisplayName = "Trigger Damage Calc from Target (Non-Ignorable)", Category = Damage)
    void TriggerDamageCalcFromTargetNonIgnorable(FDamageMultipliers& Multipliers, const TScriptInterface<IBattler>& User,
        const TScriptInterface<IBattler>& Target, const TScriptInterface<IBattleMove>&  Move, int32 BaseDamage,
        FName MoveType);

    /**
     * Trigger a damage related modification for this ability for the target's ally
     * @param Multipliers The multipliers for the move damage
     * @param User The user of the move
     * @param Target The target of the move
     * @param Move The move itself
     * @param BaseDamage The base damage of the move in question
     * @param MoveType The type of the move in question
     */
    UFUNCTION(BlueprintNativeEvent, Category = Damage)
    void TriggerDamageCalcFromTargetAlly(FDamageMultipliers& Multipliers, const TScriptInterface<IBattler>& User,
        const TScriptInterface<IBattler>& Target, const TScriptInterface<IBattleMove>&  Move, int32 BaseDamage,
        FName MoveType);

};