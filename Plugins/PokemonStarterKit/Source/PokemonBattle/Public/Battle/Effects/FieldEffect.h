// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "FieldEffect.generated.h"

class IBattleMove;
class IBattler;
struct FDamageMultipliers;

// This class does not need to be modified.
UINTERFACE()
class UFieldEffect : public UInterface {
    GENERATED_BODY()
};

/**
 * Effect that affects the entire field, not just a single battler/side
 */
class POKEMONBATTLE_API IFieldEffect {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
  public:
    /**
     * Trigger a damage related modification for this effect
     * @param Multipliers The multipliers for the move damage
     * @param User The user of the move
     * @param Target The target of the move
     * @param Move The move being used
     * @param BaseDamage The base damage of the move in question
     * @param MoveType The type of the move in question
     */
    UFUNCTION(BlueprintNativeEvent, Category = Damage)
    void ModifyDamage(FDamageMultipliers &Multipliers, const TScriptInterface<IBattler> &User,
                      const TScriptInterface<IBattler> &Target, const TScriptInterface<IBattleMove> &Move,
                      int32 BaseDamage, FName MoveType);
};