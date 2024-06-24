// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/GameplayAbilities/BattlerAbilityComponent.h"
#include "Battle/GameplayAbilities/Attributes/DamageModificationAttributeSet.h"


void UBattlerAbilityComponent::BeginPlay() {
    Super::BeginPlay();
    if (DamageModificationAttributes == nullptr) {
        DamageModificationAttributes = NewObject<UDamageModificationAttributeSet>(this);
        AddAttributeSetSubobject(DamageModificationAttributes.Get());
    }
}

UDamageModificationAttributeSet * UBattlerAbilityComponent::GetDamageModificationAttributes() const {
    return DamageModificationAttributes;
}