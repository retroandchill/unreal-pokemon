// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Traits/TraitUtilities.h"

void Traits::ApplyIndividualDamageModifications(FDamageMultipliers &Multipliers, const FMoveDamageInfo &Context) {
    Context.User->ForEachIndividualTraitHolder(DAMAGE_MODIFICATION_CALLBACK(User, Multipliers, Context));
    Context.User->ForEachAlly(ALLY_DAMAGE_MODIFICATION_CALLBACK(UserAllies, Multipliers, Context));
    Context.Target->ForEachIndividualTraitHolder(DAMAGE_MODIFICATION_CALLBACK(Target, Multipliers, Context));
    Context.Target->ForEachAlly(ALLY_DAMAGE_MODIFICATION_CALLBACK(TargetAllies, Multipliers, Context));
}