// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Moves/BattleMoveFunctionCode.h"
#include "RangeHelpers.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Moves/BattleMove.h"
#include "Pokemon/Moves/Move.h"
#include <range/v3/view/filter.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/transform.hpp>

UBattleMoveFunctionCode::UBattleMoveFunctionCode() {
    auto &AbilityTrigger = AbilityTriggers.Emplace_GetRef();
    AbilityTrigger.TriggerTag = FGameplayTag::RequestGameplayTag("Battle.UsingMove");
}

void UBattleMoveFunctionCode::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData *TriggerEventData) {
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    // If this is not triggered by an event throw an exception
    check(TriggerEventData != nullptr)
    BattleMove = TriggerEventData->OptionalObject;
    check(BattleMove.GetObject()->Implements<IBattleMove>())
}

bool UBattleMoveFunctionCode::CheckCost(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo *ActorInfo, FGameplayTagContainer *OptionalRelevantTags) const {
    return BattleMove->GetCurrentPP() > 0 && Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags);
}

void UBattleMoveFunctionCode::ApplyCost(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const {
    Super::ApplyCost(Handle, ActorInfo, ActivationInfo);
    
    auto AttributeSet = ActorInfo->AbilitySystemComponent->GetAttributeSet(CostAttribute.GetAttributeSetClass());
    check(AttributeSet != nullptr)
    BattleMove->PayCost(FMath::FloorToInt32(CostAttribute.GetNumericValue(AttributeSet)));
}

void UBattleMoveFunctionCode::CommitExecute(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) {
    Super::CommitExecute(Handle, ActorInfo, ActivationInfo);

    check(ActorInfo != nullptr)
    auto TargetsActors = FilterInvalidTargets(Handle, *ActorInfo);

    TScriptInterface<IBattler> User = ActorInfo->OwnerActor.Get();
    check(User.GetObject()->Implements<IBattler>())
    TArray<TScriptInterface<IBattler>> Targets;
    Targets.Reserve(TargetsActors.Num());
    for (auto Actor : TargetsActors) {
        Targets.Emplace(Actor);
    }

    if (MoveFailed(User, Targets)) {
        // TODO: Add the "but it failed" message
        EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
    }
    
    if (Targets.IsEmpty()) {
        // TODO: Add "But there was no target" message
        EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
    }

    bool bSuccessEncountered = true;
    for (auto &Target : Targets) {
        if (!MoveSucceedsAgainstTarget(User, Target, Targets.Num())) {
            continue;
        }

        // TODO: Handle the actual hit processing
        
        bSuccessEncountered = true;
    }
}

bool UBattleMoveFunctionCode::MoveSucceedsAgainstTarget_Implementation(const TScriptInterface<IBattler> &User,
    const TScriptInterface<IBattler> &Target, int32 TargetCount) {
    return true;
}

bool UBattleMoveFunctionCode::MoveFailed_Implementation(const TScriptInterface<IBattler> &User, const TArray<TScriptInterface<IBattler>> &Targets) {
    if (BattleMove->GetCategory() == EMoveDamageCategory::Status) {
        return true;
    }

    return false;
}

TArray<AActor *> UBattleMoveFunctionCode::FilterInvalidTargets_Implementation(const FGameplayAbilitySpecHandle Handle,
                                                                              const FGameplayAbilityActorInfo &ActorInfo) {
    auto AbilitySystemComponent = ActorInfo.AbilitySystemComponent.Get();
    auto AbilitySpec = AbilitySystemComponent->FindAbilitySpecFromHandle(Handle);

    check(AbilitySpec->GameplayEventData != nullptr)
    return RangeHelpers::CreateRange(AbilitySpec->GameplayEventData->TargetData.Data)
        | ranges::views::transform([](const TSharedPtr<FGameplayAbilityTargetData> &Ptr)
            { return RangeHelpers::CreateRange(Ptr->GetActors()); })
        | ranges::views::join
        | ranges::views::transform([](const TWeakObjectPtr<AActor>& Actor) { return Actor.Get(); })
        | ranges::views::filter(&AActor::Implements<IBattler>)
        | ranges::views::filter([](AActor* Actor) {
            TScriptInterface<IBattler> Battler = Actor;
            return !Battler->IsFainted();
        })
        | RangeHelpers::TToArray<AActor*>();
}