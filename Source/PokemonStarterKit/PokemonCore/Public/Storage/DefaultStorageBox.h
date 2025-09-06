// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "StorageBox.h"
#include "UObject/Object.h"

#include "DefaultStorageBox.generated.h"

class IPokemon;

/**
 *
 */
UCLASS()
class POKEMONCORE_API UDefaultStorageBox : public UObject, public IStorageBox
{
    GENERATED_BODY()

  public:
    TScriptInterface<IStorageBox> Initialize(FText &&InitialName, int32 Capacity) override;
    TScriptInterface<IStorageBox> Initialize(const FStorageBoxDTO &DTO) override;

    FStorageBoxDTO ToDTO() const override;

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Storage|Information")
    const FText &GetDisplayName() const override;

    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "Storage|Information",
              meta = (AutoCreateRefTerm = NewName))
    void SetDisplayName(FText NewName) override;

    UFUNCTION(BlueprintCallable, Category = "Storage|Information")
    int32 GetCapacity() const override;

    TOptional<int32> DepositToBox(const TScriptInterface<IPokemon> &Pokemon) override;

    UFUNCTION(BlueprintPure, Category = "Storage|Pokémon")
    bool IsBoxFull() const override;

    TOptional<IPokemon &> WithdrawFromBox(int32 BoxIndex) override;
    TOptional<IPokemon &> SwapWithPokemon(int32 BoxIndex, const TScriptInterface<IPokemon> &Pokemon) override;

    TOptional<IPokemon &> GetStoredPokemon(int32 Index) const override;
    const TArray<TScriptInterface<IPokemon>> &GetStoredPokemon() const override;

  private:
    /**
     * The name of the box as displayed to the player.
     */
    UPROPERTY(BlueprintGetter = GetDisplayName, BlueprintSetter = SetDisplayName, Category = "Storage|Information")
    FText DisplayName;

    /**
     * The list of Pokémon held by this box.
     * <p><b>NOTE:</b> An empty slot is represented using the <code>nullptr</code> literal so always check for validity
     * before using something in this array.</p>
     */
    UPROPERTY()
    TArray<TScriptInterface<IPokemon>> StoredPokemon;
};
