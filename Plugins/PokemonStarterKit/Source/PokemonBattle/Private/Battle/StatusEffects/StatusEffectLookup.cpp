// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/StatusEffects/StatusEffectLookup.h"
#include "Battle/BlueprintClasses.h"
#include "Battle/Status.h"
#include "GameplayEffect.h"

TOptional<TNonNullSubclassOf<UGameplayEffect>> Pokemon::Battle::StatusEffects::FindStatusEffect(FName ID) {
    return Classes::StatusEffects.LoadClass(ID);
}

TOptional<TNonNullSubclassOf<UGameplayEffect>>
Pokemon::Battle::StatusEffects::FindStatusEffect(TOptional<const FStatus &> Status) {
    // clang-format off
    return Status |
           Retro::Optionals::Transform(&FStatus::ID) |
           Retro::Optionals::AndThen([](FName ID) { return FindStatusEffect(ID); });
    // clang-format on
}