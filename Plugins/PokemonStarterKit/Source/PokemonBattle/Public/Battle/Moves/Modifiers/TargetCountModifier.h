// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "DamageModifier.h"

class UBaseBattleMove;

namespace Battle::Moves {
/**
 * Damage modifier for a move that hits multiple targets.
 */
class POKEMONBATTLE_API FTargetCountModifier  : public IDamageModifier {
public:
    void ApplyModifier(FModifiedDamage& Damage, const UBaseBattleMove* Move,
                       const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target, int32 TargetCount) const override;

};
}