﻿// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Moves/BattleMoveFunctionCode.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Battle/Animations/BattleAnimation.h"
#include "Battle/Animations/BattleAnimationGetter.h"
#include "Battle/Attributes/StatStagesAttributeSet.h"
#include "Battle/Attributes/TargetDamageStateAttributeSet.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Battlers/BattlerAbilityComponent.h"
#include "Battle/Battlers/BattlerHelpers.h"
#include "Battle/Effects/ApplyMessageGameplayEffectComponent.h"
#include "Battle/Events/Moves/AdditionalEffectChanceModificationPayload.h"
#include "Battle/Events/Moves/CriticalHitRateCalculationPayload.h"
#include "Battle/Events/Moves/DamageModificationPayload.h"
#include "Battle/Events/Moves/HitCheckPayload.h"
#include "Battle/Events/Moves/MoveTypeDeterminedPayload.h"
#include "Battle/Events/Moves/SuccessCheckAgainstTargetPayload.h"
#include "Battle/Events/Moves/UseMovePayload.h"
#include "Battle/Events/TargetedEvents.h"
#include "Battle/Moves/BattleMove.h"
#include "Battle/Moves/MoveTags.h"
#include "Battle/Settings/BattleMessageSettings.h"
#include "Battle/Settings/PokemonBattleSettings.h"
#include "Battle/Stats/StatTags.h"
#include "Battle/Type.h"
#include "Battle/Types/SingleTypeModPayload.h"
#include "Battle/Types/TypeMatchUpModPayload.h"
#include "Battle/Types/TypeTags.h"
#include "DataManager.h"
#include "Moves/MoveData.h"
#include "Moves/Target.h"
#include "PokemonBattleModule.h"
#include "RetroLib/Ranges/Algorithm/NameAliases.h"
#include "RetroLib/Ranges/Algorithm/To.h"
#include "RetroLib/Ranges/Views/Concat.h"
#include "RetroLib/Utils/MakeStrong.h"
#include "RetroLib/Utils/StringUtilities.h"
#include "Species/Stat.h"
#include "Utilities/PokemonCoroutineDispatcher.h"

int32 FCapturedBattleStat::GetStatValue() const {
    static auto &StatTable = FDataManager::GetInstance().GetDataTable<FStat>();
    auto Stat = StatTable.GetData(StatID);
    check(Stat != nullptr)
    check(Stat->BaseAttribute.IsValid())
    return FMath::FloorToInt32(OwningBattler->GetAbilityComponent()->GetNumericAttribute(Stat->BaseAttribute));
}

UBattleMoveFunctionCode::UBattleMoveFunctionCode() {
    auto &AbilityTrigger = AbilityTriggers.Emplace_GetRef();
    AbilityTrigger.TriggerTag = Pokemon::Battle::Moves::UsingMove;
}

const TScriptInterface<IBattleMove> &UBattleMoveFunctionCode::GetMove() const {
    return BattleMove;
}

bool UBattleMoveFunctionCode::ShouldAbilityRespondToEvent(const FGameplayAbilityActorInfo *ActorInfo,
                                                          const FGameplayEventData *Payload) const {
    auto MovePayload = CastChecked<UUseMovePayload>(Payload->OptionalObject);
    return MovePayload->Ability == this;
}

