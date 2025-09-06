// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "StorageSystem.h"
#include "UObject/Object.h"

#include "DefaultStorageSystem.generated.h"

/**
 *
 */
UCLASS()
class POKEMONCORE_API UDefaultStorageSystem : public UObject, public IStorageSystem
{
    GENERATED_BODY()

  public:
    TScriptInterface<IStorageSystem> Initialize(int32 BoxCount, int32 BoxCapacity, int32 StartingIndex) override;
    TScriptInterface<IStorageSystem> Initialize(const FStorageSystemDTO &DTO) override;
    FStorageSystemDTO ToDTO() const override;

    UFUNCTION(BlueprintPure, Category = StorageSystem)
    int32 GetBoxCount() const override;

    UFUNCTION(BlueprintCallable, Category = StorageSystem)
    const TScriptInterface<IStorageBox> &GetBox(int32 Index) const override;

    UFUNCTION(BlueprintCallable, Category = StorageSystem)
    const TScriptInterface<IStorageBox> &GetCurrentBox() const override;

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = StorageSystem)
    int32 GetCurrentBoxIndex() const override;

    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = StorageSystem)
    void SetCurrentBoxIndex(int32 NewIndex) override;

    TOptional<FDepositResult> TryDeposit(const TScriptInterface<IPokemon> &Pokemon) override;

  private:
    TScriptInterface<IStorageBox> CreateStorageBox(FText &&BoxName, int32 BoxCapacity);

    UPROPERTY()
    TArray<TScriptInterface<IStorageBox>> Boxes;

    UPROPERTY(BlueprintGetter = GetCurrentBoxIndex, BlueprintSetter = SetCurrentBoxIndex, Category = StorageSystem)
    int32 CurrentBoxIndex;
};
