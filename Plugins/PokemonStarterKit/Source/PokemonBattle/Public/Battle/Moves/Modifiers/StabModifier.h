// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "DamageModifier.h"

namespace Battle::Moves {

/**
 * Damage modifier for a move's Same-Type Attack Bonus.
 */
class POKEMONBATTLE_API FStabModifier : public IDamageModifier {
public:
    void ApplyModifier(FModifiedDamage& Damage, const UBaseBattleMove* Move,
                       const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target, int32 TargetCount) const override;

};

}
