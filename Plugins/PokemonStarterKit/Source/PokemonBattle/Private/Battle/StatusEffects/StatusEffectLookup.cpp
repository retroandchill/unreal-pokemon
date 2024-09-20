// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/StatusEffects/StatusEffectLookup.h"
#include "Battle/Status.h"
#include "GameplayEffect.h"
#include "Ranges/Optional/FlatMap.h"
#include "Ranges/Optional/Map.h"
#include "Battle/BlueprintClasses.h"

TOptional<TNonNullSubclassOf<UGameplayEffect>> Pokemon::Battle::StatusEffects::FindStatusEffect(FName ID) {
    return Classes::StatusEffects.LoadClass(ID);
}

TOptional<TNonNullSubclassOf<UGameplayEffect>> Pokemon::Battle::StatusEffects::FindStatusEffect(TOptional<const FStatus &> Status) {
    // clang-format off
    return Status |
           UE::Optionals::Map(&FStatus::ID) |
           UE::Optionals::FlatMap([](FName ID) { return FindStatusEffect(ID); });
    // clang-format on
}