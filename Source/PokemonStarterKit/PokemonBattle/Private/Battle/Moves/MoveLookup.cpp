// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Moves/MoveLookup.h"
#include "Battle/BlueprintClasses.h"
#include "Battle/Moves/BattleMoveFunctionCode.h"
#include "RetroLib/Optionals/OrElseValue.h"

TSubclassOf<UBattleMoveFunctionCode> Pokemon::Battle::Moves::LookupMoveEffectClass(FName FunctionCode)
{
    // clang-format off
    return nullptr;
    /*
    return Classes::MoveEffects.LoadClass(FunctionCode) |
           Retro::Optionals::OrElseValue(UBattleMoveFunctionCode::StaticClass());
           */
    // clang-format on
}