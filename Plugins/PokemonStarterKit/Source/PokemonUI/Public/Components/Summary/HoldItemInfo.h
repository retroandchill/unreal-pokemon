// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Components/PokemonInfoWidget.h"

#include "HoldItemInfo.generated.h"

class UImage;
class UDisplayText;

/**
 * Display's information about the Pokémon's hold item.
 */
UCLASS(Abstract)
class POKEMONUI_API UHoldItemInfo : public UPokemonInfoWidget {
    GENERATED_BODY()

  protected:
    void Refresh_Implementation(const TScriptInterface<IPokemon> &Pokemon) override;

  private:
    /**
     * The widget used to display the item's name
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> ItemNameText;

    /**
     * The image used to display the item's icon
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> ItemIcon;

    /**
     * The image used to if a Pokémon is shiny or not
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> ShinyIcon;

    /**
     * The text displayed if the Pokémon is holding no item
     */
    UPROPERTY(EditAnywhere, Category = Display)
    FText NoItemText = NSLOCTEXT("UHoldItemInfo", "NoItemText", "None");
};
