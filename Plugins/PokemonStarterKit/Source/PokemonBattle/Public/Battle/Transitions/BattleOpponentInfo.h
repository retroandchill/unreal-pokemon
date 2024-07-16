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
     * @param Battle The battle that will take ownership of the side
     * @param SideClass The class for the side that is being spawned in
     * @param Transform The transform to spawn the side at
     * @param ActivePokemonCount The number of Pokémon to spawn
     * @return The created side
     */
    virtual TScriptInterface<IBattleSide> CreateOpposingSide(const TScriptInterface<IBattle> &Battle,
                                                             const TSubclassOf<AActor> &SideClass,
                                                             const FTransform &Transform,
                                                             int32 ActivePokemonCount = 1) = 0;
};

/**
 * Struct that wraps around a handle for opponent info
 */
USTRUCT(BlueprintType)
struct POKEMONBATTLE_API FBattleOpponentInfoHandle {
    GENERATED_BODY()

    /**
     * The wrapped data that underlies this object
     */
    TSharedPtr<IBattleOpponentInfo> Data;

    /**
     * Create the opposing side of battle with this information
     * @return The created side
     */
    FORCEINLINE TScriptInterface<IBattleSide> CreateOpposingSide(const TScriptInterface<IBattle> &Battle,
                                                                 TSubclassOf<AActor> SideClass,
                                                                 const FTransform &Transform,
                                                                 int32 ActivePokemonCount = 1) const {
        return Data->CreateOpposingSide(Battle, SideClass, Transform, ActivePokemonCount);
    }
};
