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
#include <range/v3/view/filter.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/transform.hpp>

static void SendOutEventForBattler(const FGameplayTag &Tag, FGameplayEventData &EventData,
                                             AActor* BattlerActor) {
    EventData.Target = BattlerActor;
    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(BattlerActor, Tag, EventData);
}

static void SendOutMoveEvents(const TScriptInterface<IBattler>& User, const TScriptInterface<IBattler>& Target,
    const FGameplayTag& GlobalTag, const FGameplayTag& UserTag, const FGameplayTag& UserAllyTag,
    const FGameplayTag& TargetTag, const FGameplayTag& TargetAllyTag) {
    using namespace ranges::views;
    auto UserActor = CastChecked<AActor>(User.GetObject());
    FGameplayEventData EventData;
    auto TargetData = MakeShared<FGameplayAbilityTargetData_ActorArray>();
    TargetData->TargetActorArray.Add(UserActor);
    EventData.TargetData.Data.Emplace(MoveTemp(TargetData));

    auto ConvertToActor = [](const TScriptInterface<IBattler>& Battler) {
        return CastChecked<AActor>(Battler.GetObject());
    };
    auto AllyNotFainted = [](const TScriptInterface<IBattler>& Battler) {
        return !Battler->IsFainted();
    };
    
    SendOutEventForBattler(GlobalTag, EventData, UserActor);
    SendOutEventForBattler(UserTag, EventData, UserActor);
    for (AActor *Ally : User->GetAllies() | filter(AllyNotFainted) | transform(ConvertToActor)) {
    SendOutEventForBattler(GlobalTag, EventData, Ally);
        SendOutEventForBattler(UserAllyTag, EventData, Ally);
    }

    auto TargetActor = CastChecked<AActor>(Target.GetObject());
    SendOutEventForBattler(GlobalTag, EventData, TargetActor);
    SendOutEventForBattler(TargetTag, EventData, TargetActor);
    for (AActor *Ally : Target->GetAllies() | filter(AllyNotFainted) | transform(ConvertToActor)) {
        SendOutEventForBattler(GlobalTag, EventData, Ally);
        SendOutEventForBattler(TargetAllyTag, EventData, Ally);
    }
}

static void SendOutAccuracyCheckEvents(const TScriptInterface<IBattler>& User, const TScriptInterface<IBattler>& Target) {
    const static auto GlobalTag = FGameplayTag::RequestGameplayTag("Battle.Moves.AccuracyCheck.Scope.Global");
    const static auto UserTag = FGameplayTag::RequestGameplayTag("Battle.Moves.AccuracyCheck.Scope.User");
    const static auto UserAllyTag = FGameplayTag::RequestGameplayTag("Battle.Moves.AccuracyCheck.Scope.UserAlly");
    const static auto TargetTag = FGameplayTag::RequestGameplayTag("Battle.Moves.AccuracyCheck.Scope.Target");
    const static auto TargetAllyTag = FGameplayTag::RequestGameplayTag("Battle.Moves.AccuracyCheck.Scope.TargetAlly");
    SendOutMoveEvents(User, Target, GlobalTag, UserTag, UserAllyTag, TargetTag, TargetAllyTag);
}

static void SendOutCriticalHitRateEvents(const TScriptInterface<IBattler>& User, const TScriptInterface<IBattler>& Target) {
    const static auto GlobalTag = FGameplayTag::RequestGameplayTag("Battle.Moves.CriticalHits.Scope.Global");
    const static auto UserTag = FGameplayTag::RequestGameplayTag("Battle.Moves.CriticalHits.Scope.User");
    const static auto UserAllyTag = FGameplayTag::RequestGameplayTag("Battle.Moves.CriticalHits.Scope.UserAlly");
    const static auto TargetTag = FGameplayTag::RequestGameplayTag("Battle.Moves.CriticalHits.Scope.Target");
    const static auto TargetAllyTag = FGameplayTag::RequestGameplayTag("Battle.Moves.CriticalHits.Scope.TargetAlly");
    SendOutMoveEvents(User, Target, GlobalTag, UserTag, UserAllyTag, TargetTag, TargetAllyTag);
}

static void SendOutDamageEvents(const TScriptInterface<IBattler>& User, const TScriptInterface<IBattler>& Target) {
    const static auto GlobalTag = FGameplayTag::RequestGameplayTag("Battle.Moves.Damage.Scope.Global");
    const static auto UserTag = FGameplayTag::RequestGameplayTag("Battle.Moves.Damage.Scope.User");
    const static auto UserAllyTag = FGameplayTag::RequestGameplayTag("Battle.Moves.Damage.Scope.UserAlly");
    const static auto TargetTag = FGameplayTag::RequestGameplayTag("Battle.Moves.Damage.Scope.Target");
    const static auto TargetAllyTag = FGameplayTag::RequestGameplayTag("Battle.Moves.Damage.Scope.TargetAlly");
    SendOutMoveEvents(User, Target, GlobalTag, UserTag, UserAllyTag, TargetTag, TargetAllyTag);
}

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
    BattleMove = CastChecked<UUseMovePayload>(TriggerEventData->OptionalObject)->Move;

    auto World = ActorInfo->OwnerActor->GetWorld();
    Executor = World->SpawnActor<AMoveExecutor>();
    Animation = World->SpawnActor<AMoveAnimation>();

    User = ActorInfo->OwnerActor.Get();
    check(User.GetObject()->Implements<UBattler>())
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
    
    Targets.Reset();
    Targets.Reserve(TargetsActors.Num());
    for (auto Actor : TargetsActors) {
        Targets.Emplace(Actor);
    }

    Executor->GetCompleteCheckDelegate().BindLambda(&UBattleMoveFunctionCode::OnFailureCheckComplete,
        this, Handle, ActorInfo, ActivationInfo);
    Executor->PerformGlobalFailureCheck(User, Targets);
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

