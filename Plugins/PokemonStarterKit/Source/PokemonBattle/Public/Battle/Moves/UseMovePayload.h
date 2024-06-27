// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UseMovePayload.generated.h"

class IBattleMove;

/**
 * 
 */
UCLASS()
class POKEMONBATTLE_API UUseMovePayload : public UObject {
    GENERATED_BODY()

public:
    UPROPERTY()
    TScriptInterface<IBattleMove> Move;

};
