// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Moves/BattleMoveFunctionCode.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "PokemonBattleModule.h"
#include "RangeHelpers.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/GameplayAbilities/BattlerAbilityComponent.h"
#include "Battle/GameplayAbilities/Attributes/AccuracyModifiersAttributeSet.h"
#include "Battle/GameplayAbilities/Attributes/MoveUsageAttributeSet.h"
#include "Battle/GameplayAbilities/Attributes/TargetDamageStateAttributeSet.h"
#include "Battle/Moves/BattleMove.h"
#include "Battle/Moves/MoveAnimation.h"
#include "Battle/Moves/MoveEvaluationHelpers.h"
#include "Battle/Moves/MoveExecutor.h"
#include "Battle/Moves/MoveTags.h"
#include "Battle/Moves/TargetSuccessCheckPayload.h"
#include "Battle/Moves/UseMovePayload.h"
#include "Battle/Types/SingleTypeModPayload.h"
#include "Battle/Types/TypeMatchUpModPayload.h"
#include "Battle/Types/TypeTags.h"
#include "Moves/MoveData.h"
#include "Moves/Target.h"
#include "Settings/BaseSettings.h"
#include <range/v3/view/filter.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/transform.hpp>

UBattleMoveFunctionCode::UBattleMoveFunctionCode() {
    auto &AbilityTrigger = AbilityTriggers.Emplace_GetRef();
    AbilityTrigger.TriggerTag = Pokemon::Battle::Moves::UsingMove;
}

const TScriptInterface<IBattleMove> &UBattleMoveFunctionCode::GetMove() const {
    return BattleMove;
}

void UBattleMoveFunctionCode::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo *ActorInfo,
                                              const FGameplayAbilityActivationInfo ActivationInfo,
                                              const FGameplayEventData *TriggerEventData) {
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    // If this is not triggered by an event throw an exception
    check(TriggerEventData != nullptr)
    BattleMove = CastChecked<UUseMovePayload>(TriggerEventData->OptionalObject)->Move;
}

bool UBattleMoveFunctionCode::CheckCost(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo *ActorInfo,
                                        FGameplayTagContainer *OptionalRelevantTags) const {
    return BattleMove->GetCurrentPP() > 0 && Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags);
}

void UBattleMoveFunctionCode::ApplyCost(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo *ActorInfo,
                                        const FGameplayAbilityActivationInfo ActivationInfo) const {
    Super::ApplyCost(Handle, ActorInfo, ActivationInfo);

    auto AttributeSet = ActorInfo->AbilitySystemComponent->GetAttributeSet(CostAttribute.GetAttributeSetClass());
    check(AttributeSet != nullptr)
    BattleMove->PayCost(FMath::FloorToInt32(CostAttribute.GetNumericValue(AttributeSet)));
}

void UBattleMoveFunctionCode::CommitExecute(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo *ActorInfo,
                                            const FGameplayAbilityActivationInfo ActivationInfo) {
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
           | ranges::views::transform([](const TSharedPtr<FGameplayAbilityTargetData> &Ptr) {
               return RangeHelpers::CreateRange(Ptr->GetActors());
           })
           | ranges::views::join
           | ranges::views::transform([](const TWeakObjectPtr<AActor> &Actor) {
               return Actor.Get();
           })
           | ranges::views::filter(&AActor::Implements<UBattler>)
           | ranges::views::filter([](AActor *Actor) {
               TScriptInterface<IBattler> Battler = Actor;
               return !Battler->IsFainted();
           })
           | RangeHelpers::TToArray<AActor *>();
}

