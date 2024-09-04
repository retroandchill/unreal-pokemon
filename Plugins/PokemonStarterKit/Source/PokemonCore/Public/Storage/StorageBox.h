// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StorageBox.generated.h"

struct FStorageBoxDTO;
// This class does not need to be modified.
UINTERFACE(BlueprintType)
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
    virtual TScriptInterface<IStorageBox> Initialize(const FStorageBoxDTO& DTO) = 0;
    
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
    UFUNCTION(BlueprintCallable, Category = "Storage|Information",
        meta = (AutoCreateRefTerm = NewName))
    virtual void SetDisplayName(const FText& NewName) = 0;

};