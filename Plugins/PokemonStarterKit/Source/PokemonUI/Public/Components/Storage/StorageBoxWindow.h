// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "StorageInfoPage.h"
#include "Components/SelectableWidget.h"
#include "Ranges/Functional/Delegates.h"

#include "StorageBoxWindow.generated.h"

class IPokemon;
class UStorageBoxIcon;
class IStorageBox;
/**
 * The window used to display the Pokémon stored inside a box.
 */
UCLASS(Abstract)
class POKEMONUI_API UStorageBoxWindow : public USelectableWidget {
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    const TScriptInterface<IStorageBox>& GetStorageBox() const;

    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
    void SetStorageBox(const TScriptInterface<IStorageBox>& InStorageBox);

    /**
     * Binding for the selected Pokémon being changed.
     */
    UE_MULTICAST_DELEGATE_MEMBER(FOnSelectedPokemonChanged, OnSelectedPokemonChanged)

    protected:
    void OnSelectionChange_Implementation(int32 OldIndex, int32 NewIndex) override;

private:
    void CreateStorageBoxIcon(const TScriptInterface<IPokemon>& Pokemon);
    
    UPROPERTY(BlueprintGetter = GetStorageBox, BlueprintSetter = SetStorageBox, Category = Storage)
    TScriptInterface<IStorageBox> StorageBox;

    UPROPERTY(EditAnywhere, Category = Display)
    TSubclassOf<UStorageBoxIcon> StorageBoxIconClass;
};
