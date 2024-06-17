// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Traits/Damage/DamageModificationTrait.h"
#include "Algo/AllOf.h"
#include "Battle/Traits/Damage/Conditions/DamageModificationCondition.h"

bool EvaluateCondition(const FMoveDamageInfo &Context, const UDamageModificationCondition *Condition) {
    return Condition->Evaluate(Context);
}

bool UDamageModificationTrait::MeetsConditions(const FMoveDamageInfo &Context) const {
    auto Predicate = [&Context](const UDamageModificationCondition* Condition) {
        return Condition->Evaluate(Context);
    };
    return Algo::AllOf(Conditions, Predicate);
}

void UDamageModificationTrait::Apply_Implementation(FDamageMultipliers &Multipliers,
                                                    const FMoveDamageInfo &Context) const {
    // No implementation in the base class
}