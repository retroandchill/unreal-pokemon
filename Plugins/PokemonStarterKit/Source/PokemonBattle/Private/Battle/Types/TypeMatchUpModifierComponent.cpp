// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Types/TypeMatchUpModifierComponent.h"
#include "AbilitySystemComponent.h"
#include "PokemonBattleModule.h"
#include "RangeHelpers.h"
#include "Battle/Type.h"
#include "Battle/Types/SingleTypeModPayload.h"
#include "Battle/Types/TypeTags.h"
#include <range/v3/view/transform.hpp>

using FEventDelegate = FGameplayEventTagMulticastDelegate::FDelegate;

static FGameplayTag GetTagForScope(ETargetedEventScope Scope) {
    using namespace Pokemon::Battle::Types;
    return SingleTypeModifierEvents.GetTagForScope(Scope);
}

bool UTypeMatchUpModifierComponent::OnActiveGameplayEffectAdded(FActiveGameplayEffectsContainer &ActiveGEContainer,
                                                                FActiveGameplayEffect &ActiveGE) const {
    auto ASC = ActiveGEContainer.Owner;
    check(ASC != nullptr)
    auto GameplayTags = RangeHelpers::CreateRange(Scope)
        | ranges::views::transform(&GetTagForScope)
        | RangeHelpers::TToArray<FGameplayTag>();
    auto TagFilter = FGameplayTagContainer::CreateFromArray(GameplayTags);
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
    auto &Data = Payload->GetData();

    if ((!AttackingTypes.IsEmpty() && !AttackingTypes.Contains(Data.AttackingType)) ||
        (!DefendingTypes.IsEmpty() && !DefendingTypes.Contains(Data.DefendingType))) {
        return;
    }

    if (auto Effectiveness = static_cast<uint8>(GetEffectivenessFromMultiplier(Data.Multiplier)); (Effectiveness & ApplyFor) == 0) {
        return;
    }

    switch (ModifierOp) {
    case EGameplayModOp::Additive:
        Payload->SetMultiplier(Data.Multiplier + Modifier);
        break;
    case EGameplayModOp::Multiplicitive:
        Payload->SetMultiplier(Data.Multiplier * Modifier);
        break;
    case EGameplayModOp::Division:
        Payload->SetMultiplier(Data.Multiplier / Modifier);
        break;
    case EGameplayModOp::Override:
        Payload->SetMultiplier(Modifier);
        break;
    case EGameplayModOp::Max:
        Payload->SetMultiplier(FMath::Max(Data.Multiplier, Modifier));
        break;
    default:
        UE_LOG(LogBattle, Warning, TEXT("The provided modifier operator '%s', was invalid for type modification!"),
            *UEnum::GetValueAsString(ModifierOp))
        break;
    }
}