UE5Coro::GAS::FAbilityCoroutine UBattleMoveFunctionCode::ExecuteAbility(FGameplayAbilitySpecHandle Handle,
                                                                        const FGameplayAbilityActorInfo *ActorInfo,
                                                                        FGameplayAbilityActivationInfo ActivationInfo,
                                                                        const FGameplayEventData *TriggerEventData) {
    // If this is not triggered by an event throw an exception
    check(TriggerEventData != nullptr)
    BattleMove = CastChecked<UUseMovePayload>(TriggerEventData->OptionalObject)->Move;

    static auto &Lookup = Pokemon::Battle::Moves::FLookup::GetInstance();
    // clang-format off
    auto TagsList = BattleMove->GetTags() |
                    Retro::Ranges::Views::Transform([](FName Tag) -> FGameplayTag {
                        return Lookup.GetTag(Tag);
                    }) |
                    Retro::Ranges::To<TArray>();
    // clang-format on
    TagsList.Emplace(Pokemon::Battle::Moves::UsingMove);
    TagsList.Emplace(Pokemon::Battle::Moves::GetUserCategoryTag(BattleMove->GetCategory()));

    DeterminedType = DetermineType();
    static auto &TypeLookup = Pokemon::Battle::Types::FLookup::GetInstance();
    TagsList.Emplace(TypeLookup.GetMoveTypeUserTag(DeterminedType));

    AddedTags = FGameplayTagContainer::CreateFromArray(TagsList);
    ActorInfo->AbilitySystemComponent->AddLooseGameplayTags(AddedTags);

    check(ActorInfo != nullptr)
    auto TargetsActors = FilterInvalidTargets(Handle, *ActorInfo, TriggerEventData);

    TScriptInterface<IBattler> User = ActorInfo->OwnerActor.Get();
    TArray<TScriptInterface<IBattler>> Targets;
    Targets.Reserve(TargetsActors.Num());
    for (auto Actor : TargetsActors) {
        auto &Battler = Targets.Emplace_GetRef(Actor);
        auto &Tags = AddedTargetTags.Emplace(Actor);
        Tags.AddTag(Pokemon::Battle::Moves::MoveTarget);
        Tags.AddTag(TypeLookup.GetMoveTypeTargetTag(DeterminedType));
        Tags.AddTag(Pokemon::Battle::Moves::GetTargetCategoryTag(BattleMove->GetCategory()));
        Battler->GetAbilityComponent()->AddLooseGameplayTags(Tags);
    }

    check(User.GetObject()->Implements<UBattler>())
    auto TypePayload = NewObject<UMoveTypeDeterminedPayload>();
    TypePayload->User = User;
    TypePayload->Type = DeterminedType;
    co_await Pokemon::Battle::Events::SendOutMoveEvent(User, TypePayload, Pokemon::Battle::Moves::TypeDetermined);
    UE_LOG(LogBattle, Display, TEXT("%s determined to be %s-type!"), *BattleMove->GetDisplayName().ToString(),
           *DeterminedType.ToString())

    co_await UseMove(User, std::move(Targets));

    ActorInfo->AbilitySystemComponent->RemoveLooseGameplayTags(AddedTags, TNumericLimits<int32>::Max());
    for (auto &[Actor, Tags] : AddedTargetTags) {
        TScriptInterface<IBattler> Battler = Actor;
        auto AbilityComponent = Battler->GetAbilityComponent();
        AbilityComponent->RemoveLooseGameplayTags(Tags, TNumericLimits<int32>::Max());
        AbilityComponent->GetTargetDamageStateAttributeSet()->Reset();
    }
}

FName UBattleMoveFunctionCode::DetermineType_Implementation() const {
    return BattleMove->GetDisplayType();
}

TArray<AActor *> UBattleMoveFunctionCode::FilterInvalidTargets(const FGameplayAbilitySpecHandle Handle,
                                                               const FGameplayAbilityActorInfo &ActorInfo,
                                                               const FGameplayEventData *TriggerEventData) {
    // clang-format off
    return TriggerEventData->TargetData.Data |
           Retro::Ranges::Views::Transform(&FGameplayAbilityTargetData::GetActors) |
           Retro::Ranges::Views::Join |
           Retro::Ranges::Views::Transform(Retro::MakeStrongChecked) |
           Retro::Ranges::Views::Filter(Retro::InstanceOf<IBattler>) |
           Retro::Ranges::Views::Filter([](AActor *Actor) {
               TScriptInterface<IBattler> Battler = Actor;
               return !Battler->IsFainted();
           }) |
           Retro::Ranges::To<TArray>();
    // clang-format on
}

UE5Coro::TCoroutine<> UBattleMoveFunctionCode::UseMove(TScriptInterface<IBattler> User,
                                                       TArray<TScriptInterface<IBattler>> Targets,
                                                       FForceLatentCoroutine) {
    TArray<TScriptInterface<IBattler>> SuccessfulHits;
    if (!co_await CheckMoveSuccess(User, Targets, SuccessfulHits)) {
        ProcessMoveFailure(User);
        co_await EndMove(User, Targets);
        co_return;
    }

    co_await PlayAnimation(User, SuccessfulHits);
    co_await DealDamage(User, SuccessfulHits);
    auto FainedBattlers = co_await ApplyMoveEffects(User, SuccessfulHits);
    co_await ApplyAdditionalEffects(User, SuccessfulHits);
    co_await EndMove(User, SuccessfulHits, FainedBattlers);
}

