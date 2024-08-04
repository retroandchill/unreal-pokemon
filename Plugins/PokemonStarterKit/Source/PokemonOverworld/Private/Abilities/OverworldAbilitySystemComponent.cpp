// "Unreal Pokémon" created by Retro & Chill.

#include "Abilities/OverworldAbilitySystemComponent.h"
#include "Management/RandomEncounterAttributeSet.h"

void UOverworldAbilitySystemComponent::BeginPlay() {
    Super::BeginPlay();

    EncounterAttributes = NewObject<URandomEncounterAttributeSet>(GetOwner());
    AddSpawnedAttribute(EncounterAttributes);
}