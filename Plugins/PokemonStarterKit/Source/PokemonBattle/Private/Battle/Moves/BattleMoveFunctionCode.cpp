// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Moves/BattleMoveFunctionCode.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "PokemonBattleModule.h"
#include "RangeHelpers.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Events/TargetedEvents.h"
#include "Battle/GameplayAbilities/BattlerAbilityComponent.h"
#include "Battle/GameplayAbilities/Attributes/AccuracyModifiersAttributeSet.h"
#include "Battle/GameplayAbilities/Attributes/MoveUsageAttributeSet.h"
#include "Battle/GameplayAbilities/Attributes/StatStagesAttributeSet.h"
#include "Battle/GameplayAbilities/Attributes/TargetDamageStateAttributeSet.h"
#include "Battle/Moves/BattleMove.h"
#include "Battle/Moves/CriticalHitRateCalculationPayload.h"
#include "Battle/Moves/DamageModificationPayload.h"
#include "Battle/Moves/HitCheckPayload.h"
#include "Battle/Moves/MoveAnimation.h"
#include "Battle/Moves/MoveEvaluationHelpers.h"
#include "Battle/Moves/MoveExecutor.h"
#include "Battle/Moves/MoveTags.h"
#include "Battle/Moves/SuccessCheckAgainstTargetPayload.h"
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
    static auto &Lookup = Pokemon::Battle::Moves::FLookup::GetInstance();
    auto TagsList = RangeHelpers::CreateRange(BattleMove->GetTags())
        | ranges::views::transform([](FName Tag) -> FGameplayTag { return Lookup.GetTag(Tag); })
        | RangeHelpers::TToArray<FGameplayTag>();
    AddedTags = FGameplayTagContainer::CreateFromArray(TagsList);
    ActorInfo->AbilitySystemComponent->AddLooseGameplayTags(AddedTags);
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

void UBattleMoveFunctionCode::EndAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo *ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
    bool bReplicateEndAbility, bool bWasCancelled) {
    Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
    ActorInfo->AbilitySystemComponent->RemoveLooseGameplayTags(AddedTags);
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
        bool bSuccess = SuccessCheckAgainstTarget(User, Target, Messages);
        if (!bSuccess) {
            Target->GetAbilityComponent()->AddLooseGameplayTag(Pokemon::Battle::Moves::MoveTarget_Unaffected_Failed);
        }
        return bSuccess;
    };
    auto FilteredTargets = RangeHelpers::CreateRange(Targets)
                           | ranges::views::filter(TargetFailureCheckCallback)
                           | RangeHelpers::TToArray<TScriptInterface<IBattler>>();

    if (FilteredTargets.IsEmpty()) {
        ProcessMoveFailure(*Messages.Messages);
        return;
    }

    auto HitCheckCallback = [this, &User](const TScriptInterface<IBattler> &Target) {
        bool bHitResult = HitCheck(User, Target);
        if (!bHitResult) {
            Target->GetAbilityComponent()->AddLooseGameplayTag(Pokemon::Battle::Moves::MoveTarget_Unaffected_Missed);
        }
        return bHitResult;
    };
    auto SuccessfulHits = RangeHelpers::CreateRange(FilteredTargets)
                          | ranges::views::filter(HitCheckCallback)
                          | RangeHelpers::TToArray<TScriptInterface<IBattler>>();

    if (SuccessfulHits.IsEmpty()) {
        Messages.Messages->Emplace(NSLOCTEXT("BattleMove", "HitCheckFailed", "But it missed!"));
        ProcessMoveFailure(*Messages.Messages);
        return;
    }

    if (BattleMove->GetCategory() != EMoveDamageCategory::Status) {
        for (auto &Target : SuccessfulHits) {
            CalculateDamageAgainstTarget(User, Target, SuccessfulHits.Num(), Messages);
        }
    }

    DisplayMessagesAndAnimation(User, SuccessfulHits, *Messages.Messages);
}

