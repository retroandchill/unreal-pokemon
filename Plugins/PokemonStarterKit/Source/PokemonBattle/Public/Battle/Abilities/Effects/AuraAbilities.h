// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Abilities/AbilityBattleEffect.h"
#include "UObject/Object.h"

#include "AuraAbilities.generated.h"

/**
 * Powers up the Dark-type moves of all Pokémon on the field.
 */
UCLASS()
class POKEMONBATTLE_API UDarkAura : public UObject, public IAbilityBattleEffect {
    GENERATED_BODY()

  public:
    void TriggerDamageCalcFromGlobal_Implementation(FDamageMultipliers &Multipliers,
                                                    const TScriptInterface<IBattler> &User,
                                                    const TScriptInterface<IBattler> &Target,
                                                    const TScriptInterface<IBattleMove> &Move, int32 BaseDamage,
                                                    FName MoveType) override;
};

/**
 * Powers up the Fairy-type moves of all Pokémon on the field.
 */
UCLASS()
class POKEMONBATTLE_API UFairyAura : public UObject, public IAbilityBattleEffect {
    GENERATED_BODY()

  public:
    void TriggerDamageCalcFromGlobal_Implementation(FDamageMultipliers &Multipliers,
                                                    const TScriptInterface<IBattler> &User,
                                                    const TScriptInterface<IBattler> &Target,
                                                    const TScriptInterface<IBattleMove> &Move, int32 BaseDamage,
                                                    FName MoveType) override;
};
