// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Moves/BaseBattleMove.h"
#include "Battle/Battle.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/BattleSide.h"
#include "Battle/Type.h"
#include "DataManager.h"
#include "Moves/MoveData.h"
#include "Pokemon/Moves/Move.h"
#include "RangeHelpers.h"
#include "Battle/GameplayAbilities/BattlerAbilityComponent.h"
#include "Battle/GameplayAbilities/Attributes/DamageModificationAttributeSet.h"
#include "Battle/GameplayAbilities/Context/MoveEffectContext.h"
#include "Settings/BaseSettings.h"
#include <range/v3/view/filter.hpp>
#include <range/v3/view/transform.hpp>
#include "AbilitySystemBlueprintLibrary.h"
#include "Battle/GameplayAbilities/Attributes/CriticalHitRateModificationAttributeSet.h"
#include "Battle/Moves/MoveEvaluationHelpers.h"

static int32 ModifiedParameter(int32 Base, float Multiplier) {
    return FMath::Max(FMath::RoundToInt32(static_cast<float>(Base) * Multiplier), 1);
}

static void SendOutEventForBattler(const FGameplayTag &Tag, FGameplayEventData &EventData,
                                             AActor* BattlerActor) {
    EventData.Target = BattlerActor;
    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(BattlerActor, Tag, EventData);
}

static void SendOutMoveEvents(UMoveEffectContext *MoveEffectContext, const FGameplayTag& GlobalTag,
    const FGameplayTag& UserTag, const FGameplayTag& UserAllyTag, const FGameplayTag& TargetTag,
    const FGameplayTag& TargetAllyTag) {
    using namespace ranges::views;
    auto &DamageInfo = MoveEffectContext->GetDamageInfo();
    auto UserActor = CastChecked<AActor>(DamageInfo.User.GetObject());
    FGameplayEventData EventData;
    EventData.OptionalObject = MoveEffectContext;
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
    for (AActor *Ally : DamageInfo.User->GetAllies() | filter(AllyNotFainted) | transform(ConvertToActor)) {
    SendOutEventForBattler(GlobalTag, EventData, Ally);
        SendOutEventForBattler(UserAllyTag, EventData, Ally);
    }

    auto TargetActor = CastChecked<AActor>(DamageInfo.Target.GetObject());
    SendOutEventForBattler(GlobalTag, EventData, TargetActor);
    SendOutEventForBattler(TargetTag, EventData, TargetActor);
    for (AActor *Ally : DamageInfo.Target->GetAllies() | filter(AllyNotFainted) | transform(ConvertToActor)) {
        SendOutEventForBattler(GlobalTag, EventData, Ally);
        SendOutEventForBattler(TargetAllyTag, EventData, Ally);
    }
}

static void SendOutCriticalHitRateEvents(UMoveEffectContext *MoveEffectContext) {
    const static auto GlobalTag = FGameplayTag::RequestGameplayTag("Battle.Moves.CriticalHits.Scope.Global");
    const static auto UserTag = FGameplayTag::RequestGameplayTag("Battle.Moves.CriticalHits.Scope.User");
    const static auto UserAllyTag = FGameplayTag::RequestGameplayTag("Battle.Moves.CriticalHits.Scope.UserAlly");
    const static auto TargetTag = FGameplayTag::RequestGameplayTag("Battle.Moves.CriticalHits.Scope.Target");
    const static auto TargetAllyTag = FGameplayTag::RequestGameplayTag("Battle.Moves.CriticalHits.Scope.TargetAlly");
    SendOutMoveEvents(MoveEffectContext, GlobalTag, UserTag, UserAllyTag, TargetTag, TargetAllyTag);
}

static void SendOutDamageEvents(UMoveEffectContext *MoveEffectContext) {
    const static auto GlobalTag = FGameplayTag::RequestGameplayTag("Battle.Moves.Damage.Scope.Global");
    const static auto UserTag = FGameplayTag::RequestGameplayTag("Battle.Moves.Damage.Scope.User");
    const static auto UserAllyTag = FGameplayTag::RequestGameplayTag("Battle.Moves.Damage.Scope.UserAlly");
    const static auto TargetTag = FGameplayTag::RequestGameplayTag("Battle.Moves.Damage.Scope.Target");
    const static auto TargetAllyTag = FGameplayTag::RequestGameplayTag("Battle.Moves.Damage.Scope.TargetAlly");
    SendOutMoveEvents(MoveEffectContext, GlobalTag, UserTag, UserAllyTag, TargetTag, TargetAllyTag);
}

TScriptInterface<IBattleMove> UBaseBattleMove::Initialize(const TScriptInterface<IBattle> &Battle,
                                                          const TScriptInterface<IMove> &Move) {
    CurrentBattle = Battle;
    WrappedMove = Move;
    return this;
}

bool UBaseBattleMove::IsUsable_Implementation() const {
    return WrappedMove->GetCurrentPP() > 0;
}