bool UBattleMoveFunctionCode::MoveFailed_Implementation(const TScriptInterface<IBattler> &User,
                                                        const TArray<TScriptInterface<IBattler>> &Targets,
                                                        const FRunningMessageSet &FailureMessages) const {
    return false;
}

bool UBattleMoveFunctionCode::SuccessCheckAgainstTarget_Implementation(const TScriptInterface<IBattler> &User,
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

    auto Payload = USuccessCheckAgainstTargetPayload::Create(BattleMove, User, Target, PreDamageMessages);
    Pokemon::Battle::Events::SendOutMoveEvents(User, Target, Payload, Pokemon::Battle::Moves::SuccessCheckAgainstTarget);
    return true;
}

bool UBattleMoveFunctionCode::FailsAgainstTarget_Implementation(const TScriptInterface<IBattler> &User,
                                                                const TScriptInterface<IBattler> &Target,
                                                                const FRunningMessageSet &FailureMessages) const {
    return false;
}

bool UBattleMoveFunctionCode::WorksWithNoTargets_Implementation() {
    return false;
}

bool UBattleMoveFunctionCode::HitCheck_Implementation(const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target) {
    auto BaseAccuracy = CalculateBaseAccuracy(BattleMove->GetAccuracy(), User, Target);
    if (BaseAccuracy == FMoveData::GuaranteedHit) {
        return true;
    }
    
    auto Payload = UHitCheckPayload::Create(this, User, Target, BaseAccuracy);
    Pokemon::Battle::Events::SendOutMoveEvents(User, Target, Payload, Pokemon::Battle::Moves::HitCheckEvents);
    BaseAccuracy = Payload->GetData().BaseAccuracy;
    if (BaseAccuracy == FMoveData::GuaranteedHit) {
        return true;
    }
    
    int32 Accuracy = FMath::RoundToInt32(User->GetAbilityComponent()->GetCoreAttributes()->GetAccuracy());
    int32 Evasion = FMath::RoundToInt32(Target->GetAbilityComponent()->GetCoreAttributes()->GetEvasion());
    Evasion = FMath::Max(Evasion, 1);
    
    int32 Threshold = BaseAccuracy * Accuracy / Evasion;
    int32 Roll = FMath::Rand() % 100;
    return Roll < Threshold;
}

int32 UBattleMoveFunctionCode::CalculateBaseAccuracy_Implementation(int32 Accuracy,
    const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target) {
    return Accuracy;
}

void UBattleMoveFunctionCode::DealDamage(const TScriptInterface<IBattler> &User,
                                         const TArray<TScriptInterface<IBattler>> &Targets) {
    // This is the list of tags to ignore when dealing out damage
    static const auto UnaffectedTagsFilter = FGameplayTagContainer::CreateFromArray(TArray<FGameplayTag>{
        Pokemon::Battle::Moves::MoveTarget_Unaffected,
        Pokemon::Battle::Moves::MoveTarget_NoDamage
    });
    
}

int32 UBattleMoveFunctionCode::CalculateBasePower_Implementation(int32 Power, const TScriptInterface<IBattler> &User,
    const TScriptInterface<IBattler> &Target) {
    return Power;
}

