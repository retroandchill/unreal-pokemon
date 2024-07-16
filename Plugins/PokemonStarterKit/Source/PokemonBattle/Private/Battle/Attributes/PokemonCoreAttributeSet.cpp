// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Attributes/PokemonCoreAttributeSet.h"
#include "Settings/BaseSettings.h"

void UPokemonCoreAttributeSet::InitHP(float NewVal) {
    HP.SetBaseValue(NewVal);
    HP.SetCurrentValue(NewVal);
    UpdateHPTags();
}

void UPokemonCoreAttributeSet::InitMaxHP(float NewVal) {
    MaxHP.SetBaseValue(NewVal);
    MaxHP.SetCurrentValue(NewVal);
    UpdateHPTags();
}

void UPokemonCoreAttributeSet::PreAttributeChange(const FGameplayAttribute &Attribute, float &NewValue) {
    NewValue = FMath::RoundHalfToZero(FMath::Max(NewValue, 0.f));

    if (Attribute == GetHPAttribute()) {
        NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHP());
    }
}

void UPokemonCoreAttributeSet::PostAttributeChange(const FGameplayAttribute &Attribute, float OldValue,
                                                   float NewValue) {
    // When the maximum HP changes we want to set the current value to the ratio between the new value and old value.
    // This is the behavior we see when a Pokémon Dynamaxes and when Zygarde transforms into its Complete Forme.
    if (Attribute == GetMaxHPAttribute()) {
        SetHP(GetHP() * NewValue / OldValue);
    } else if (Attribute == GetHPAttribute()) {
        UpdateHPTags();
    }
}

void UPokemonCoreAttributeSet::UpdateHPTags() const {
    float MaxHPValue = GetMaxHP();
    if (MaxHPValue == 0.f) {
        return;
    }

    float HPPercent = GetHP() / MaxHPValue;
    auto Owner = GetOwningAbilitySystemComponent();
    static auto &HPStateTags = Pokemon::FBaseSettings::Get().GetHPStateTags();
    for (auto &[Threshold, Tag] : HPStateTags) {
        if (HPPercent <= Threshold) {
            Owner->AddLooseGameplayTag(Tag);
        } else if (Owner->HasMatchingGameplayTag(Tag)) {
            Owner->RemoveLooseGameplayTag(Tag);
        }
    }
}