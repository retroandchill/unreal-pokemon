// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/GameplayAbilities/BattlerAbilityComponent.h"
#include "Battle/GameplayAbilities/Attributes/PokemonCoreAttributeSet.h"

void UBattlerAbilityComponent::BeginPlay() {
    Super::BeginPlay();

    CoreAttributes = NewObject<UPokemonCoreAttributeSet>(this);
    AddSpawnedAttribute(CoreAttributes);
}

UPokemonCoreAttributeSet *UBattlerAbilityComponent::GetCoreAttributes() const {
    return CoreAttributes;
}