void UBattleMoveFunctionCode::CalculateDamageAgainstTarget_Implementation(
    const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target,
    int32 TargetCount, const FRunningMessageSet &PreDamageMessages) {
    check(BattleMove->GetCategory() != EMoveDamageCategory::Status)
    auto TargetAbilityComponent = Target->GetAbilityComponent();
    auto &DamageState = *TargetAbilityComponent->GetTargetDamageStateAttributeSet();
    if (IsCritical(User, Target)) {
        TargetAbilityComponent->AddLooseGameplayTag(Pokemon::Battle::Moves::MoveTarget_CriticalHit);
    }

    int32 BasePower = CalculateBasePower(BattleMove->GetBasePower(), User, Target);
    auto Payload = UDamageModificationPayload::Create(User, Target, TargetCount, PreDamageMessages, DeterminedType, BasePower);
    Pokemon::Battle::Events::SendOutMoveEvents(User, Target, Payload, Pokemon::Battle::Moves::DamageModificationEvents);

    auto [Attack, Defense] = GetAttackAndDefense(User, Target);
    
    auto &PayloadData = Payload->GetData();
    BasePower = FMath::Max(FMath::RoundToInt32(BasePower * PayloadData.PowerMultiplier), 1);
    Attack = FMath::Max(FMath::RoundToInt32(Attack * PayloadData.AttackMultiplier), 1);
    Defense = FMath::Max(FMath::RoundToInt32(Defense * PayloadData.DefenseMultiplier), 1);

    int32 Level = User->GetPokemonLevel();
    int32 Damage = FMath::FloorToInt32(2.0f * static_cast<float>(Level) / 5 + 2) * BasePower * Attack / Defense / 50 + 2;
    Damage = FMath::Max(FMath::RoundToInt32(Damage * PayloadData.FinalDamageMultiplier), 1);
    DamageState.SetCalculatedDamage(static_cast<float>(Damage));
}

FAttackAndDefenseStats UBattleMoveFunctionCode::GetAttackAndDefense_Implementation(const TScriptInterface<IBattler> &User,
    const TScriptInterface<IBattler> &Target) {
    using enum EMoveDamageCategory;
    auto Category = BattleMove->GetCategory();
    check(Category != Status)

    if (Category == Special) {
        return {
            FMath::FloorToInt32(User->GetAbilityComponent()->GetCoreAttributes()->GetSpecialAttack()),
            FMath::FloorToInt32(Target->GetAbilityComponent()->GetCoreAttributes()->GetSpecialDefense())
        };
    }
    
    return {
        FMath::FloorToInt32(User->GetAbilityComponent()->GetCoreAttributes()->GetAttack()),
        FMath::FloorToInt32(Target->GetAbilityComponent()->GetCoreAttributes()->GetDefense())
    };
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

    auto Payload = UTypeMatchUpModPayload::Create(User, Target, MoveType, Effectiveness);
    Pokemon::Battle::Events::SendOutMoveEvents(User, Target, Payload, Pokemon::Battle::Types::FullTypeMatchUpEvents);
    return Payload->GetData().Multiplier;
}

float UBattleMoveFunctionCode::CalculateSingleTypeMod_Implementation(FName AttackingType, FName DefendingType,
                                                                     const TScriptInterface<IBattler> &User,
                                                                     const TScriptInterface<IBattler> &Target) {
    float Effectiveness = UTypeHelper::GetTypeEffectiveness(AttackingType, DefendingType);
    auto Payload = USingleTypeModPayload::Create(User, Target, AttackingType, DefendingType, Effectiveness);
    Pokemon::Battle::Events::SendOutMoveEvents(User, Target, Payload, Pokemon::Battle::Types::SingleTypeModifierEvents);
    return Payload->GetData().Multiplier;
}

bool UBattleMoveFunctionCode::IsCritical_Implementation(const TScriptInterface<IBattler> &User,
                                         const TScriptInterface<IBattler> &Target) {
    auto StatStagesAttributes = User->GetAbilityComponent()->GetStatStages();
    auto Override = GetCriticalOverride(User, Target);
    auto Stage = static_cast<int32>(StatStagesAttributes->GetCriticalHitStages());
    auto Payload = UCriticalHitRateCalculationPayload::Create(this, User, Target, Override, Stage);
    Pokemon::Battle::Events::SendOutMoveEvents(User, Target, Payload, Pokemon::Battle::Moves::CriticalHitRateModEvents);
    auto &Data = Payload->GetData();
    Override = Data.Override;
    Stage = Data.CriticalHitRateStages;
    
    switch (Override) {
    case ECriticalOverride::Always:
        return true;
    case ECriticalOverride::Never:
        return false;
    default:
        // Fallthrough and do nothing
        break;
    }

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