// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "CommonUserWidget.h"
#include "StorageInfoPage.h"

#include "StorageBoxIcon.generated.h"

class IPokemon;
/**
 * The icon widget for a stroage box.
 */
UCLASS(Abstract)
class POKEMONUI_API UStorageBoxIcon : public UCommonButtonBase
{
    GENERATED_BODY()

  public:
    /**
     * Set the Pokémon for this icon.
     * @param Pokemon The Pokémon in question
     */
    UFUNCTION(BlueprintImplementableEvent, Category = UI)
    void SetPokemon(const TScriptInterface<IPokemon> &Pokemon);

    /**
     * Set this icon to be an empty slot.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = UI)
    void ResetPokemon();
};
