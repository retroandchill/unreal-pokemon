// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/GameplayAbilities/BattlerAbilityComponent.h"
#include "Battle/GameplayAbilities/Attributes/PokemonCoreAttributeSet.h"
#include "Battle/GameplayAbilities/Attributes/StatStagesAttributeSet.h"

void UBattlerAbilityComponent::BeginPlay() {
    Super::BeginPlay();

    CoreAttributes = NewObject<UPokemonCoreAttributeSet>(this);
    AddSpawnedAttribute(CoreAttributes);

    StatStagesAttributeSet = NewObject<UStatStagesAttributeSet>(this);
    AddSpawnedAttribute(StatStagesAttributeSet);
}

UPokemonCoreAttributeSet *UBattlerAbilityComponent::GetCoreAttributes() const {
    return CoreAttributes;
}

UStatStagesAttributeSet * UBattlerAbilityComponent::GetStatStages() const {
    return StatStagesAttributeSet;
}