UE5Coro::TCoroutine<bool> UBattleMoveFunctionCode::CheckMoveSuccess(TScriptInterface<IBattler> User,
                                                                    const TArray<TScriptInterface<IBattler>> &Targets,
                                                                    TArray<TScriptInterface<IBattler>> &SuccessfulHits,
                                                                    FForceLatentCoroutine) {
    UE_LOG(LogBattle, Display, TEXT("%s is using move %s!"), *User->GetNickname().ToString(),
           *BattleMove->GetDisplayName().ToString())
    if (co_await MoveFailed(User, Targets)) {
        UE_LOG(LogBattle, Display, TEXT("%s failed!"), *BattleMove->GetDisplayName().ToString())
        co_return false;
    }

    auto &Dispatcher = IPokemonCoroutineDispatcher::Get(User);
    auto Settings = GetDefault<UBattleMessageSettings>();
    if (Targets.IsEmpty() && BattleMove->GetTargetType().NumTargets != ETargetCount::NoneOrSelf &&
        !WorksWithNoTargets()) {
        co_await Dispatcher.DisplayMessage(Settings->NoTargetMessage);
        UE_LOG(LogBattle, Display, TEXT("%s has no targets!"), *BattleMove->GetDisplayName().ToString())
        co_return false;
    }

    TArray<TScriptInterface<IBattler>> FilteredTargets;
    FilteredTargets.Reserve(Targets.Num());
    for (auto Target : Targets) {
        Target->GetAbilityComponent()->GetTargetDamageStateAttributeSet()->Reset();
        if (!co_await SuccessCheckAgainstTarget(User, Target)) {
            Target->GetAbilityComponent()->AddLooseGameplayTag(Pokemon::Battle::Moves::MoveTarget_Unaffected_Failed);
            AddedTargetTags.FindChecked(CastChecked<AActor>(Target.GetObject()))
                .AddTag(Pokemon::Battle::Moves::MoveTarget_Unaffected_Failed);
            continue;
        }

        FilteredTargets.Add(Target);
    }

    if (!Targets.IsEmpty() && FilteredTargets.IsEmpty()) {
        UE_LOG(LogBattle, Display, TEXT("%s failed against all targets!"), *BattleMove->GetDisplayName().ToString())
        co_return false;
    }

    auto HitCheckCallback = [this, &User](const TScriptInterface<IBattler> &Target) {
        bool bHitResult = HitCheck(User, Target);
        if (!bHitResult) {
            UE_LOG(LogBattle, Display, TEXT("%s missed %s!"), *BattleMove->GetDisplayName().ToString(),
                   *Target->GetNickname().ToString())
            Target->GetAbilityComponent()->AddLooseGameplayTag(Pokemon::Battle::Moves::MoveTarget_Unaffected_Missed);
            AddedTargetTags.FindChecked(CastChecked<AActor>(Target.GetObject()))
                .AddTag(Pokemon::Battle::Moves::MoveTarget_Unaffected_Missed);
        }
        return bHitResult;
    };
    SuccessfulHits = FilteredTargets | Retro::Ranges::Views::Filter(HitCheckCallback) | Retro::Ranges::To<TArray>();

    if (!Targets.IsEmpty() && SuccessfulHits.IsEmpty()) {
        UE_LOG(LogBattle, Display, TEXT("%s missed all targets!"), *BattleMove->GetDisplayName().ToString())
        co_await Dispatcher.DisplayMessage(Settings->MoveMissedMessage);
        ProcessMoveFailure(User);
        co_await EndMove(User, Targets);
        co_return false;
    }

    co_return true;
}

UE5Coro::TCoroutine<bool> UBattleMoveFunctionCode::MoveFailed(TScriptInterface<IBattler> User,
                                                              const TArray<TScriptInterface<IBattler>> &Targets,
                                                              FForceLatentCoroutine) {
    co_return false;
}

void UBattleMoveFunctionCode::ProcessMoveFailure(const TScriptInterface<IBattler> &User) const {
    User->OnMoveFailed(BattleMove);
}

UE5Coro::TCoroutine<bool> UBattleMoveFunctionCode::SuccessCheckAgainstTarget(TScriptInterface<IBattler> User,
                                                                             TScriptInterface<IBattler> Target,
                                                                             FForceLatentCoroutine) {
    float TypeMod = CalculateTypeMatchUp(DeterminedType, User, Target);
    auto &TargetAbilities = *Target->GetAbilityComponent();
    auto &DamageState = *TargetAbilities.GetTargetDamageStateAttributeSet();
    DamageState.SetTypeMod(TypeMod);
    if (User->GetAbilityComponent()->HasMatchingGameplayTag(Pokemon::Battle::Moves::TwoTurnAttack)) {
        co_return true;
    }

    if (BattleMove->GetCategory() != EMoveDamageCategory::Status && FMath::IsNearlyZero(TypeMod)) {
        TargetAbilities.AddLooseGameplayTag(Pokemon::Battle::Moves::MoveTarget_Unaffected_NoEffect);
        AddedTargetTags.FindChecked(CastChecked<AActor>(Target.GetObject()))
            .AddTag(Pokemon::Battle::Moves::MoveTarget_Unaffected_NoEffect);
        UE_LOG(LogBattle, Display, TEXT("%s is unaffected by %s!"), *Target->GetNickname().ToString(),
               *BattleMove->GetDisplayName().ToString())
        co_return false;
    }

    if (co_await FailsAgainstTarget(User, Target)) {
        co_return false;
    }

    auto Payload = USuccessCheckAgainstTargetPayload::Create(BattleMove, User, Target);
    co_await Pokemon::Battle::Events::SendOutMoveEvents(User, Target, Payload,
                                                        Pokemon::Battle::Moves::SuccessCheckAgainstTarget);
    co_return true;
}

