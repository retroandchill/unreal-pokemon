// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Moves/BattleMoveFunctionCode.h"
#include "Battle/Moves/PokemonBattleMove.h"

#include "CriticalTestMove.generated.h"

/**
 *
 */
UCLASS()
class POKEMONTESTS_API UCriticalTestMove : public UPokemonBattleMove {
    GENERATED_BODY()

  public:
    ECriticalOverride CriticalOverride = ECriticalOverride::Normal;
};
