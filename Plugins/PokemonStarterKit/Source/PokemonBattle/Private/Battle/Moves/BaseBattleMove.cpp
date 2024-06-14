// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Moves/BaseBattleMove.h"
#include "Battle/Abilities/AbilityBattleEffect.h"
#include "Battle/Battle.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Effects/BattlerEffect.h"
#include "Battle/Effects/FieldEffect.h"
#include "Battle/Items/HoldItemBattleEffect.h"
#include "Battle/Type.h"
#include "DataManager.h"
#include "Battle/BattleSide.h"
#include "Moves/MoveData.h"
#include "Pokemon/Moves/Move.h"

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

TArray<TScriptInterface<IBattler>> UBaseBattleMove::GetAllPossibleTargets_Implementation(
    const TScriptInterface<IBattler> &User) const {
    TArray<TScriptInterface<IBattler>> Targets;
    auto UserSide = User->GetOwningSide();
    auto UserId = User->GetInternalId();
    auto &Battle = UserSide->GetOwningBattle();
    Battle->ForEachActiveBattler([&Targets, &UserId](const TScriptInterface<IBattler>& Battler) {
       if (Battler->GetInternalId() == UserId) {
           return;
       }
        Targets.Emplace(Battler);
    });
    return Targets;
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

bool UBaseBattleMove::HasTag(FName Tag) const {
    return WrappedMove->GetMoveData().Tags.Contains(Tag);
}

FBattleDamage UBaseBattleMove::CalculateDamage_Implementation(const TScriptInterface<IBattler> &User,
                                                              const TScriptInterface<IBattler> &Target,
                                                              int32 TargetCount) {
    if (WrappedMove->GetDamageCategory() == EMoveDamageCategory::Status) {
        return {.Damage = 0, .Effectiveness = EDamageEffectiveness::NonDamaging};
    }

    auto MoveType = DetermineType();
    FDamageEffects Effects;
    CalculateTypeMatchups(Effects, Target, MoveType);
    if (Effects.Effectiveness == EDamageEffectiveness::NoEffect) {
        return {.Damage = 0, .Effectiveness = EDamageEffectiveness::NoEffect};
    }

    // TODO: Roll crit
    int32 BasePower = CalculateBasePower(WrappedMove->GetBasePower(), User, Target);
    auto [Attack, Defense] = GetAttackAndDefense(User, Target);

    FDamageMultipliers Multipliers;
    CalculateDamageMultipliers(Multipliers, User, Target, TargetCount, MoveType, BasePower, Effects);
    BasePower = ModifiedParameter(BasePower, Multipliers.PowerMultiplier);
    Attack = ModifiedParameter(Attack, Multipliers.AttackMultiplier);
    Defense = ModifiedParameter(Defense, Multipliers.DefenseMultiplier);
    int32 Damage = CalculateBaseDamage(BasePower, User->GetPokemonLevel(), Attack, Defense);
    Damage = ModifiedParameter(Damage, Multipliers.FinalDamageMultiplier);

    return {.Damage = Damage, .Effectiveness = Effects.Effectiveness, .bCriticalHit = Effects.bCriticalHit};
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

void UBaseBattleMove::CalculateDamageMultipliers(FDamageMultipliers &Multipliers,
                                                 const TScriptInterface<IBattler> &User,
                                                 const TScriptInterface<IBattler> &Target, int32 TargetCount,
                                                 FName Type, int32 BaseDamage, const FDamageEffects &Effects) {
    //ApplyAbilityMultipliers(Multipliers, User, Target, Type, BaseDamage);
    //ApplyHoldItemMultipliers(Multipliers, User, Target, Type, BaseDamage);
    //ApplyUserEffects(Multipliers, User, Target, Type, BaseDamage);
    //ApplyFieldEffects(Multipliers, User, Target, Type, BaseDamage);
    // TODO: Terrain Moves
    // TODO: Badge Multipliers
    ApplyMultiTargetModifier(Multipliers, TargetCount);
    // TODO: Weather

    ApplyCriticalHitModifier(Multipliers, Effects);
    ApplyDamageSwing(Multipliers);

    ApplyStabModifiers(Multipliers, User, Type);
    ApplyTypeMatchUps(Multipliers, Effects);
    // TODO: Status Effects (Burn)

    // TODO: Aurora Veil, Reflect, Light Screen
    //ApplyTargetEffects(Multipliers, User, Target, Type, BaseDamage);

    ApplyAdditionalDamageModifiers(Multipliers, User, Target, TargetCount, Type, BaseDamage);
}

void UBaseBattleMove::ApplyAbilityMultipliers(FDamageMultipliers &Multipliers, const TScriptInterface<IBattler> &User,
                                              const TScriptInterface<IBattler> &Target, FName Type, int32 BaseDamage) {
    CurrentBattle->ForEachActiveBattler([&](const TScriptInterface<IBattler> &Battler) {
        IAbilityBattleEffect::Execute_TriggerDamageCalcFromGlobal(Battler->GetAbility().GetObject(), Multipliers, User,
                                                                  Target, this, BaseDamage, Type);
    });

    if (User->IsAbilityActive()) {
        IAbilityBattleEffect::Execute_TriggerDamageCalcFromUser(User->GetAbility().GetObject(), Multipliers, User,
                                                                Target, this, BaseDamage, Type);
    }

    if (!CurrentBattle->ShouldIgnoreAbilities()) {
        User->ForEachAlly([&](const TScriptInterface<IBattler> &Ally) {
            if (!Ally->IsAbilityActive()) {
                return;
            }

            IAbilityBattleEffect::Execute_TriggerDamageCalcFromAlly(Ally->GetAbility().GetObject(), Multipliers, User,
                                                                    Target, this, BaseDamage, Type);
        });

        if (Target->IsAbilityActive()) {
            IAbilityBattleEffect::Execute_TriggerDamageCalcFromTarget(Target->GetAbility().GetObject(), Multipliers,
                                                                      User, Target, this, BaseDamage, Type);
        }
    }

    if (Target->IsAbilityActive()) {
        IAbilityBattleEffect::Execute_TriggerDamageCalcFromTargetNonIgnorable(
            Target->GetAbility().GetObject(), Multipliers, User, Target, this, BaseDamage, Type);
    }

    if (!CurrentBattle->ShouldIgnoreAbilities()) {
        Target->ForEachAlly([&](const TScriptInterface<IBattler> &Ally) {
            if (!Ally->IsAbilityActive()) {
                return;
            }

            IAbilityBattleEffect::Execute_TriggerDamageCalcFromTargetAlly(Ally->GetAbility().GetObject(), Multipliers,
                                                                          User, Target, this, BaseDamage, Type);
        });
    }
}

void UBaseBattleMove::ApplyHoldItemMultipliers(FDamageMultipliers &Multipliers, const TScriptInterface<IBattler> &User,
                                               const TScriptInterface<IBattler> &Target, FName Type, int32 BaseDamage) {
    if (User->IsHoldItemActive()) {
        IHoldItemBattleEffect::Execute_TriggerDamageCalcFromUser(User->GetHoldItem().GetObject(), Multipliers, User,
                                                                 Target, this, BaseDamage, Type);
    }

    if (Target->IsHoldItemActive()) {
        IHoldItemBattleEffect::Execute_TriggerDamageCalcFromTarget(Target->GetHoldItem().GetObject(), Multipliers, User,
                                                                   Target, this, BaseDamage, Type);
    }
}

void UBaseBattleMove::ApplyUserEffects(FDamageMultipliers &Multipliers, const TScriptInterface<IBattler> &User,
                                       const TScriptInterface<IBattler> &Target, FName Type, int32 BaseDamage) {
    User->ForEachBattleEffect([&](const TScriptInterface<IBattlerEffect> &Effect) {
        IBattlerEffect::Execute_ModifyDamageForUser(Effect.GetObject(), Multipliers, User, Target, this, BaseDamage,
                                                    Type);
    });
}

void UBaseBattleMove::ApplyTargetEffects(FDamageMultipliers &Multipliers, const TScriptInterface<IBattler> &User,
                                         const TScriptInterface<IBattler> &Target, FName Type, int32 BaseDamage) {
    Target->ForEachBattleEffect([&](const TScriptInterface<IBattlerEffect> &Effect) {
        IBattlerEffect::Execute_ModifyDamageForUser(Effect.GetObject(), Multipliers, User, Target, this, BaseDamage,
                                                    Type);
    });
}

void UBaseBattleMove::ApplyFieldEffects(FDamageMultipliers &Multipliers, const TScriptInterface<IBattler> &User,
                                        const TScriptInterface<IBattler> &Target, FName Type, int32 BaseDamage) {
    CurrentBattle->ForEachFieldEffect([&](const TScriptInterface<IFieldEffect> &Effect) {
        IFieldEffect::Execute_ModifyDamage(Effect.GetObject(), Multipliers, User, Target, this, BaseDamage, Type);
    });
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
                                                                    const TScriptInterface<IBattler> &User,
                                                                    const TScriptInterface<IBattler> &Target,
                                                                    int32 TargetCount, FName Type, int32 BaseDamage) {
    // Currently there are no additional modifiers that need to be applied
}