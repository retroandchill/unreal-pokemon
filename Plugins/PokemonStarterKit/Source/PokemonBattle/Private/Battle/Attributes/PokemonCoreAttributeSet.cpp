// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Attributes/PokemonCoreAttributeSet.h"

void UPokemonCoreAttributeSet::PreAttributeChange(const FGameplayAttribute &Attribute, float &NewValue) {
    NewValue = FMath::RoundHalfToZero(FMath::Max(NewValue, 0.f));

    if (Attribute == GetHPAttribute()) {
        NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHP());
    }
}

void UPokemonCoreAttributeSet::PostAttributeChange(const FGameplayAttribute &Attribute, float OldValue, float NewValue) {
    Super::PostAttributeChange(Attribute, OldValue, NewValue);

    // When the maximum HP changes we want to set the current value to the ratio between the new value and old value.
    // This is the behavior we see when a Pokémon Dynamaxes and when Zygarde transforms into its Complete Forme.
    if (Attribute == GetMaxHPAttribute()) {
        SetHP(GetHP() * NewValue / OldValue);
    }
}