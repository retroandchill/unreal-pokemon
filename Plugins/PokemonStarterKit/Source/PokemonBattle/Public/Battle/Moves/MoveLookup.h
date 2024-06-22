// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

namespace Battle::Moves {

/**
 * Lookup a move effect by its function code
 * @param FunctionCode The function code of the move in question
 * @return The class that corresponds with the move
 */
POKEMONBATTLE_API UClass* LookupMoveEffectClass(FName FunctionCode);

}