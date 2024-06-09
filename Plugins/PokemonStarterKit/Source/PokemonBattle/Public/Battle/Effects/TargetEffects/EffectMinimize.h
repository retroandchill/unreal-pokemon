// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Effects/BattlerEffect.h"
#include "UObject/Object.h"
#include "EffectMinimize.generated.h"

/**
 * Causes perfect accuracy and double damage if target used Minimize.
 */
UCLASS()
class POKEMONBATTLE_API UEffectMinimize : public UObject, public IBattlerEffect {
    GENERATED_BODY()

protected:
    void ModifyDamageForTarget_Implementation(FDamageMultipliers &Multipliers, const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target, const TScriptInterface<IBattleMove> &Move, int32 BaseDamage, FName MoveType) override;

};
