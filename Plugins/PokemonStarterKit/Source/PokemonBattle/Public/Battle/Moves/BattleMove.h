// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BattleDamage.h"
#include "UObject/Interface.h"
#include "BattleMove.generated.h"

class IMove;
class IBattler;

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UBattleMove : public UInterface {
    GENERATED_BODY()
};

/**
 * Wrapper around a move for its effects in battle.
 */
class POKEMONBATTLE_API IBattleMove {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    /**
     * Initialize the move from the given owned move.
     * @param Move The move that is currently owned by the user. 
     * @return The initialized interface
     */
    virtual TScriptInterface<IBattleMove> Initialize(const TScriptInterface<IMove>& Move) = 0;
    
    /**
     * Calculate the total damage the move will deal.
     * @param User The user of the move.
     * @param Target The target the move is being hit by.
     * @param TargetCount The number of targets being hit.
     * @return The amount of damage dealt 
     */
    UFUNCTION(BlueprintNativeEvent, Category = Damage)
    FBattleDamage CalculateDamage(const TScriptInterface<IBattler>& User, const TScriptInterface<IBattler>& Target, int32 TargetCount = 1);

    /**
     * Calculate the base damage of the particular move before any modifiers are applied
     * @param User The user of the move
     * @param Target The target of the move
     * @return The base damage, pre-modification
     */
    UFUNCTION(BlueprintNativeEvent, Category = Damage)
    int32 CalculateBaseDamage(const TScriptInterface<IBattler>& User, const TScriptInterface<IBattler>& Target);

    /**
     * Calculate the true power of the move, pulled from the move's base damage
     * @param User The user of the move
     * @param Target The target of the move
     * @return The actual power value to use
     */
    UFUNCTION(BlueprintNativeEvent, Category = Damage)
    int32 CalculateTruePower(const TScriptInterface<IBattler>& User, const TScriptInterface<IBattler>& Target);

    /**
     * Fetch the attack and defense stats to use for damage
     * @param User The user of the move
     * @param Target The target of the move
     * @return The values of the stats to use
     */
    UFUNCTION(BlueprintNativeEvent, Category = Damage)
    FAttackAndDefense GetAttackAndDefense(const TScriptInterface<IBattler>& User, const TScriptInterface<IBattler>& Target);
    
};