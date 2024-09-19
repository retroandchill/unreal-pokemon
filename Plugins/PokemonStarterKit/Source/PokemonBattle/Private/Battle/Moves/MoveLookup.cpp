// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Moves/MoveLookup.h"
#include "DynamicAssetLoadingSettings.h"
#include "PokemonBattleSettings.h"
#include "Battle/BlueprintClasses.h"
#include "Battle/Moves/BattleMoveFunctionCode.h"
#include "Ranges/Optional/OrElseGet.h"

static TSubclassOf<UBattleMoveFunctionCode> GetDefaultMoveClass() {
    auto MoveClass = GetDefault<UPokemonBattleSettings>()->DefaultMoveAbility.TryLoadClass<UBattleMoveFunctionCode>();
    check(MoveClass != nullptr)
    return MoveClass;
}

TSubclassOf<UBattleMoveFunctionCode> Pokemon::Battle::Moves::LookupMoveEffectClass(FName FunctionCode) {
    // clang-format off
    return Classes::MoveEffects.LoadClass(FunctionCode) |
           UE::Optionals::OrElseGet(&GetDefaultMoveClass);
    // clang-format on
}