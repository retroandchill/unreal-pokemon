// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Effects/FieldEffect.h"
#include "UObject/Object.h"
#include "SportMoves.generated.h"

/**
 * The effect for the move Mud Sport.
 */
UCLASS()
class POKEMONBATTLE_API UEffectMudSport : public UObject, public IFieldEffect {
    GENERATED_BODY()

protected:
    void ModifyDamage_Implementation(FDamageMultipliers &Multipliers, const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target, const TScriptInterface<IBattleMove> &Move, int32 BaseDamage, FName MoveType) override;

};

/**
 * The effect for the move Water Sport.
 */
UCLASS()
class POKEMONBATTLE_API UEffectWaterSport : public UObject, public IFieldEffect {
    GENERATED_BODY()

protected:
    void ModifyDamage_Implementation(FDamageMultipliers &Multipliers, const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target, const TScriptInterface<IBattleMove> &Move, int32 BaseDamage, FName MoveType) override;

};
