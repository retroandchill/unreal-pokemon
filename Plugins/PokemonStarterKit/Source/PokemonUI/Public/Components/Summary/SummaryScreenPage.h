// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CommonUserWidget.h"
#include "Components/Common/PokemonInfoWidget.h"

#include "SummaryScreenPage.generated.h"

class UCommonButtonStyle;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPageSelected);

class IPokemon;

/**
 * Abstract declaration of a page on the summary screen.
 */
UCLASS(Abstract)
class POKEMONUI_API USummaryScreenPage : public UPokemonInfoWidget {
    GENERATED_BODY()

  public:
    virtual void OnPageShown();
    virtual void OnPageHidden();

    TSubclassOf<UCommonButtonStyle> GetTabButtonStyle() const;

  private:
    /**
     * The delegate for when a page is selected
     */
    UPROPERTY(BlueprintAssignable)
    FPageSelected PageSelected;

    /**
     * The style applied to the button in the tab widget in the screen
     */
    UPROPERTY(EditAnywhere, Category = Selection)
    TSubclassOf<UCommonButtonStyle> TabButtonStyle;
};