UE5Coro::TCoroutine<bool> UBattleMoveFunctionCode::FailsAgainstTarget(TScriptInterface<IBattler> User,
                                                                      TScriptInterface<IBattler> Target,
                                                                      FForceLatentCoroutine) {
    co_return false;
}

bool UBattleMoveFunctionCode::WorksWithNoTargets_Implementation() {
    return false;
}

bool UBattleMoveFunctionCode::HitCheck_Implementation(const TScriptInterface<IBattler> &User,
                                                      const TScriptInterface<IBattler> &Target) {
    auto BaseAccuracy = CalculateBaseAccuracy(BattleMove->GetAccuracy(), User, Target);
    if (BaseAccuracy == FMoveData::GuaranteedHit) {
        UE_LOG(LogBattle, Display, TEXT("%s always hits, bypassing accuracy check against %s"),
               *BattleMove->GetDisplayName().ToString(), *Target->GetNickname().ToString())
        return true;
    }

    auto Payload = UHitCheckPayload::Create(this, User, Target, BaseAccuracy);
    Pokemon::Battle::Events::SendOutMoveEvents(User, Target, Payload, Pokemon::Battle::Moves::HitCheckEvents);
    BaseAccuracy = Payload->GetData().BaseAccuracy;
    if (BaseAccuracy == FMoveData::GuaranteedHit) {
        return true;
    }

    static auto &StageInfo = GetDefault<UPokemonBattleSettings>()->StatStages;
    int32 StatStageBound = StageInfo.Num();
    int32 AccuracyStages = FMath::RoundToInt32(User->GetAbilityComponent()->GetStatStages()->GetAccuracyStages());
    int32 EvasionStages = FMath::RoundToInt32(Target->GetAbilityComponent()->GetStatStages()->GetEvasionStages());
    int32 CompositeStages = FMath::Clamp(AccuracyStages - EvasionStages, -StatStageBound, StatStageBound);
    float AccuracyMultiplier;
    if (CompositeStages > 0) {
        AccuracyMultiplier = StageInfo[CompositeStages - 1].PositiveAccEvaMultiplier;
    } else if (CompositeStages < 0) {
        AccuracyMultiplier = StageInfo[-CompositeStages - 1].NegativeAccEvaMultiplier;
    } else {
        AccuracyMultiplier = 1.f;
    }

    int32 Threshold = FMath::RoundToInt32(BaseAccuracy * AccuracyMultiplier);
    UE_LOG(LogBattle, Display, TEXT("Accuracy threshold for %s is %d"), *Target->GetNickname().ToString(), Threshold)
    int32 Roll = FMath::Rand() % 100;
    return Roll < Threshold;
}

int32 UBattleMoveFunctionCode::CalculateBaseAccuracy_Implementation(int32 Accuracy,
                                                                    const TScriptInterface<IBattler> &User,
                                                                    const TScriptInterface<IBattler> &Target) {
    return Accuracy;
}

UE5Coro::TCoroutine<> UBattleMoveFunctionCode::PlayAnimation(TScriptInterface<IBattler> User,
                                                             const TArray<TScriptInterface<IBattler>> &Targets,
                                                             FForceLatentCoroutine) {
    // TODO: Retrieve the move animation
    co_return;
}

