// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Traits/Damage/Conditions/GlobalTraitTagExists.h"
#include "Battle/Battle.h"
#include "Battle/BattleSide.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Moves/BattleDamage.h"
#include "Battle/Traits/TraitHolder.h"

bool UGlobalTraitTagExists::Evaluate_Implementation(const FMoveDamageInfo &Context) const {
    auto &Battle = Context.User->GetOwningSide()->GetOwningBattle();
    return Battle->AnyTraitHolder([this](const ITraitHolder& TraitHolder) {
        return TraitHolder.HasTag(Tag);
    });
}