// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Battlers/BattlerHelpers.h"
#include "AbilitySystemComponent.h"
#include "Abilities/GameplayAbilityTypes.h"

float UBattlerHelpers::GetBattlerStat(const FGameplayAbilityActorInfo &ActorInfo, FGameplayAttribute Attribute, bool& bFound) {
    return ActorInfo.AbilitySystemComponent->GetGameplayAttributeValue(Attribute, bFound);
}