void UBattleMoveFunctionCode::UseMove(const TScriptInterface<IBattler> &User,
                                      const TArray<TScriptInterface<IBattler>> &Targets) {
    FRunningMessageSet Messages;
    if (MoveFailed(User, Targets, Messages)) {
        ProcessMoveFailure(*Messages.Messages);
        return;
    }

    if (Targets.IsEmpty() && BattleMove->GetTargetType().NumTargets != ETargetCount::NoneOrSelf && !
        WorksWithNoTargets()) {
        Messages.Messages->Emplace(NSLOCTEXT("BattleMoveFunction", "NoTarget", "But there was no target..."));
        ProcessMoveFailure(*Messages.Messages);
        return;
    }

    auto TargetFailureCheckCallback = [this, &Messages, &User](const TScriptInterface<IBattler> &Target) {
        return SuccessCheckAgainstTarget(User, Target, Messages);
    };
    auto FilteredTargets = RangeHelpers::CreateRange(Targets)
                           | ranges::views::filter(TargetFailureCheckCallback)
                           | RangeHelpers::TToArray<TScriptInterface<IBattler>>();

    if (FilteredTargets.IsEmpty()) {
        ProcessMoveFailure(*Messages.Messages);
        return;
    }

    auto HitCheckCallback = [this, &Messages, &User](const TScriptInterface<IBattler> &Target) {
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
                                                        const TArray<TScriptInterface<IBattler>> &Targets,
                                                        const FRunningMessageSet &FailureMessages) const {
    return false;
}

bool UBattleMoveFunctionCode::FailsAgainstTarget_Implementation(const TScriptInterface<IBattler> &User,
                                                                const TScriptInterface<IBattler> &Target,
                                                                const FRunningMessageSet &FailureMessages) const {
    return false;
}

bool UBattleMoveFunctionCode::WorksWithNoTargets_Implementation() {
    return false;
}

void UBattleMoveFunctionCode::DealDamage(const TScriptInterface<IBattler> &User,
                                         const TArray<TScriptInterface<IBattler>> &Targets) {

}

void UBattleMoveFunctionCode::CalculateDamageAgainstTarget_Implementation(
    const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target,
    int32 TargetCount, const FRunningMessageSet &PreDamageMessages) {
    if (BattleMove->GetCategory() == EMoveDamageCategory::Status) {
        return;
    }

}

ECriticalOverride UBattleMoveFunctionCode::GetCriticalOverride_Implementation(const TScriptInterface<IBattler> &User,
                                                                              const TScriptInterface<IBattler> &
                                                                              Target) {
    return ECriticalOverride::Normal;
}

float UBattleMoveFunctionCode::CalculateTypeMatchUp_Implementation(FName MoveType,
                                                                   const TScriptInterface<IBattler> &User,
                                                                   const TScriptInterface<IBattler> &Target) {
    float Effectiveness = Pokemon::TypeEffectiveness::NormalEffectiveMultiplier;
    if (MoveType.IsNone()) {
        return Effectiveness;
    }

    for (auto DefendingType : Target->GetTypes()) {
        Effectiveness *= CalculateSingleTypeMod(MoveType, DefendingType, User, Target);
    }

    auto Payload = UTypeMatchUpModPayload::Create(MoveType, Effectiveness);
    FGameplayEventData EventData = CurrentEventData;
    EventData.Target = CastChecked<AActor>(Target.GetObject());
    EventData.OptionalObject = Payload;
    SendGameplayEvent(Pokemon::Battle::Types::FullTypeMatchUpEvent, EventData);
    return Payload->GetData().Multiplier;
}

float UBattleMoveFunctionCode::CalculateSingleTypeMod_Implementation(FName AttackingType, FName DefendingType,
                                                                     const TScriptInterface<IBattler> &User,
                                                                     const TScriptInterface<IBattler> &Target) {
    float Effectiveness = UTypeHelper::GetTypeEffectiveness(AttackingType, DefendingType);
    auto Payload = USingleTypeModPayload::Create(AttackingType, DefendingType, Effectiveness);
    FGameplayEventData EventData = CurrentEventData;
    EventData.Target = CastChecked<AActor>(Target.GetObject());
    EventData.OptionalObject = Payload;
    SendGameplayEvent(Pokemon::Battle::Types::SingleTypeModifierEvent, EventData);
    return Payload->GetData().Multiplier;
}

bool UBattleMoveFunctionCode::SuccessCheckAgainstTarget(const TScriptInterface<IBattler> &User,
    const TScriptInterface<IBattler> &Target, const FRunningMessageSet &PreDamageMessages) {
    float TypeMod = CalculateTypeMatchUp(DeterminedType, User, Target);
    auto &DamageState = *Target->GetAbilityComponent()->GetTargetDamageStateAttributeSet();
    DamageState.SetTypeMod(TypeMod);
    if (User->GetAbilityComponent()->HasMatchingGameplayTag(Pokemon::Battle::Moves::TwoTurnAttack)) {
        return true;
    }

    if (FailsAgainstTarget(User, Target, PreDamageMessages)) {
        return false;
    }

    auto Payload = UTargetSuccessCheckPayload::Create(BattleMove, PreDamageMessages);
    FGameplayEventData EventData = CurrentEventData;
    EventData.Target = CastChecked<AActor>(Target.GetObject());
    EventData.OptionalObject = Payload;
    SendGameplayEvent(Pokemon::Battle::Moves::SuccessCheckAgainstTarget, EventData);
    return true;
}

bool UBattleMoveFunctionCode::HitCheck(const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target,
                                       const FRunningMessageSet &FailureMessages) {
    return true;
}

bool UBattleMoveFunctionCode::IsCritical(const TScriptInterface<IBattler> &User,
                                         const TScriptInterface<IBattler> &Target) {
    auto MoveUsageAttributes = User->GetAbilityComponent()->GetMoveUsageAttributeSet();
    auto Override = GetCriticalOverride(User, Target);
    Override = UMoveEvaluationHelpers::ApplyCriticalHitOverride(Override,
                                                                UMoveEvaluationHelpers::AttributeValueToOverride(
                                                                    MoveUsageAttributes->GetCriticalHitRateOverride()));
    switch (Override) {
    case ECriticalOverride::Always:
        return true;
    case ECriticalOverride::Never:
        return false;
    default:
        // Fallthrough and do nothing
        break;
    }

    auto Stage = static_cast<int32>(MoveUsageAttributes->GetCriticalHitStages());
    auto &Ratios = Pokemon::FBaseSettings::Get().GetCriticalHitRatios();
    Stage = FMath::Clamp(Stage, 0, Ratios.Num() - 1);

    int32 Rate = Ratios[Stage];
    check(Rate > 0)
    if (Rate == 1) {
        return true;
    }
    int32 Roll = FMath::Rand() % Rate;
    return Roll == 0;
}

TArray<FText> &UBattleMoveFunctionCodeHelper::GetMessages(const FRunningMessageSet &Messages) {
    return *Messages.Messages;
}