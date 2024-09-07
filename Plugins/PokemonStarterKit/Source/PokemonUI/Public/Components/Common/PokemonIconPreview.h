// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "PokemonInfoWidget.h"

#include "PokemonIconPreview.generated.h"

class UPaperFlipbookUserWidget;
class UImage;

/**
 * Info widget for a given Pokémon that displays their icon and type.
 */
UCLASS(Abstract)
class POKEMONUI_API UPokemonIconPreview : public UPokemonInfoWidget {
    GENERATED_BODY()

  protected:
    void Refresh_Implementation(const TScriptInterface<IPokemon> &Pokemon) override;

    /**
     * Slot a Pokémon type icon into the widget
     * @param TypeIcon The type icon to slot
     */
    UFUNCTION(BlueprintImplementableEvent, Category = Display)
    void SlotTypeIcon(UImage *TypeIcon);

  private:
    void CreateTypeIcon(UObject *Resource);

    /**
     * The icon for the Pokémon
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UPaperFlipbookUserWidget> PokemonIcon;

    UPROPERTY()
    TArray<TObjectPtr<UImage>> TypeIcons;
};
