// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Events/MoveEventPayload.h"
#include "UObject/Object.h"

#include "MoveTypeDeterminedPayload.generated.h"

class IBattler;
/**
 * Event payload for when the Type for a move is determined.
 */
UCLASS(BlueprintType)
class POKEMONBATTLE_API UMoveTypeDeterminedPayload : public UObject, public IMoveEventPayload {
    GENERATED_BODY()

  public:
    /**
     * The user of the move
     */
    UPROPERTY(BlueprintReadOnly, Category = "Moves|Context")
    TScriptInterface<IBattler> User;

    /**
     * The type of the move in question
     */
    UPROPERTY(BlueprintReadOnly, Category = "Moves|Typing")
    FName Type;

    const TScriptInterface<IBattler> &GetUser() const override;
};
