// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BattleOpponentInfo.generated.h"

class IBattle;
class IBattleSide;

/**
 * Abstract class for the basic information about a wild battle
 */
class POKEMONBATTLE_API IBattleOpponentInfo {

public:
    virtual ~IBattleOpponentInfo() = default;

    /**
     * Create the opposing side of battle with this information
     * @param Battle
     * @param SideClass The class for the side that is being spawned in
     * @param Transform The transform to spawn the side at
     * @param ActivePokemonCount
     * @return The created side
     */
    virtual TScriptInterface<IBattleSide> CreateOpposingSide(const TScriptInterface<IBattle>& Battle, TSubclassOf<AActor> SideClass, const FTransform& Transform, int32  ActivePokemonCount = 1);
};

/**
 * Struct that wraps around a handle for opponent info
 */
USTRUCT(BlueprintType)
struct POKEMONBATTLE_API FBattleOpponentInfoHandle {

    /**
     * The wrapped data that underlies this object
     */
    TSharedPtr<IBattleOpponentInfo> Data;

    /**
     * Create the opposing side of battle with this information
     * @tparam A The argument types that are passed to the proxied object
     * @param Args The arguments that are passed to the proxied object
     * @return The created side
     */
    template <typename... A>
    FORCEINLINE TScriptInterface<IBattleSide> CreateOpposingSide(A&&... Args) const {
        return Data->CreateOpposingSide(Forward<A...>(Args)...);
    }
    
};
