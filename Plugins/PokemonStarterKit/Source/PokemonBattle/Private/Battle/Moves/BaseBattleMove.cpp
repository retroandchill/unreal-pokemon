// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Moves/BaseBattleMove.h"
#include "Battle/Battle.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/BattleSide.h"
#include "Battle/Traits/Damage/DamageModificationTrait.h"
#include "Battle/Traits/TraitUtilities.h"
#include "Battle/Type.h"
#include "DataManager.h"
#include "Moves/MoveData.h"
#include "Pokemon/Moves/Move.h"
#include "RangeHelpers.h"
#include "Settings/BaseSettings.h"

static int32 ModifiedParameter(int32 Base, float Multiplier) {
    return FMath::Max(FMath::RoundToInt32(static_cast<float>(Base) * Multiplier), 1);
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
               [&UserId](const TScriptInterface<IBattler> &Battler) { return Battler->GetInternalId() == UserId; }) |
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

void UBaseBattleMove::PayCost_Implementation() {
    WrappedMove->DecrementPP();
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
    if (WrappedMove->GetDamageCategory() == EMoveDamageCategory::Status) {
        return {.Damage = 0, .Effectiveness = EDamageEffectiveness::NonDamaging};
    }
    FMoveDamageInfo Context = {.User = User, .Target = Target, .TargetCount = TargetCount};

    Context.Type = DetermineType();
    CalculateTypeMatchups(Context.Effects, Target, Context.Type);
    if (Context.Effects.Effectiveness == EDamageEffectiveness::NoEffect) {
        return {.Damage = 0, .Effectiveness = EDamageEffectiveness::NoEffect};
    }

    Context.Effects.bCriticalHit = IsCritical(User, Target);
    Context.BaseDamage = CalculateBasePower(WrappedMove->GetBasePower(), User, Target);
    auto [Attack, Defense] = GetAttackAndDefense(User, Target);

    FDamageMultipliers Multipliers;
    CalculateDamageMultipliers(Multipliers, Context);
    Context.BaseDamage = ModifiedParameter(Context.BaseDamage, Multipliers.PowerMultiplier);
    Attack = ModifiedParameter(Attack, Multipliers.AttackMultiplier);
    Defense = ModifiedParameter(Defense, Multipliers.DefenseMultiplier);
    int32 Damage = CalculateBaseDamage(Context.BaseDamage, User->GetPokemonLevel(), Attack, Defense);
    Damage = ModifiedParameter(Damage, Multipliers.FinalDamageMultiplier);

    return {
        .Damage = Damage, .Effectiveness = Context.Effects.Effectiveness, .bCriticalHit = Context.Effects.bCriticalHit};
}

bool UBaseBattleMove::IsCritical(const TScriptInterface<IBattler> &User,
    const TScriptInterface<IBattler> &Target) const {
    int32 Stage = Traits::ApplyIndividualCriticalHitRateModifications(User, Target);
    switch (GetCriticalOverride(User, Target)) {
    case ECriticalOverride::Always:
        return true;
    case ECriticalOverride::Never:
        return false;
    default:
        // Fallthrough and do nothing
        break;
    }

    if (HasHighCriticalHitRate()) {
        Stage += 1;
    }

    auto &Ratios = Pokemon::FBaseSettings::Get().GetCriticalHitRatios();
    Stage = FMath::Min(Stage, Ratios.Num() - 1);

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

void UBaseBattleMove::CalculateDamageMultipliers(FDamageMultipliers &Multipliers, const FMoveDamageInfo &Context) {
    Traits::ApplyIndividualDamageModifications(Multipliers, Context);
    ApplyMultiTargetModifier(Multipliers, Context.TargetCount);
    ApplyCriticalHitModifier(Multipliers, Context.Effects);
    ApplyDamageSwing(Multipliers);
    ApplyStabModifiers(Multipliers, Context.User, Context.Type);
    ApplyTypeMatchUps(Multipliers, Context.Effects);
    ApplyAdditionalDamageModifiers(Multipliers, Context);
}

void UBaseBattleMove::ApplyCriticalHitModifier_Implementation(FDamageMultipliers &Multipliers,
                                                              const FDamageEffects &Effects) {
    if (Effects.bCriticalHit) {
        Multipliers.FinalDamageMultiplier *= 1.5f;
    }
}

void UBaseBattleMove::ApplyMultiTargetModifier_Implementation(FDamageMultipliers &Multipliers, int32 TargetCount) {
    if (TargetCount > 1) {
        Multipliers.FinalDamageMultiplier *= 0.75f;
    }
}

void UBaseBattleMove::ApplyDamageSwing_Implementation(FDamageMultipliers &Multipliers) {
    if (!IsConfusionAttack()) {
        Multipliers.FinalDamageMultiplier *= static_cast<float>(FMath::RandRange(85, 100)) / 100.0f;
    }
}

void UBaseBattleMove::ApplyStabModifiers_Implementation(FDamageMultipliers &Multipliers,
                                                        const TScriptInterface<IBattler> &User, FName MoveType) {
    if (User->GetTypes().Contains(MoveType)) {
        // TODO: Account for adaptability/terrastilization
        Multipliers.FinalDamageMultiplier *= 1.5f;
    }
}

void UBaseBattleMove::ApplyTypeMatchUps_Implementation(FDamageMultipliers &Multipliers, const FDamageEffects &Effects) {
    Multipliers.FinalDamageMultiplier *= Effects.TypeMatchUp;
}

void UBaseBattleMove::ApplyAdditionalDamageModifiers_Implementation(FDamageMultipliers &Multipliers,
                                                                    const FMoveDamageInfo &Context) {
    // Currently there are no additional modifiers that need to be applied
}