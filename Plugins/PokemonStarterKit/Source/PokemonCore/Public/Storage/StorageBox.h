// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Lookup/InjectableDependency.h"
#include "RetroLib.h"
#include "UObject/Interface.h"

#include "StorageBox.generated.h"

class IPokemon;
struct FStorageBoxDTO;
// This class does not need to be modified.
UINTERFACE(BlueprintType, NotBlueprintable)
class UStorageBox : public UInterface {
    GENERATED_BODY()
};

/**
 * Defines a "Box" in the Pokémon storage system
 */
class POKEMONCORE_API IStorageBox {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
  public:
    /**
     * Initialize a storage box with the given name and capacity
     * @param InitialName The name of the box upon creation
     * @param Capacity The capacity of the box
     * @return A pointer to the created box
     */
    virtual TScriptInterface<IStorageBox> Initialize(FText &&InitialName, int32 Capacity) = 0;
    virtual TScriptInterface<IStorageBox> Initialize(const FStorageBoxDTO &DTO) = 0;

    virtual FStorageBoxDTO ToDTO() const = 0;

    /**
     * Get the display name of the box.
     * @return The box's display name
     */
    UFUNCTION(BlueprintCallable, Category = "Storage|Information")
    virtual const FText &GetDisplayName() const = 0;

    /**
     * Set the display name for the box.
     * @param NewName The new name for the box
     */
    UFUNCTION(BlueprintCallable, Category = "Storage|Information", meta = (AutoCreateRefTerm = NewName))
    virtual void SetDisplayName(const FText &NewName) = 0;

    /**
     * Get the capacity of the box.
     * @return The capacity of the box in question
     */
    UFUNCTION(BlueprintCallable, Category = "Storage|Information")
    virtual int32 GetCapacity() const = 0;

    /**
     * Deposit of a Pokémon into the box.
     * @param Pokemon The Pokémon to deposit
     * @return The index of the Pokémon if it was deposited
     */
    virtual TOptional<int32> DepositToBox(const TScriptInterface<IPokemon> &Pokemon) = 0;

    /**
     * Check if the given box is full or not.
     * @return Is the given box full
     */
    UFUNCTION(BlueprintCallable, Category = "Storage|Pokémon")
    virtual bool IsBoxFull() const = 0;

    /**
     * Withdraw a Pokémon from a box at the given index
     * @param BoxIndex The index to withdraw from
     * @return The withdrawn Pokémon
     */
    virtual TOptional<IPokemon &> WithdrawFromBox(int32 BoxIndex) = 0;

    /**
     * Swap the out the Pokémon stored at the given index with a new Pokémon
     * @param BoxIndex The index to store into
     * @param Pokemon The Pokémon to replace into the slot
     * @return The Pokémon that used to be in slot
     */
    virtual TOptional<IPokemon &> SwapWithPokemon(int32 BoxIndex, const TScriptInterface<IPokemon> &Pokemon) = 0;

    /**
     * Get the list of the stored Pokémon in the box.
     * @return The stored Pokémon
     */
    virtual TOptional<IPokemon &> GetStoredPokemon(int32 Index) const = 0;

    /**
     * Get the list of the stored Pokémon in the box.
     * @return The stored Pokémon
     */
    virtual const TArray<TScriptInterface<IPokemon>> &GetStoredPokemon() const = 0;
};

DECLARE_INJECTABLE_DEPENDENCY(POKEMONCORE_API, IStorageBox);