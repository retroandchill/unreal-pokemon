// "Unreal Pokémon" created by Retro & Chill.

#include "Utilities/StatusEffectUtilities.h"
#include "Pokemon/Pokemon.h"

EStatusEffectStatus UStatusEffectUtilities::GetStatusEffect(const TScriptInterface<IPokemon> &Pokemon,
                                                            FStatus &StatusEffect) {
    auto Status = Pokemon->GetStatusEffect();
    if (Status == nullptr) {
        return EStatusEffectStatus::NoStatusEffect;
    }

    StatusEffect = *Status;
    return EStatusEffectStatus::HasStatusEffect;
}