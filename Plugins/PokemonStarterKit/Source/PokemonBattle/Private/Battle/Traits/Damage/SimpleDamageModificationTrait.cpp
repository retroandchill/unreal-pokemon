// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Traits/Damage/SimpleDamageModificationTrait.h"

void USimpleDamageModificationTrait::Apply_Implementation(FDamageMultipliers &Multipliers,
                                                          const FMoveDamageInfo &Context) const {
    Multipliers *= DamageMultipliers;
}