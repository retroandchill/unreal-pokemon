// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BattleInfo.h"

struct FPokemonDTO;

/**
 * Information about a wild battle in the game
 */
class POKEMONBATTLE_API FWildBattleOpponentInfo : public IBattleOpponentInfo {
public:
    /**
     * Construct a new wild battle against a single Pokémon
     * @param PokemonInfo The information about the Pokémon that is being fought
     */
    explicit FWildBattleOpponentInfo(const TSharedRef<FPokemonDTO>& PokemonInfo);

    TScriptInterface<IBattleSide> CreateOpposingSide(const TScriptInterface<IBattle>& Battle, const TSubclassOf<AActor>& SideClass, const FTransform &Transform, int32
                                                     ActivePokemonCount = 1) override;


private:
    /**
     * The info about the owning Pokémon
     */
    TArray<TSharedRef<FPokemonDTO>> OpposingPokemonInfo;
};
