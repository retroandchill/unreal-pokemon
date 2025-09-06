// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Battlers/BattlerAbilityComponent.h"
#include "Battle/Attributes/ExpAttributeSet.h"
#include "Battle/Attributes/PokemonCoreAttributeSet.h"
#include "Battle/Attributes/StatStagesAttributeSet.h"
#include "Battle/Attributes/TargetDamageStateAttributeSet.h"

void UBattlerAbilityComponent::BeginPlay()
{
    Super::BeginPlay();

    CoreAttributes = NewObject<UPokemonCoreAttributeSet>(GetOwner());
    AddSpawnedAttribute(CoreAttributes);

    StatStagesAttributeSet = NewObject<UStatStagesAttributeSet>(GetOwner());
    AddSpawnedAttribute(StatStagesAttributeSet);

    TargetDamageStateAttributeSet = NewObject<UTargetDamageStateAttributeSet>(GetOwner());
    AddSpawnedAttribute(TargetDamageStateAttributeSet);

    ExpAttributeSet = NewObject<UExpAttributeSet>(GetOwner());
    AddSpawnedAttribute(ExpAttributeSet);
}

UPokemonCoreAttributeSet *UBattlerAbilityComponent::GetCoreAttributes() const
{
    return CoreAttributes;
}

UStatStagesAttributeSet *UBattlerAbilityComponent::GetStatStages() const
{
    return StatStagesAttributeSet;
}

UTargetDamageStateAttributeSet *UBattlerAbilityComponent::GetTargetDamageStateAttributeSet() const
{
    return TargetDamageStateAttributeSet;
}

UExpAttributeSet *UBattlerAbilityComponent::GetExpAttributeSet() const
{
    return ExpAttributeSet;
}

TOptional<FGameplayAbilitySpecHandle> UBattlerAbilityComponent::FindAbilityOfClass(
    TSubclassOf<UGameplayAbility> AbilityClass) const
{
    auto AbilitySpec =
        Algo::FindByPredicate(ActivatableAbilities.Items, [AbilityClass](const FGameplayAbilitySpec &Spec) {
            return Spec.Ability->IsA(AbilityClass);
        });
    if (AbilitySpec == nullptr)
    {
        return TOptional<FGameplayAbilitySpecHandle>();
    }

    return AbilitySpec->Handle;
}