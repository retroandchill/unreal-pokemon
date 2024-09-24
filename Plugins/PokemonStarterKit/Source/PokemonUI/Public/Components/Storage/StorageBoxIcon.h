// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "CommonUserWidget.h"
#include "StorageBoxIcon.generated.h"

class IPokemon;
/**
 * The icon widget for a stroage box.
 */
UCLASS(Abstract)
class POKEMONUI_API UStorageBoxIcon : public UCommonButtonBase {
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintImplementableEvent, Category = UI)
    void SetPokemon(const TScriptInterface<IPokemon>& Pokemon);

    UFUNCTION(BlueprintImplementableEvent, Category = UI)
    void ResetPokemon();

};
