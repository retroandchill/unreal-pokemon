// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "StorageSystemDTO.h"
#include "UObject/Interface.h"

#include "StorageSystem.generated.h"

USTRUCT(BlueprintType)
struct FDepositResult {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    int32 BoxIndex;

    UPROPERTY(BlueprintReadWrite)
    int32 DepositedPokemonIndex;
};

class IStorageBox;
// This class does not need to be modified.
UINTERFACE(BlueprintType, NotBlueprintable, meta = (Injectable))
class UStorageSystem : public UInterface {
    GENERATED_BODY()
};

/**
 * The base implementation for the storage system.
 */
class POKEMONCORE_API IStorageSystem {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
  public:
    virtual TScriptInterface<IStorageSystem> Initialize(int32 BoxCount, int32 BoxCapacity, int32 StartingIndex = 0) = 0;
    virtual TScriptInterface<IStorageSystem> Initialize(const FStorageSystemDTO &DTO) = 0;

    virtual FStorageSystemDTO ToDTO() const = 0;

    UFUNCTION(BlueprintCallable, Category = StorageSystem)
    virtual int32 GetBoxCount() const = 0;

    UFUNCTION(BlueprintCallable, Category = StorageSystem)
    virtual const TScriptInterface<IStorageBox> &GetBox(int32 Index) const = 0;

    UFUNCTION(BlueprintCallable, Category = StorageSystem)
    virtual int32 GetCurrentBoxIndex() const = 0;

    UFUNCTION(BlueprintCallable, Category = StorageSystem)
    virtual void SetCurrentBoxIndex(int32 NewIndex) = 0;

    /**
     * Attempt to deposit a Pokémon into the box system
     * @param Pokemon The Pokémon to try to deposit
     * @return The index of the box and the slot the Pokémon was deposited to
     */
    virtual TOptional<FDepositResult> TryDeposit(const TScriptInterface<IPokemon>& Pokemon) = 0;
};