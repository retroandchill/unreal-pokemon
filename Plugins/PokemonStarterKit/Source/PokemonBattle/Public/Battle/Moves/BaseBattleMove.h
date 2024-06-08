// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BattleMove.h"
#include "UObject/Object.h"
#include "BaseBattleMove.generated.h"

/**
 * The base class used for all battle moves used by the game.
 */
UCLASS()
class POKEMONBATTLE_API UBaseBattleMove : public UObject, public IBattleMove {
    GENERATED_BODY()

public:
    TScriptInterface<IBattleMove> Initialize(const TScriptInterface<IMove>& Move) override;

protected:
    int32 CalculateBaseDamage_Implementation(const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target) override;
    int32 CalculateTruePower_Implementation(const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target) override;
    FAttackAndDefense GetAttackAndDefense_Implementation(const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target) override;

private:
    /**
     * The move that is actively being wrapped around
     */
    UPROPERTY()
    TScriptInterface<IMove> WrappedMove;
};
