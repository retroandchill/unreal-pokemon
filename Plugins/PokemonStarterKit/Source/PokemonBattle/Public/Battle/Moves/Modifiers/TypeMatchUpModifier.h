// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "DamageModifier.h"


namespace Battle::Moves {
/**
 * Damage modifier for type match-ups
 */
class POKEMONBATTLE_API FTypeMatchUpModifier : public IDamageModifier {
public:
    void ApplyModifier(FModifiedDamage& Damage, const UBaseBattleMove* Move,
                       const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target, int32 TargetCount) const override;

};
}