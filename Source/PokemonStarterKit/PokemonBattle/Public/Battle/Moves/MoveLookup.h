// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

class UBattleMoveFunctionCode;

namespace Pokemon::Battle::Moves
{

    /**
     * Lookup a move effect by its function code
     * @param FunctionCode The function code of the move in question
     * @return The class that corresponds with the move
     */
    POKEMONBATTLE_API TSubclassOf<UBattleMoveFunctionCode> LookupMoveEffectClass(FName FunctionCode);

} // namespace Pokemon::Battle::Moves