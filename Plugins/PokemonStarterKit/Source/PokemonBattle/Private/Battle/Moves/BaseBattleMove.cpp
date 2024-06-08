// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Moves/BaseBattleMove.h"
#include "Battle/Battlers/Battler.h"
#include "Pokemon/Moves/Move.h"
#include "Battle/Moves/Modifiers/StabModifier.h"
#include "Battle/Moves/Modifiers/TypeMatchUpModifier.h"

TScriptInterface<IBattleMove> UBaseBattleMove::Initialize(const TScriptInterface<IMove> &Move) {
    using namespace Battle::Moves;
    WrappedMove = Move;
    DamageModifiers.Empty();
    DamageModifiers.Add(MakeUnique<FTypeMatchUpModifier>());
    DamageModifiers.Add(MakeUnique<FStabModifier>());
    DamageModifiers.Add(MakeUnique<FTargetCountModifier>());
    return this;
}

FBattleDamage UBaseBattleMove::CalculateDamage_Implementation(const TScriptInterface<IBattler> &User,
    const TScriptInterface<IBattler> &Target, int32 TargetCount) const {
    if (WrappedMove->GetDamageCategory() == EMoveDamageCategory::Status) {
            return { .Damage = 0, .Effectiveness = EDamageEffectiveness::NonDamaging };
    }

    int32 BaseDamage = CalculateBaseDamage(User, Target);

    FModifiedDamage ModifiedDamage = {.Modifier = 1.f, .Effectiveness = EDamageEffectiveness::Normal};
    for (auto &Modifier : DamageModifiers) {
        Modifier->ApplyModifier(ModifiedDamage, this, User, Target, TargetCount);
        if (ModifiedDamage.Effectiveness == EDamageEffectiveness::NoEffect) {
            return {.Damage = 0, .Effectiveness = ModifiedDamage.Effectiveness };
        }
    }

    ApplyAdditionalDamageModifiers(ModifiedDamage, User, Target, TargetCount);
    ApplyDamageSwing(ModifiedDamage);
    return {
        .Damage = FMath::Max(FMath::RoundToInt32(BaseDamage * ModifiedDamage.Modifier), 1),
        .Effectiveness = ModifiedDamage.Effectiveness
    };
}

FName UBaseBattleMove::DetermineType_Implementation() const {
    return WrappedMove->GetType();
}

int32 UBaseBattleMove::CalculateBaseDamage_Implementation(const TScriptInterface<IBattler> &User,
                                                          const TScriptInterface<IBattler> &Target) const {
    auto [Attack, Defense] = GetAttackAndDefense(User, Target);
    int32 Power = CalculateTruePower(User, Target);
    return FMath::FloorToInt32(2.0f * User->GetLevel() / 5 + 2) * Power * Attack / Defense / 50 + 2;
}

int32 UBaseBattleMove::CalculateTruePower_Implementation(const TScriptInterface<IBattler> &User,
    const TScriptInterface<IBattler> &Target) const {
    return WrappedMove->GetBasePower();
}

FAttackAndDefense UBaseBattleMove::GetAttackAndDefense_Implementation(const TScriptInterface<IBattler> &User,
    const TScriptInterface<IBattler> &Target) const {
    using enum EMoveDamageCategory;
    auto Category = WrappedMove->GetDamageCategory();
    check(Category != Status)
    return Category == Special ? FAttackAndDefense{ .Attack = User->GetSpecialAttack(), .Defense = Target->GetSpecialDefense() }
        : FAttackAndDefense{ .Attack = User->GetAttack(), .Defense = Target->GetDefense() };
}

void UBaseBattleMove::ApplyAdditionalDamageModifiers_Implementation(FModifiedDamage &Damage,
    const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target, int32 TargetCount) const {
    // Currently there are no additional modifiers that need to be applied
}

void UBaseBattleMove::ApplyDamageSwing_Implementation(FModifiedDamage &Damage) const {
    Damage.Modifier *= FMath::RandRange(85, 100) / 100.f;
}