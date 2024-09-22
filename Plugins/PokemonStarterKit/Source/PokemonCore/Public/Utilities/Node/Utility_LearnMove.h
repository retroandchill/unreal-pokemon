// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Blueprints/BlueprintUtilityNode.h"
#include "Pokemon/Moves/MoveBlock.h"

#include "Utility_LearnMove.generated.h"

struct FMoveHandle;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, Abstract, meta = (UtilityNode))
class POKEMONCORE_API UUtility_LearnMove : public UBlueprintUtilityNode {
    GENERATED_BODY()

public:
    void Execute(const TScriptInterface<IPokemon> &Pokemon, const FMoveHandle &Move, FOnMoveLearnEnd::FDelegate &&OnEnd);

protected:
    UFUNCTION(BlueprintImplementableEvent, Category = Items)
    void Execute(const TScriptInterface<IPokemon> &Pokemon, const FMoveHandle &Move);

    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Events)
    void ExecuteOnEnd(bool bMoveLearned);

private:
    FOnMoveLearnEnd EndProcess;

};
