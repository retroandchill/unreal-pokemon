// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BattleInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Pokemon/PokemonDTO.h"

#include "BattleTransitionHelpers.generated.h"

/**
 *
 */
UCLASS()
class POKEMONBATTLE_API UBattleTransitionHelpers : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
    /**
     * Make a wild battle spec for a single Pokémon
     * @param Species The Pokémon to battle against
     * @param Level The level of the wild Pokémon
     * @param bLossAllowed Is the player allowed to lose this battle
     * @return The created battle information
     */
    UFUNCTION(BlueprintPure, Category = Battle)
    static FBattleInfo MakeSingleWildBattle(FName Species, int32 Level = 5, bool bLossAllowed = false);
};