void UBattleMoveFunctionCode::OnFailureCheckComplete(const FGameplayAbilitySpecHandle Handle,
                                                     const FGameplayAbilityActorInfo *ActorInfo,
                                                     const FGameplayAbilityActivationInfo ActivationInfo,
                                                     bool bSuccess) {
    if (!bSuccess) {
        return EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
    }

    Executor->GetCompleteCheckDelegate().BindLambda(&UBattleMoveFunctionCode::OnTargetValidationComplete,
        this, Handle, ActorInfo, ActivationInfo);
}

void UBattleMoveFunctionCode::OnTargetValidationComplete(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bSuccess) {
    if (!bSuccess) {
        return EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
    }

    check(!Targets.IsEmpty())
    ValidTargets.Reset();
    ValidTargets.Reserve(Targets.Num());
    Executor->GetCompleteCheckDelegate().BindLambda(&UBattleMoveFunctionCode::CheckFailureOnNextTarget, this,
            Handle, ActorInfo, ActivationInfo, 0);
    Executor->PerformSuccessCheckOnTarget(User, Targets[0], Targets.Num());
}

void UBattleMoveFunctionCode::CheckFailureOnNextTarget(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, int32 Index,
    bool bSuccess) {
    check(Targets.IsValidIndex(Index))
    if (bSuccess) {
        bSucceededAgainstTarget = true;
        check(Index > 0)
        ValidTargets.Add(Targets[Index - 1]);
    }

    if (Index < Targets.Num() - 1) {
        Executor->GetCompleteCheckDelegate().BindLambda(&UBattleMoveFunctionCode::CheckFailureOnNextTarget, this,
            Handle, ActorInfo, ActivationInfo, Index + 1);
        Executor->PerformSuccessCheckOnTarget(User, Targets[Index], Targets.Num());
    } else {
        if (!bSucceededAgainstTarget) {
            return EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        }

        auto MoveUsageAttributes = ActorInfo->AbilitySystemComponent->GetSet<UMoveUsageAttributes>();
        auto AccuracyModifiers = NewObject<UAccuracyModifiersAttributeSet>(this);
        ActorInfo->AbilitySystemComponent->AddSpawnedAttribute(AccuracyModifiers);
        check(MoveUsageAttributes != nullptr)
        Targets = RangeHelpers::CreateRange(ValidTargets)
            | ranges::views::filter([this, MoveUsageAttributes, AccuracyModifiers](const TScriptInterface<IBattler>& Target)
                { return AccuracyCheck(*MoveUsageAttributes, *AccuracyModifiers, Target); })
            | RangeHelpers::TToArray<TScriptInterface<IBattler>>();
        
        if (Targets.IsEmpty()) {
            Executor->GetOnExecutorDispatchComplete().BindLambda(&EndAbility, this, Handle, ActorInfo, ActivationInfo, true, true);
            return Executor->ProcessMiss();
        }

        Animation->GetOnMoveAnimationComplete().BindLambda(&DamageCalculation, Handle, ActorInfo, ActivationInfo);
        Animation->PlayAnimation();
    }
}

bool UBattleMoveFunctionCode::AccuracyCheck(const UMoveUsageAttributes& MoveUsageAttributes,
                                            UAccuracyModifiersAttributeSet& AccuracyModifiers, const TScriptInterface<IBattler> &Target) const {
    SendOutAccuracyCheckEvents(User, Target);
    auto BaseAccuracy = FMath::FloorToInt32(MoveUsageAttributes.GetAccuracy());
    if (BaseAccuracy == FMoveData::GuaranteedHit) {
        return true;
    }

    int32 Threshold = BaseAccuracy;
    int32 Roll = FMath::Rand() % 100;
    return Roll < Threshold;
}

void UBattleMoveFunctionCode::DamageCalculation(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const {
    check(DamageEffect != nullptr)

    FGameplayAbilityTargetDataHandle TargetDataHandle;
    auto &Target = TargetDataHandle.Data.Emplace_GetRef(MakeShared<FGameplayAbilityTargetData_ActorArray>());
    Target->SetActors(RangeHelpers::CreateRange(Targets)
        | ranges::views::transform([](const TScriptInterface<IBattler>& T) { return CastChecked<AActor>(T.GetObject()); })
        | RangeHelpers::TToArray<TWeakObjectPtr<AActor>>());
    
    auto EffectHandles = ApplyGameplayEffectToTarget(Handle, ActorInfo, ActivationInfo, TargetDataHandle, DamageEffect, 1);
}