TArray<TScriptInterface<IBattler>>
UBaseBattleMove::GetAllPossibleTargets_Implementation(const TScriptInterface<IBattler> &User) const {
    TArray<TScriptInterface<IBattler>> Targets;
    auto UserSide = User->GetOwningSide();
    auto UserId = User->GetInternalId();
    auto &Battle = UserSide->GetOwningBattle();
    return Battle->GetActiveBattlers() |
           ranges::views::filter(
               [&UserId](const TScriptInterface<IBattler> &Battler) { return Battler->GetInternalId() != UserId; }) |
           RangeHelpers::TToArray<TScriptInterface<IBattler>>();
}

FText UBaseBattleMove::GetDisplayName_Implementation() const {
    return WrappedMove->GetMoveData().RealName;
}

int32 UBaseBattleMove::GetCurrentPP_Implementation() const {
    return WrappedMove->GetCurrentPP();
}

int32 UBaseBattleMove::GetMaxPP_Implementation() const {
    return WrappedMove->GetTotalPP();
}

FName UBaseBattleMove::GetDisplayType_Implementation() const {
    return WrappedMove->GetType();
}

int32 UBaseBattleMove::GetPriority_Implementation() const {
    return WrappedMove->GetMoveData().Priority;
}

void UBaseBattleMove::PayCost_Implementation(int32 Amount) {
    WrappedMove->DecrementPP(Amount);
}

TScriptInterface<IBattle> UBaseBattleMove::GetOwningBattle_Implementation() const {
    return CurrentBattle;
}

bool UBaseBattleMove::IsConfusionAttack() const {
    return false;
}

bool UBaseBattleMove::HasHighCriticalHitRate() const {
    static const FName HighCriticalHitRate = TEXT("HighCriticalHitRate");
    return HasTag(HighCriticalHitRate);
}

bool UBaseBattleMove::HasTag(FName Tag) const {
    return WrappedMove->GetMoveData().Tags.Contains(Tag);
}

bool UBaseBattleMove::PerformHitCheck_Implementation(const TScriptInterface<IBattler> &User,
                                                     const TScriptInterface<IBattler> &Target) {
    auto BaseAccuracy = CalculateBaseAccuracy(WrappedMove->GetAccuracy(), User, Target);
    if (BaseAccuracy == FMoveData::GuaranteedHit) {
        return true;
    }

    int32 Threshold = BaseAccuracy;
    int32 Roll = FMath::Rand() % 100;
    return Roll < Threshold;
}

FBattleDamage UBaseBattleMove::CalculateDamage_Implementation(const TScriptInterface<IBattler> &User,
                                                              const TScriptInterface<IBattler> &Target,
                                                              int32 TargetCount) {
    auto UserAbilities = User->GetAbilityComponent();
    
    if (WrappedMove->GetDamageCategory() == EMoveDamageCategory::Status) {
        return {.Damage = 0, .Effectiveness = EDamageEffectiveness::NonDamaging};
    }

    auto Context = MakeShared<FMoveDamageInfo>(User, Target, TargetCount);
    Context->Type = DetermineType();

    CalculateTypeMatchups(Context->Effects, Target, Context->Type);
    if (Context->Effects.Effectiveness == EDamageEffectiveness::NoEffect) {
        return {.Damage = 0, .Effectiveness = EDamageEffectiveness::NoEffect};
    }

    auto MoveContext = NewObject<UMoveEffectContext>();
    MoveContext->SetDamageInfo(Context);
    auto CriticalHitRateAttributes = NewObject<UCriticalHitRateModificationAttributeSet>(UserAbilities);
    UserAbilities->AddSpawnedAttribute(CriticalHitRateAttributes);
    SendOutCriticalHitRateEvents(MoveContext);
    
    static const auto CriticalHitTag = FGameplayTag::RequestGameplayTag("Battle.Moves.Damage.CriticalHit");
    if (IsCritical(User, Target)) {
        Context->Effects.bCriticalHit = true;
        UserAbilities->AddLooseGameplayTag(CriticalHitTag);
    }
    
    Context->BaseDamage = CalculateBasePower(WrappedMove->GetBasePower(), User, Target);
    auto [Attack, Defense] = GetAttackAndDefense(User, Target);

    // If a critical hit is scored ignore any negative attack stages or positive defense stages
    if (Context->Effects.bCriticalHit) {
        Attack.Stages = FMath::Max(0, Attack.Stages);
        Defense.Stages = FMath::Min(0, Defense.Stages);
    }
    
    auto DamageModificationAttributes = NewObject<UDamageModificationAttributeSet>(UserAbilities);
    UserAbilities->AddSpawnedAttribute(DamageModificationAttributes);
    SendOutDamageEvents(MoveContext);

    FDamageMultiplierHandler MultiplierHandler(*DamageModificationAttributes);
    const auto &[PowerMultiplier, AttackMultiplier, DefenseMultiplier, FinalDamageMultiplier] = *MultiplierHandler.Multipliers;
    ApplyAdditionalDamageModifiers(MultiplierHandler, *Context);
    Context->BaseDamage = ModifiedParameter(Context->BaseDamage, PowerMultiplier);
    int32 AttackValue = ModifiedParameter(Attack.GetModifiedValue(), AttackMultiplier);
    int32 DefenseValue = ModifiedParameter(Defense.GetModifiedValue(), DefenseMultiplier);
    int32 Damage = CalculateBaseDamage(Context->BaseDamage, User->GetPokemonLevel(), AttackValue, DefenseValue);
    Damage = ModifiedParameter(Damage, FinalDamageMultiplier);
    
    UserAbilities->RemoveLooseGameplayTag(CriticalHitTag);
    UserAbilities->RemoveSpawnedAttribute(CriticalHitRateAttributes);
    UserAbilities->RemoveSpawnedAttribute(DamageModificationAttributes);
    return {
        .Damage = Damage,
        .Effectiveness = Context->Effects.Effectiveness,
        .bCriticalHit = Context->Effects.bCriticalHit
    };
}

