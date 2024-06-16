// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Traits/Damage/ChooseOneDamageModification.h"

void UChooseOneDamageModification::Apply_Implementation(FDamageMultipliers &Multipliers,
                                                        const FMoveDamageInfo &Context) const {
    for (auto Trait : Traits) {
        if (Trait->MeetsConditions(Context)) {
            Trait->Apply(Multipliers, Context);
            break;
        }
    }
}