UE5Coro::TCoroutine<> UBattleMoveFunctionCode::DealDamage(TScriptInterface<IBattler> User,
                                                          const TArray<TScriptInterface<IBattler>> &Targets,
                                                          FForceLatentCoroutine) {
    if (BattleMove->GetCategory() != EMoveDamageCategory::Status) {
        for (auto &Target : Targets) {
            CalculateDamageAgainstTarget(User, Target, Targets.Num());
        }
    }

    // This is the list of tags to ignore when dealing out damage
    static const auto UnaffectedTagsFilter = FGameplayTagContainer::CreateFromArray(TArray<FGameplayTag>{
        Pokemon::Battle::Moves::MoveTarget_Unaffected, Pokemon::Battle::Moves::MoveTarget_NoDamage});

    UE_LOG(LogBattle, Display, TEXT("Dealing damage for %s!"), *BattleMove->GetDisplayName().ToString())

    TSubclassOf<UGameplayEffect> DealDamageEffect =
        co_await UE5Coro::Latent::AsyncLoadClass(TSoftClassPtr(GetDefault<UPokemonBattleSettings>()->DealDamageEffect));
    check(DealDamageEffect != nullptr);

    auto Handle = GetCurrentAbilitySpecHandle();
    auto ActorInfo = GetActorInfo();
    auto &ActivationInfo = GetCurrentActivationInfoRef();
    for (auto &Target : Targets) {
        auto TargetAbilities = Target->GetAbilityComponent();
        if (TargetAbilities->HasAnyMatchingGameplayTags(UnaffectedTagsFilter)) {
            UE_LOG(LogBattle, Display, TEXT("%s is unaffected by the move, skipping damage step!"),
                   *Target->GetNickname().ToString())
            continue;
        }

        auto TargetData = MakeUnique<FGameplayAbilityTargetData_ActorArray>();
        TargetData->SetActors({CastChecked<AActor>(Target.GetObject())});
        FGameplayAbilityTargetDataHandle TargetDataHandle(TargetData.Release());
        auto EffectSpec = MakeOutgoingGameplayEffectSpec(Handle, &ActorInfo, ActivationInfo, DealDamageEffect);
        auto DamageState = TargetAbilities->GetTargetDamageStateAttributeSet();
        EffectSpec.Data->SetSetByCallerMagnitude(Pokemon::Battle::Moves::DamageDealtValue,
                                                 -DamageState->GetCalculatedDamage());
        auto Applied =
            ApplyGameplayEffectSpecToTarget(Handle, &ActorInfo, ActivationInfo, EffectSpec, TargetDataHandle);
    }

    co_await DisplayDamage(Targets);
}

int32 UBattleMoveFunctionCode::CalculateBasePower_Implementation(int32 Power, const TScriptInterface<IBattler> &User,
                                                                 const TScriptInterface<IBattler> &Target) {
    return Power;
}

void UBattleMoveFunctionCode::CalculateDamageAgainstTarget_Implementation(const TScriptInterface<IBattler> &User,
                                                                          const TScriptInterface<IBattler> &Target,
                                                                          int32 TargetCount) {
    check(BattleMove->GetCategory() != EMoveDamageCategory::Status)
    auto TargetAbilityComponent = Target->GetAbilityComponent();
    auto &DamageState = *TargetAbilityComponent->GetTargetDamageStateAttributeSet();
    if (IsCritical(User, Target)) {
        UE_LOG(LogBattle, Display, TEXT("Critical hit against %s!"), *Target->GetNickname().ToString());
        TargetAbilityComponent->AddLooseGameplayTag(Pokemon::Battle::Moves::MoveTarget_CriticalHit);
    }

    int32 BasePower = CalculateBasePower(BattleMove->GetBasePower(), User, Target);
    auto Payload = UDamageModificationPayload::Create(User, Target, TargetCount, DeterminedType, BasePower);
    SYNC_EVENT(Pokemon::Battle::Events::SendOutMoveEvents(User, Target, Payload,
                                                          Pokemon::Battle::Moves::DamageModificationEvents))
    auto &PayloadData = Payload->GetData();
    float NewFinal = PayloadData.FinalDamageMultiplier;
    NewFinal *= User->GetAbilityComponent()->GetStatStages()->GetSameTypeAttackBonus();
    NewFinal *= DamageState.GetTypeMod();
    Payload->SetFinalDamageMultiplier(NewFinal);
    ApplyAdditionalDamageMultipliers(Payload);

    auto [Attack, Defense] = GetAttackAndDefense(User, Target);

    static auto &Lookup = Pokemon::Battle::Stats::FLookup::Get();
    if (TargetAbilityComponent->HasMatchingGameplayTag(Pokemon::Battle::Moves::MoveTarget_CriticalHit)) {
        Attack.OwningBattler->GetAbilityComponent()->AddLooseGameplayTag(Lookup.GetIgnoreNegativeTag(Attack.StatID));
        Defense.OwningBattler->GetAbilityComponent()->AddLooseGameplayTag(Lookup.GetIgnorePositiveTag(Defense.StatID));
    }

    BasePower = FMath::Max(FMath::RoundToInt32(BasePower * PayloadData.PowerMultiplier), 1);
    int32 AttackValue = FMath::Max(FMath::RoundToInt32(Attack.GetStatValue() * PayloadData.AttackMultiplier), 1);
    int32 DefenseValue = FMath::Max(FMath::RoundToInt32(Defense.GetStatValue() * PayloadData.DefenseMultiplier), 1);

    int32 Level = User->GetPokemonLevel();
    int32 Damage =
        FMath::FloorToInt32(2.0f * static_cast<float>(Level) / 5 + 2) * BasePower * AttackValue / DefenseValue / 50 + 2;
    Damage = FMath::Max(FMath::RoundToInt32(Damage * PayloadData.FinalDamageMultiplier), 1);
    DamageState.SetCalculatedDamage(static_cast<float>(Damage));
    UE_LOG(LogBattle, Display, TEXT("%s calculated to take %d damage"), *Target->GetNickname().ToString(), Damage);

    if (TargetAbilityComponent->HasMatchingGameplayTag(Pokemon::Battle::Moves::MoveTarget_CriticalHit)) {
        Attack.OwningBattler->GetAbilityComponent()->RemoveLooseGameplayTag(Lookup.GetIgnoreNegativeTag(Attack.StatID));
        Defense.OwningBattler->GetAbilityComponent()->RemoveLooseGameplayTag(
            Lookup.GetIgnorePositiveTag(Defense.StatID));
    }
}

