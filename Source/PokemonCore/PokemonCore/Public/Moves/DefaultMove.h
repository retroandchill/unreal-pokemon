// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "DefaultMove.generated.h"
#include "Moves/Move.h"

/**
 * The default implementation for a move
 */
UCLASS()
class POKEMONCORE_API UDefaultMove : public UObject, public IMove {
    GENERATED_BODY()

  public:
    TScriptInterface<IMove> Initialize(FName MoveID);

    const FMoveData &GetMoveData() const override;
    int32 GetCurrentPP() const override;
    int32 GetTotalPP() const override;

  private:
    /**
     * A reference to the move data struct
     */
    UPROPERTY(SaveGame)
    FName ID;

    /**
     * The current PP for this move
     */
    UPROPERTY(SaveGame)
    int32 CurrentPP;
};
