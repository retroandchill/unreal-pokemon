// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Pokemon/Moves/Move.h"

#include "DefaultMove.generated.h"

struct FMoveTarget;
/**
 * The default implementation for a move
 */
UCLASS()
class POKEMONCORE_API UDefaultMove : public UObject, public IMove {
    GENERATED_BODY()

  public:
    TScriptInterface<IMove> Initialize(const FMoveDTO &MoveID);

    FMoveDTO ToDTO() const override;

    const FMoveData &GetMoveData() const override;

    UFUNCTION(BlueprintPure, Category = "Pokémon|Moves")
    FText GetDisplayName() const override;

    UFUNCTION(BlueprintPure, Category = "Pokémon|Moves")
    FText GetMoveDescription() const override;

    UFUNCTION(BlueprintPure, Category = "Pokémon|Moves")
    FName GetType() const override;

    UFUNCTION(BlueprintPure, Category = "Pokémon|Moves")
    EMoveDamageCategory GetDamageCategory() const override;

    UFUNCTION(BlueprintPure, Category = "Pokémon|Moves")
    int32 GetBasePower() const override;

    UFUNCTION(BlueprintPure, Category = "Pokémon|Moves")
    bool IsVariablePower() const override;

    UFUNCTION(BlueprintPure, Category = "Pokémon|Moves")
    int32 GetAccuracy() const override;

    UFUNCTION(BlueprintCallable, Category = "Pokémon|Moves")
    bool AlwaysHits() const override;

    UFUNCTION(BlueprintPure, Category = "Pokémon|Moves")
    int32 GetCurrentPP() const override;

    UFUNCTION(BlueprintPure, Category = "Pokémon|Moves")
    int32 GetTotalPP() const override;

    UFUNCTION(BlueprintPure, Category = "Pokémon|Moves")
    FName GetFunctionCode() const override;

    UFUNCTION(BlueprintPure, Category = "Pokémon|Moves")
    const FMoveTarget &GetTargetType() const override;

    UFUNCTION(BlueprintPure, Category = "Pokémon|Moves")
    int32 GetAdditionalEffectChance() const override;

    UFUNCTION(BlueprintPure, Category = "Pokémon|Moves")
    const TArray<FName> &GetTags() const override;

    UFUNCTION(BlueprintCallable, Category = "Pokémon|Moves")
    void RecoverPP(int32 Amount = 1) override;

    UFUNCTION(BlueprintCallable, Category = "Pokémon|Moves")
    void DecrementPP(int32 Amount) override;

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