FAttackAndDefenseStats
UBattleMoveFunctionCode::GetAttackAndDefense_Implementation(const TScriptInterface<IBattler> &User,
                                                            const TScriptInterface<IBattler> &Target) {
    using enum EMoveDamageCategory;
    auto Category = BattleMove->GetCategory();
    check(Category != Status)

    if (Category == Special) {
        static FName SpecialAttack = "SPECIAL_ATTACK";
        static FName SpecialDefense = "SPECIAL_DEFENSE";
        return {{User, SpecialAttack}, {Target, SpecialDefense}};
    }

    static FName Attack = "ATTACK";
    static FName Defense = "DEFENSE";
    return {{User, Attack}, {Target, Defense}};
}

ECriticalOverride
UBattleMoveFunctionCode::GetCriticalOverride_Implementation(const TScriptInterface<IBattler> &User,
                                                            const TScriptInterface<IBattler> &Target) {
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
    SYNC_EVENT(Pokemon::Battle::Events::SendOutMoveEvents(User, Target, Payload,
                                                          Pokemon::Battle::Types::FullTypeMatchUpEvents))
    return Payload->GetData().Multiplier;
}

float UBattleMoveFunctionCode::CalculateSingleTypeMod_Implementation(FName AttackingType, FName DefendingType,
                                                                     const TScriptInterface<IBattler> &User,
                                                                     const TScriptInterface<IBattler> &Target) {
    float Effectiveness = UTypeHelper::GetTypeEffectiveness(AttackingType, DefendingType);
    auto Payload = USingleTypeModPayload::Create(User, Target, AttackingType, DefendingType, Effectiveness);
    SYNC_EVENT(Pokemon::Battle::Events::SendOutMoveEvents(User, Target, Payload,
                                                          Pokemon::Battle::Types::SingleTypeModifierEvents));
    return Payload->GetData().Multiplier;
}

bool UBattleMoveFunctionCode::IsCritical_Implementation(const TScriptInterface<IBattler> &User,
                                                        const TScriptInterface<IBattler> &Target) {
    auto StatStagesAttributes = User->GetAbilityComponent()->GetStatStages();
    auto Override = GetCriticalOverride(User, Target);
    auto Stage = static_cast<int32>(StatStagesAttributes->GetCriticalHitStages());
    auto Payload = UCriticalHitRateCalculationPayload::Create(this, User, Target, Override, Stage);
    SYNC_EVENT(Pokemon::Battle::Events::SendOutMoveEvents(User, Target, Payload,
                                                          Pokemon::Battle::Moves::CriticalHitRateModEvents));
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

    auto &Ratios = GetDefault<UPokemonBattleSettings>()->CriticalHitRatios;
    Stage = FMath::Clamp(Stage, 0, Ratios.Num() - 1);

    int32 Rate = Ratios[Stage];
    check(Rate > 0)
    if (Rate == 1) {
        return true;
    }
    int32 Roll = FMath::Rand() % Rate;
    return Roll == 0;
}

