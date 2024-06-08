// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Moves/BaseBattleMove.h"
#include "DataManager.h"
#include "Battle/Type.h"
#include "Battle/Battlers/Battler.h"
#include "Pokemon/Moves/Move.h"

static int32 ModifiedParameter(int32 Base, float Multiplier) {
    return FMath::Max(FMath::RoundToInt32(Base * Multiplier), 1);
}

TScriptInterface<IBattleMove> UBaseBattleMove::Initialize(const TScriptInterface<IMove> &Move) {
    WrappedMove = Move;
    return this;
}

FBattleDamage UBaseBattleMove::CalculateDamage_Implementation(const TScriptInterface<IBattler> &User,
    const TScriptInterface<IBattler> &Target, int32 TargetCount) const {
    if (WrappedMove->GetDamageCategory() == EMoveDamageCategory::Status) {
            return { .Damage = 0, .Effectiveness = EDamageEffectiveness::NonDamaging };
    }

    auto MoveType = DetermineType();
    FDamageEffects Effects;
    CalculateTypeMatchups(Effects, Target, MoveType);
    if (Effects.Effectiveness == EDamageEffectiveness::NoEffect) {
        return { .Damage = 0, .Effectiveness = EDamageEffectiveness::NoEffect };
    }
    
    // TODO: Roll crit
    int32 BasePower = CalculateBasePower(WrappedMove->GetBasePower(), User, Target);
    auto [Attack, Defense] = GetAttackAndDefense(User, Target);

    FDamageMultipliers Multipliers;
    CalculateDamageMultipliers(Multipliers, User, Target, TargetCount, MoveType, BasePower, Effects);
    BasePower = ModifiedParameter(BasePower, Multipliers.PowerMultiplier);
    Attack = ModifiedParameter(Attack, Multipliers.AttackMultiplier);
    Defense = ModifiedParameter(Defense, Multipliers.DefenseMultiplier);
    int32 Damage = CalculateBaseDamage(BasePower, User->GetLevel(), Attack, Defense);
    Damage = ModifiedParameter(Damage, Multipliers.FinalDamageMultiplier);

    return {.Damage = Damage, .Effectiveness = Effects.Effectiveness, .bCriticalHit = Effects.bCriticalHit };
}

void UBaseBattleMove::CalculateTypeMatchups_Implementation(FDamageEffects &Effects,
    const TScriptInterface<IBattler> &Target, FName MoveType) const {
    using enum EDamageEffectiveness;
    float Effectiveness = 1.0f;
    auto &TypeTable = FDataManager::GetInstance().GetDataTable<FType>();
    TArray<FType*> Types;
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


FName UBaseBattleMove::DetermineType_Implementation() const {
    return WrappedMove->GetType();
}

int32 UBaseBattleMove::CalculateBasePower_Implementation(int32 MovePower, const TScriptInterface<IBattler> &User,
    const TScriptInterface<IBattler> &Target) const {
    return MovePower;
}

int32 UBaseBattleMove::CalculateBaseDamage_Implementation(int32 Power, int32 Level, int32 Attack, int32 Defense) const {
    return FMath::FloorToInt32(2.0f * Level / 5 + 2) * Power * Attack / Defense / 50 + 2;
}

FAttackAndDefense UBaseBattleMove::GetAttackAndDefense_Implementation(const TScriptInterface<IBattler> &User,
    const TScriptInterface<IBattler> &Target) const {
    using enum EMoveDamageCategory;
    auto Category = WrappedMove->GetDamageCategory();
    check(Category != Status)
    return Category == Special ? FAttackAndDefense{ .Attack = User->GetSpecialAttack(), .Defense = Target->GetSpecialDefense() }
        : FAttackAndDefense{ .Attack = User->GetAttack(), .Defense = Target->GetDefense() };
}

void UBaseBattleMove::CalculateDamageMultipliers(FDamageMultipliers &Multipliers,
                                                 const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target, int32 TargetCount, FName Type,
                                                 int32 BaseDamage, const FDamageEffects &Effects) const {
    // TODO: Global abilities
    // TODO: User's ability
    // TODO: Ally abilities
    // TODO: Target abilities
    // TODO: Ally target ally abilities

    // TODO: User and target items
    // TODO: Battle effect boosters

    // TODO: Mud Sport/Water Sport
    // TODO: Terrain Moves
    // TODO: Badge Multipliers
    ApplyMultiTargetModifier(Multipliers, TargetCount);
    // TODO: Weather
    
    ApplyCriticalHitModifier(Multipliers, Effects);
    ApplyDamageSwing(Multipliers);
    
    ApplyStabModifiers(Multipliers, User, Type);
    ApplyTypeMatchUps(Multipliers, Effects);
    // TODO: Status Effects
    
    // TODO: Aurora Veil, Reflect, Light Screen
    // TODO: Minimize
    
    ApplyAdditionalDamageModifiers(Multipliers, User, Target, TargetCount, Type, BaseDamage);
}

void UBaseBattleMove::ApplyCriticalHitModifier_Implementation(FDamageMultipliers &Multipliers,
    const FDamageEffects &Effects) const {
    if (Effects.bCriticalHit) {
        Multipliers.FinalDamageMultiplier *= 1.5f;
    }
}

void UBaseBattleMove::ApplyMultiTargetModifier_Implementation(FDamageMultipliers &Multipliers, int32 TargetCount) const {
    if (TargetCount > 1) {
        Multipliers.FinalDamageMultiplier *= 0.75f;
    }
}

void UBaseBattleMove::ApplyDamageSwing_Implementation(FDamageMultipliers &Multipliers) const {
    Multipliers.FinalDamageMultiplier *= FMath::RandRange(85, 100) / 100.0f;
}

void UBaseBattleMove::ApplyStabModifiers_Implementation(FDamageMultipliers &Multipliers,
    const TScriptInterface<IBattler> &User, FName MoveType) const {
    if (User->GetTypes().Contains(MoveType)) {
        // TODO: Account for adaptability/terrastilization
        Multipliers.FinalDamageMultiplier *= 1.5f;
    }
}

void UBaseBattleMove::ApplyTypeMatchUps_Implementation(FDamageMultipliers &Multipliers, const FDamageEffects& Effects) const {
    Multipliers.FinalDamageMultiplier *= Effects.TypeMatchUp;
}

void UBaseBattleMove::ApplyAdditionalDamageModifiers_Implementation(FDamageMultipliers& Multipliers, const TScriptInterface<IBattler>& User, const TScriptInterface<IBattler>& Target, int32 TargetCount, FName Type, int32 BaseDamage) const {
    // Currently there are no additional modifiers that need to be applied
}