bool UBaseBattleMove::IsCritical(const TScriptInterface<IBattler> &User,
    const TScriptInterface<IBattler> &Target) const {
    auto CriticalHitRateAttributes = CastChecked<UCriticalHitRateModificationAttributeSet>(User->GetAbilityComponent()
            ->GetAttributeSet(UCriticalHitRateModificationAttributeSet::StaticClass()));
    
    auto Override = GetCriticalOverride(User, Target);
    Override = UMoveEvaluationHelpers::ApplyCriticalHitOverride(Override,
        UMoveEvaluationHelpers::AttributeValueToOverride(CriticalHitRateAttributes->GetCriticalHitRateOverride()));
    switch (Override) {
    case ECriticalOverride::Always:
        return true;
    case ECriticalOverride::Never:
        return false;
    default:
        // Fallthrough and do nothing
        break;
    }

    auto Stage = static_cast<int32>(CriticalHitRateAttributes->GetCriticalHitStages());
    if (HasHighCriticalHitRate()) {
        Stage += 1;
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

ECriticalOverride UBaseBattleMove::GetCriticalOverride_Implementation(const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target) const {
    return ECriticalOverride::Normal;
}

void UBaseBattleMove::CalculateTypeMatchups_Implementation(FDamageEffects &Effects,
                                                           const TScriptInterface<IBattler> &Target, FName MoveType) {
    using enum EDamageEffectiveness;
    float Effectiveness = 1.0f;
    auto &TypeTable = FDataManager::GetInstance().GetDataTable<FType>();
    TArray<FType *> Types;
    Algo::Transform(Target->GetTypes(), Types, [&TypeTable](FName Type) { return TypeTable.GetData(Type); });
    for (auto Type : Types) {
        if (Type->Immunities.Contains(MoveType)) {
            Effects.TypeMatchUp = 0.0f;
            Effects.Effectiveness = NoEffect;
            return;
        }

        if (Type->Weaknesses.Contains(MoveType)) {
            Effectiveness *= 2.f;
        } else if (Type->Resistances.Contains(MoveType)) {
            Effectiveness /= 2.f;
        }
    }

    if (Effectiveness > 1.f) {
        Effects.Effectiveness = SuperEffective;
    } else if (Effectiveness < 1.f) {
        Effects.Effectiveness = NotVeryEffective;
    }
    Effects.TypeMatchUp *= Effectiveness;
}

FName UBaseBattleMove::DetermineType_Implementation() {
    return WrappedMove->GetType();
}

int32 UBaseBattleMove::CalculateBasePower_Implementation(int32 MovePower, const TScriptInterface<IBattler> &User,
                                                         const TScriptInterface<IBattler> &Target) {
    return MovePower;
}

int32 UBaseBattleMove::CalculateBaseDamage_Implementation(int32 Power, int32 Level, int32 Attack, int32 Defense) {
    return FMath::FloorToInt32(2.0f * static_cast<float>(Level) / 5 + 2) * Power * Attack / Defense / 50 + 2;
}

FAttackAndDefense UBaseBattleMove::GetAttackAndDefense_Implementation(const TScriptInterface<IBattler> &User,
                                                                      const TScriptInterface<IBattler> &Target) {
    using enum EMoveDamageCategory;
    auto Category = WrappedMove->GetDamageCategory();
    check(Category != Status)
    return Category == Special
               ? FAttackAndDefense{.Attack = User->GetSpecialAttack(), .Defense = Target->GetSpecialDefense()}
               : FAttackAndDefense{.Attack = User->GetAttack(), .Defense = Target->GetDefense()};
}

int32 UBaseBattleMove::CalculateBaseAccuracy_Implementation(int32 Accuracy, const TScriptInterface<IBattler> &User,
    const TScriptInterface<IBattler> &Target) const {
    return Accuracy;
}