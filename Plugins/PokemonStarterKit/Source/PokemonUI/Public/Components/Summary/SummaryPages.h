// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Components/SelectableWidget.h"
#include "Pokemon/Pokemon.h"
#include <range/v3/view/any_view.hpp>

#include "SummaryPages.generated.h"

class USummaryScreenPage;

/**
 * Delegate called when the Pokémon is changed by the iterator.
 */
DECLARE_DELEGATE_OneParam(FOnPokemonChange, const TScriptInterface<IPokemon> &);

class UPokemonInfoPage;
class UWidgetSwitcher;

/**
 * The widget that controls allowing the player to scroll between pages on the summary screen.
 */
UCLASS(Abstract)
class POKEMONUI_API USummaryPages : public UCommonUserWidget {
    GENERATED_BODY()

  protected:
    void NativeConstruct() override;

  public:
    /**
     * Refresh any of the child pages added to this widget.
     * @param Pokemon The Pokémon in question to refresh the data for
     */
    UFUNCTION(BlueprintImplementableEvent, Category = Events)
    void Refresh(const TScriptInterface<IPokemon> &Pokemon);

    /**
     * Set the initial Pokémon to use with this iterator.
     * @param NewPokemon The index of said Pokémon.
     */
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = Content)
    void SetPokemon(const TScriptInterface<IPokemon> &NewPokemon);

    /**
     * Get the delegate that is called when a Pokémon is changed.
     * @return Delegate that is called when a Pokémon is changed.
     */
    FOnPokemonChange &GetOnPokemonChange();

    /**
     * Get the Pokémon that is currently being displayed.
     * @return The currently displayed Pokémon
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Content)
    const TScriptInterface<IPokemon> &GetCurrentPokemon() const;

    /**
     * Get the page at the given index.
     * @param PageIndex The index to search at
     * @return The found page
     */
    UFUNCTION(BlueprintPure, BlueprintImplementableEvent, Category = Widgets)
    USummaryScreenPage *GetPage(int32 PageIndex) const;

    UFUNCTION(BlueprintCallable, Category = Widgets)
    void SetPage(int32 PageIndex);

    TArray<USummaryScreenPage *> GetPages() const;

    int32 GetCurrentPageIndex() const;

    void NextPage();
    void PreviousPage();

  protected:
    /**
     * Get the page switching widget.
     * @return The widget used to swap what page is shown.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Widgets)
    UWidgetSwitcher *GetPageSwitcher() const;

  private:
    /**
     * The Pokémon that is currently the subject for this widget
     */
    UPROPERTY(BlueprintGetter = GetCurrentPokemon, BlueprintSetter = SetPokemon, Category = Content)
    TScriptInterface<IPokemon> CurrentPokemon;

    /**
     * Called when a Pokémon is changed
     */
    FOnPokemonChange OnPokemonChange;

    /**
     * The widget used to actually switch the pages
     */
    UPROPERTY(BlueprintGetter = GetPageSwitcher, Category = Widgets, meta = (BindWidget))
    TObjectPtr<UWidgetSwitcher> PageSwitcher;
};