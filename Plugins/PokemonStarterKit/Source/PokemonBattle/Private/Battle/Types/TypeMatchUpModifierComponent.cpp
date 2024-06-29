// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Types/TypeMatchUpModifierComponent.h"
#include "AbilitySystemComponent.h"
#include "PokemonBattleModule.h"
#include "Battle/Type.h"
#include "Battle/GameplayAbilities/Attributes/MoveUsageAttributeSet.h"
#include "Battle/Types/SingleTypeModPayload.h"
#include "Battle/Types/TypeTags.h"

using FEventDelegate = FGameplayEventTagMulticastDelegate::FDelegate;

bool UTypeMatchUpModifierComponent::OnActiveGameplayEffectAdded(FActiveGameplayEffectsContainer &ActiveGEContainer,
                                                                FActiveGameplayEffect &ActiveGE) const {
    auto ASC = ActiveGEContainer.Owner;
    check(ASC != nullptr)
    FGameplayTagContainer TagFilter(Pokemon::Battle::Types::SingleTypeModifierEvent);
    auto EventHandler = ASC->AddGameplayEventTagContainerDelegate(TagFilter,
        FEventDelegate::CreateUObject(this, &UTypeMatchUpModifierComponent::OnEventReceived));
    auto EventSet = ASC->GetActiveEffectEventSet(ActiveGE.Handle);
    check(EventSet != nullptr)
    EventSet->OnEffectRemoved.AddUObject(this, &UTypeMatchUpModifierComponent::OnGameplayEffectRemoved, ASC, MoveTemp(TagFilter), EventHandler);
    return true;
}

void UTypeMatchUpModifierComponent::OnGameplayEffectRemoved(const FGameplayEffectRemovalInfo& GERemovalInfo, UAbilitySystemComponent *ASC,
    FGameplayTagContainer Container, FDelegateHandle DelegateHandle) const {
    ASC->RemoveGameplayEventTagContainerDelegate(Container, DelegateHandle);
}

void UTypeMatchUpModifierComponent::OnEventReceived(FGameplayTag Tag, const FGameplayEventData *EventData) const {
    using namespace Pokemon::TypeEffectiveness;
    auto Payload = CastChecked<USingleTypeModPayload>(EventData->OptionalObject);
    auto &[AttackingType, DefendingType, Multiplier] = Payload->GetData();

    if ((!AttackingTypes.IsEmpty() && !AttackingTypes.Contains(AttackingType)) ||
        (!DefendingTypes.IsEmpty() && !DefendingTypes.Contains(DefendingType))) {
        return;
    }

    if (auto Effectiveness = static_cast<uint8>(GetEffectivenessFromMultiplier(Multiplier)); (Effectiveness & ApplyFor) == 0) {
        return;
    }

    switch (ModifierOp) {
    case EGameplayModOp::Additive:
        Multiplier += Modifier;
        break;
    case EGameplayModOp::Multiplicitive:
        Multiplier *= Modifier;
        break;
    case EGameplayModOp::Division:
        Multiplier /= Modifier;
        break;
    case EGameplayModOp::Override:
        Multiplier = Modifier;
        break;
    case EGameplayModOp::Max:
        Multiplier = FMath::Max(Multiplier, Modifier);
        break;
    default:
        UE_LOG(LogBattle, Warning, TEXT("The provided modifier operator '%s', was invalid for type modification!"),
            *UEnum::GetValueAsString(ModifierOp))
        break;
    }
}