UE5Coro::TCoroutine<> UBattleMoveFunctionCode::DisplayDamage(const TArray<TScriptInterface<IBattler>> &Targets) {
    if (Targets.IsEmpty()) {
        co_return;
    }

    auto &Anims = UBattleAnimationGetter::Get(Targets[0].GetObject());
    auto DamageAnimation = Anims.GetDamageAnimation(Targets);
    co_await IBattleAnimation::PlayAnimation(DamageAnimation);

    if (BattleMove->GetCategory() == EMoveDamageCategory::Status) {
        co_return;
    }

    constexpr auto CriticalHit = [](const TScriptInterface<IBattler> &Battler) {
        return Battler->GetAbilityComponent()->HasMatchingGameplayTag(Pokemon::Battle::Moves::MoveTarget_CriticalHit);
    };

    auto CriticalHits = Targets | Retro::Ranges::Views::Filter(CriticalHit) |
                        Retro::Ranges::Views::Transform(&IBattler::GetNickname) | Retro::Ranges::To<TArray>();

    auto &Dispatcher = IPokemonCoroutineDispatcher::Get(Targets[0].GetObject());
    auto Messages = GetDefault<UBattleMessageSettings>();
    if (!CriticalHits.IsEmpty()) {
        co_await Dispatcher.DisplayMessage(
            Targets.Num() > 1
                ? FText::FormatNamed(Messages->CriticalHitMessageMulti, "Pkmn",
                                     UStringUtilities::GenerateList(CriticalHits, UStringUtilities::ConjunctionAnd))
                : Messages->CriticalHitMessage);
    }

    TArray<FText> SuperEffectiveHits;
    TArray<FText> NotVeryEffectiveHits;
    for (auto &Target : Targets) {
        float Mod = Target->GetAbilityComponent()->GetTargetDamageStateAttributeSet()->GetTypeMod();

        if (auto Effectiveness = Pokemon::TypeEffectiveness::GetEffectivenessFromMultiplier(Mod);
            Effectiveness == EDamageEffectiveness::SuperEffective) {
            SuperEffectiveHits.Add(Target->GetNickname());
        } else if (Effectiveness == EDamageEffectiveness::NotVeryEffective) {
            NotVeryEffectiveHits.Add(Target->GetNickname());
        }
    }

    if (Targets.Num() > 1) {
        if (!SuperEffectiveHits.IsEmpty()) {
            co_await Dispatcher.DisplayMessage(FText::FormatNamed(
                Messages->SuperEffectiveMessageMulti, "Pkmn",
                UStringUtilities::GenerateList(SuperEffectiveHits, UStringUtilities::ConjunctionAnd)));
        }

        if (!NotVeryEffectiveHits.IsEmpty()) {
            co_await Dispatcher.DisplayMessage(FText::FormatNamed(
                Messages->NotVeryEffectiveMessageMulti, "Pkmn",
                UStringUtilities::GenerateList(NotVeryEffectiveHits, UStringUtilities::ConjunctionAnd)));
        }
    } else {
        if (!SuperEffectiveHits.IsEmpty()) {
            co_await Dispatcher.DisplayMessage(Messages->SuperEffectiveMessage);
        }

        if (!NotVeryEffectiveHits.IsEmpty()) {
            co_await Dispatcher.DisplayMessage(Messages->NotVeryEffectiveMessage);
        }
    }
}

UE5Coro::TCoroutine<TArray<TScriptInterface<IBattler>>>
UBattleMoveFunctionCode::ApplyMoveEffects(TScriptInterface<IBattler> User,
                                          const TArray<TScriptInterface<IBattler>> &Targets, FForceLatentCoroutine) {
    UE_LOG(LogBattle, Display, TEXT("Applying post-damage effects of %s!"), *BattleMove->GetDisplayName().ToString())
    for (auto &Target : Targets) {
        if (Target->GetAbilityComponent()->HasMatchingGameplayTag(Pokemon::Battle::Moves::MoveTarget_Unaffected)) {
            UE_LOG(LogBattle, Display, TEXT("%s is unaffected by the move, skipping post-damage effect application!"),
                   *Target->GetNickname().ToString())
            continue;
        }

        UE_LOG(LogBattle, Display, TEXT("Applying post-damage effects of %s to %s!"),
               *BattleMove->GetDisplayName().ToString(), *Target->GetNickname().ToString())
        co_await ApplyEffectWhenDealingDamage(User, Target);
    }

    UE_LOG(LogBattle, Display, TEXT("Applying effects of %s!"), *BattleMove->GetDisplayName().ToString())
    for (auto &Target : Targets) {
        if (Target->GetAbilityComponent()->HasMatchingGameplayTag(Pokemon::Battle::Moves::MoveTarget_Unaffected)) {
            UE_LOG(LogBattle, Display, TEXT("%s is unaffected by the move, skipping effect application!"),
                   *Target->GetNickname().ToString())
            continue;
        }

        UE_LOG(LogBattle, Display, TEXT("Applying effects of %s to %s!"), *BattleMove->GetDisplayName().ToString(),
               *Target->GetNickname().ToString())
        co_await ApplyEffectAgainstTarget(User, Target);
    }

    co_await ApplyGeneralEffect(User);
    co_return co_await FaintCheck(User, Targets);
}

UE5Coro::TCoroutine<> UBattleMoveFunctionCode::ApplyEffectWhenDealingDamage(TScriptInterface<IBattler> User,
                                                                            TScriptInterface<IBattler> Target,
                                                                            FForceLatentCoroutine) {
    // No effect in this method
    co_return;
}

