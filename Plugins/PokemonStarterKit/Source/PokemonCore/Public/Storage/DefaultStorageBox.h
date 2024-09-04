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
class POKEMONCORE_API UDefaultStorageBox : public UObject, public IStorageBox {
    GENERATED_BODY()

public:
    TScriptInterface<IStorageBox> Initialize(FText &&InitialName, int32 Capacity) override;
    TScriptInterface<IStorageBox> Initialize(const FStorageBoxDTO &DTO) override;

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Storage|Information")
    const FText & GetDisplayName() const override;
    
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "Storage|Information",
        meta = (AutoCreateRefTerm = NewName))
    void SetDisplayName(const FText &NewName) override;

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
