// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Moves/Modifiers/TargetCountModifier.h"
#include "Battle/Moves/BattleDamage.h"

void Battle::Moves::FTargetCountModifier::ApplyModifier(FModifiedDamage& Damage,
                                                        const UBaseBattleMove* Move, const TScriptInterface<IBattler> &User,
                                                        const TScriptInterface<IBattler> &Target, int32 TargetCount) const {
    if (TargetCount > 1) {
        Damage.Modifier *= 0.75f;
    }
}