// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Types/PokemonType.h"
#include "Battle/Types/TypeTags.h"

UPokemonType::UPokemonType() {
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

#if WITH_EDITOR
void UPokemonType::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) {
    Super::PostEditChangeProperty(PropertyChangedEvent);
    
    AbilityTags.RemoveTag(Pokemon::Battle::Types::BattlerTypes);
    if (PropertyChangedEvent.GetPropertyName() != GET_MEMBER_NAME_CHECKED(UPokemonType, SourceType)) {
        AttackingTag = FGameplayTag();
        DefendingTag = FGameplayTag();
        return;
    }

    auto &Lookup = Pokemon::Battle::Types::FLookup::GetInstance();
    AttackingTag = Lookup.GetAttackingTag(SourceType);
    if (AttackingTag.IsValid()) {
        AbilityTags.AddTag(AttackingTag);
    }
    
    DefendingTag = Lookup.GetDefendingTag(SourceType);
    if (DefendingTag.IsValid()) {
        AbilityTags.AddTag(DefendingTag);
    }
}
#endif