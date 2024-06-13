// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "BattlerEffect.generated.h"

class IBattler;
class IBattleMove;
struct FDamageMultipliers;

// This class does not need to be modified.
UINTERFACE()
class UBattlerEffect : public UInterface {
    GENERATED_BODY()
};

/**
 * Represents a battle effect that applies directly to a single battler.
 */
class POKEMONBATTLE_API IBattlerEffect {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
  public:
    /**
     * Trigger a damage related modification for this effect on the user
     * @param Multipliers The multipliers for the move damage
     * @param User The user of the move
     * @param Target The target of the move
     * @param Move The move being used
     * @param BaseDamage The base damage of the move in question
     * @param MoveType The type of the move in question
     */
    UFUNCTION(BlueprintNativeEvent, Category = Damage)
    void ModifyDamageForUser(FDamageMultipliers &Multipliers, const TScriptInterface<IBattler> &User,
                             const TScriptInterface<IBattler> &Target, const TScriptInterface<IBattleMove> &Move,
                             int32 BaseDamage, FName MoveType);

    /**
     * Trigger a damage related modification for this effect on the target
     * @param Multipliers The multipliers for the move damage
     * @param User The user of the move
     * @param Target The target of the move
     * @param Move The move being used
     * @param BaseDamage The base damage of the move in question
     * @param MoveType The type of the move in question
     */
    UFUNCTION(BlueprintNativeEvent, Category = Damage)
    void ModifyDamageForTarget(FDamageMultipliers &Multipliers, const TScriptInterface<IBattler> &User,
                               const TScriptInterface<IBattler> &Target, const TScriptInterface<IBattleMove> &Move,
                               int32 BaseDamage, FName MoveType);
};