UE5Coro::TCoroutine<> UBattleMoveFunctionCode::ApplyEffectAgainstTarget(TScriptInterface<IBattler> User,
                                                                        TScriptInterface<IBattler> Target,
                                                                        FForceLatentCoroutine) {
    // No effect in this method
    co_return;
}

UE5Coro::TCoroutine<> UBattleMoveFunctionCode::ApplyGeneralEffect(TScriptInterface<IBattler> User,
                                                                  FForceLatentCoroutine) {
    // No effect in this method
    co_return;
}

UE5Coro::TCoroutine<TArray<TScriptInterface<IBattler>>>
UBattleMoveFunctionCode::FaintCheck(TScriptInterface<IBattler> User, const TArray<TScriptInterface<IBattler>> &Targets,
                                    FForceLatentCoroutine) {
    using FAnim = TScriptInterface<IBattleAnimation>;
    constexpr auto GetFaintAnimation = &UBattleAnimationGetter::GetFaintAnimation;

    auto &Anims = UBattleAnimationGetter::Get(User.GetObject());
    auto FaintedBattlers = Retro::Ranges::Views::Concat(Retro::Ranges::Views::Single(User), Targets) |
                           Retro::Ranges::Views::Filter(&IBattler::IsFainted) | Retro::Ranges::To<TArray>();
    auto Animations =
        FaintedBattlers | Retro::Ranges::Views::Transform(Retro::BindMethod<GetFaintAnimation>(std::ref(Anims))) |
        Retro::Ranges::Views::Transform([](const FAnim &Anim) { return IBattleAnimation::PlayAnimation(Anim); }) |
        Retro::Ranges::To<TArray>();

    co_await WhenAll(Animations);
    co_return FaintedBattlers;
}

UE5Coro::TCoroutine<> UBattleMoveFunctionCode::ApplyAdditionalEffects(TScriptInterface<IBattler> User,
                                                                      const TArray<TScriptInterface<IBattler>> &Targets,
                                                                      FForceLatentCoroutine) {
    UE_LOG(LogBattle, Display, TEXT("Applying additional effects of %s!"), *BattleMove->GetDisplayName().ToString())
    for (auto &Target : Targets) {
        if (Target->GetAbilityComponent()->GetTargetDamageStateAttributeSet()->GetCalculatedDamage() == 0.f) {
            UE_LOG(LogBattle, Display, TEXT("%s didn't take any damage, skipping the additional effect chance!"),
                   *Target->GetNickname().ToString())
            continue;
        }

        int32 Chance = CalculateAdditionalEffectChance(User, Target);
        UE_LOG(LogBattle, Display, TEXT("%s's additional effect chance against %s calculated to be %d"),
               *BattleMove->GetDisplayName().ToString(), *Target->GetNickname().ToString(), Chance)
        if (Chance <= 0) {
            UE_LOG(LogBattle, Display, TEXT("%s's additional effect chance is 0, skipping!"),
                   *BattleMove->GetDisplayName().ToString())
            continue;
        }

        int32 Roll = FMath::Rand() % 100;
        if (Roll < Chance) {
            UE_LOG(LogBattle, Display, TEXT("Applying additional effect of %s to %s!"),
                   *BattleMove->GetDisplayName().ToString(), *Target->GetNickname().ToString())
            co_await ApplyAdditionalEffect(User, Target);
        }
    }

    // TODO: Apparently flinching is done after additional effects

    UE_LOG(LogBattle, Display, TEXT("%s has finished, display messages and concluding!"),
           *BattleMove->GetDisplayName().ToString())
}

UE5Coro::TCoroutine<> UBattleMoveFunctionCode::ApplyAdditionalEffect(TScriptInterface<IBattler> User,
                                                                     TScriptInterface<IBattler> Target) {
    // No effect in this method
    co_return;
}

int32 UBattleMoveFunctionCode::CalculateAdditionalEffectChance_Implementation(
    const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target) {
    auto Payload =
        UAdditionalEffectChanceModificationPayload::Create(User, Target, BattleMove->GetAdditionalEffectChance());
    SYNC_EVENT(Pokemon::Battle::Events::SendOutMoveEvents(User, Target, Payload,
                                                          Pokemon::Battle::Moves::AdditionalEffectChanceEvents));
    return FMath::RoundToInt32(Payload->GetData().AdditionalEffectChance);
}

UE5Coro::TCoroutine<> UBattleMoveFunctionCode::EndMove(const TScriptInterface<IBattler> &User,
                                                       const TArray<TScriptInterface<IBattler>> &Targets,
                                                       const TArray<TScriptInterface<IBattler>> &FaintedBattlers,
                                                       FForceLatentCoroutine) {
    co_await UBattlerHelpers::GainExpOnFaint(User.GetObject(), FaintedBattlers);
}