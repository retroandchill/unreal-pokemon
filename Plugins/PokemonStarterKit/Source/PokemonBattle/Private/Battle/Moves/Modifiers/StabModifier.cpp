// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Moves/Modifiers/StabModifier.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Moves/BaseBattleMove.h"
#include "Battle/Moves/BattleMove.h"

void Battle::Moves::FStabModifier::ApplyModifier(FModifiedDamage& Damage, const UBaseBattleMove* Move,
                                                 const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target, int32 TargetCount) const {
    if (User->GetTypes().Contains(Move->DetermineType())) {
        Damage.Modifier *= 1.5f;
    }
}