// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SummaryScreenPage.generated.h"

class UCommonButtonStyle;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPageSelected);

class IPokemon;

/**
 * Abstract declaration of a page on the summary screen.
 */
UCLASS(Abstract)
class POKEMONUI_API USummaryScreenPage : public UUserWidget {
    GENERATED_BODY()

  public:
    /**
     * Refresh the pages information with information from the given Pokémon.
     * @param Pokemon The Pokémon to refresh the information for
     */
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = Summary)
    void RefreshInfo(const TScriptInterface<IPokemon> &Pokemon);

    /**
     * Can the player select this page to view something else?
     * @return Can this page be selected.
     */
    UFUNCTION(BlueprintPure, BlueprintNativeEvent, Category = Selection)
    bool CanSelectOnPage() const;

    /**
     * Get the page selected delegate.
     * @return The delegate for when a page is selected
     */
    FPageSelected &GetPageSelected();

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
