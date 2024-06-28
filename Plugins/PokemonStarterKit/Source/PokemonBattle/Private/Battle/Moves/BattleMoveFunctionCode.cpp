// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Moves/BattleMoveFunctionCode.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "RangeHelpers.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/GameplayAbilities/Attributes/AccuracyModifiersAttributeSet.h"
#include "Battle/GameplayAbilities/Attributes/MoveUsageAttributes.h"
#include "Battle/Moves/BattleMove.h"
#include "Battle/Moves/MoveAnimation.h"
#include "Battle/Moves/MoveExecutor.h"
#include "Battle/Moves/UseMovePayload.h"
#include "Moves/MoveData.h"
#include "Moves/Target.h"
#include <range/v3/view/filter.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/transform.hpp>

UBattleMoveFunctionCode::UBattleMoveFunctionCode() {
    auto &AbilityTrigger = AbilityTriggers.Emplace_GetRef();
    AbilityTrigger.TriggerTag = FGameplayTag::RequestGameplayTag("Battle.UsingMove");
}

const TScriptInterface<IBattleMove> & UBattleMoveFunctionCode::GetMove() const {
    return BattleMove;
}

void UBattleMoveFunctionCode::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                              const FGameplayEventData *TriggerEventData) {
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    // If this is not triggered by an event throw an exception
    check(TriggerEventData != nullptr)
    BattleMove = CastChecked<UUseMovePayload>(TriggerEventData->OptionalObject)->Move;
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
    check(User.GetObject()->Implements<UBattler>())
    TArray<TScriptInterface<IBattler>> Targets;
    Targets.Reserve(TargetsActors.Num());
    for (auto Actor : TargetsActors) {
        Targets.Emplace(Actor);
    }

    UseMove(User, Targets);
}

TArray<AActor *> UBattleMoveFunctionCode::FilterInvalidTargets(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo &ActorInfo) {
    auto AbilitySystemComponent = ActorInfo.AbilitySystemComponent.Get();
    auto AbilitySpec = AbilitySystemComponent->FindAbilitySpecFromHandle(Handle);

    check(AbilitySpec->GameplayEventData != nullptr)
    return RangeHelpers::CreateRange(AbilitySpec->GameplayEventData->TargetData.Data)
        | ranges::views::transform([](const TSharedPtr<FGameplayAbilityTargetData> &Ptr)
            { return RangeHelpers::CreateRange(Ptr->GetActors()); })
        | ranges::views::join
        | ranges::views::transform([](const TWeakObjectPtr<AActor>& Actor) { return Actor.Get(); })
        | ranges::views::filter(&AActor::Implements<UBattler>)
        | ranges::views::filter([](AActor* Actor) {
            TScriptInterface<IBattler> Battler = Actor;
            return !Battler->IsFainted();
        })
        | RangeHelpers::TToArray<AActor*>();
}

void UBattleMoveFunctionCode::UseMove(const TScriptInterface<IBattler> &User,
    const TArray<TScriptInterface<IBattler>> &Targets) {
    FRunningMessageSet Messages;
    if (MoveFailed(User, Targets, Messages)) {
        ProcessMoveFailure(*Messages.Messages);
        return;
    }

    if (Targets.IsEmpty() && BattleMove->GetTargetType().NumTargets != ETargetCount::NoneOrSelf && !WorksWithNoTargets()) {
        Messages.Messages->Emplace(NSLOCTEXT("BattleMoveFunction", "NoTarget", "But there was no target..."));
        ProcessMoveFailure(*Messages.Messages);
        return;
    }
    
    auto TargetFailureCheckCallback = [this, &Messages, &User](const TScriptInterface<IBattler>& Target) {
        return FailsAgainstTarget(User, Target, Messages);
    };
    auto FilteredTargets = RangeHelpers::CreateRange(Targets)
        | ranges::views::filter(TargetFailureCheckCallback)
        | RangeHelpers::TToArray<TScriptInterface<IBattler>>();

    if (FilteredTargets.IsEmpty()) {
        ProcessMoveFailure(*Messages.Messages);
        return;
    }

    auto HitCheckCallback = [this, &Messages, &User](const TScriptInterface<IBattler>& Target) {
        return FailsAgainstTarget(User, Target, Messages);
    };
    auto SuccessfulHits = RangeHelpers::CreateRange(FilteredTargets)
        | ranges::views::filter(HitCheckCallback)
        | RangeHelpers::TToArray<TScriptInterface<IBattler>>();
    
    if (SuccessfulHits.IsEmpty()) {
        ProcessMoveFailure(*Messages.Messages);
        return;
    }

    DisplayMessagesAndAnimation(User, SuccessfulHits, *Messages.Messages);
}

bool UBattleMoveFunctionCode::MoveFailed_Implementation(const TScriptInterface<IBattler> &User,
    const TArray<TScriptInterface<IBattler>> &Targets, const FRunningMessageSet &FailureMessages) const {
    return false;
}

bool UBattleMoveFunctionCode::FailsAgainstTarget_Implementation(const TScriptInterface<IBattler> &User,
                                                                const TScriptInterface<IBattler>& Target, const FRunningMessageSet& FailureMessages) const {
    return false;
}

bool UBattleMoveFunctionCode::WorksWithNoTargets_Implementation() {
    return false;
}

bool UBattleMoveFunctionCode::HitCheck(const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target,
                                       const FRunningMessageSet &FailureMessages) {
    return true;
}

TArray<FText> & UBattleMoveFunctionCodeHelper::GetMessages(const FRunningMessageSet &Messages) {
    return *Messages.Messages;
}