// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/GameplayAbilities/Attributes/StatStagesAttributeSet.h"

void UStatStagesAttributeSet::PreAttributeChange(const FGameplayAttribute &Attribute, float &NewValue) {
    // TODO: Change this to be more customizable
    NewValue = FMath::Floor(FMath::Clamp(NewValue, -6.f, 6.f));
}