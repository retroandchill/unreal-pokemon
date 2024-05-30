// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Pokemon/Pokemon.h"
#include "Windows/SelectableWidget.h"

#include "SummaryPages.generated.h"

class USummaryScreenPage;
/**
 * Delegate called when the Pokémon is changed by the iterator.
 */
DECLARE_DELEGATE_OneParam(FOnPokemonChange, const TScriptInterface<IPokemon> &)

    class UPokemonInfoPage;
class UWidgetSwitcher;

/**
 * The widget that controls allowing the player to scroll between pages on the summary screen.
 */
UCLASS(Abstract)
class POKEMONUI_API USummaryPages : public USelectableWidget {
    GENERATED_BODY()

  public:
    /**
     * Refresh any of the child pages added to this widget.
     * @param Pokemon The Pokémon in question to refresh the data for
     */
    UFUNCTION(BlueprintImplementableEvent, Category = Events)
    void Refresh(const TScriptInterface<IPokemon> &Pokemon);

    /**
     * Set the initial Pokémon to use with this iterator.
     * @param Pokemon The Pokémon in question to look at.
     * @param Index The index of said Pokémon.
     */
    void SetInitialPokemon(TConstArrayView<TScriptInterface<IPokemon>> Pokemon, int32 Index);

    /**
     * Get the delegate that is called when a Pokémon is changed.
     * @return Delegate that is called when a Pokémon is changed.
     */
    FOnPokemonChange &GetOnPokemonChange();

    /**
     * Get the Pokémon that is currently being displayed.
     * @return The currently displayed Pokémon
     */
    const TScriptInterface<IPokemon> &GetCurrentPokemon() const;

    /**
     * Get the page at the given index.
     * @param PageIndex The index to search at
     * @return The found page
     */
    UFUNCTION(BlueprintPure, BlueprintImplementableEvent, Category = Widgets)
    USummaryScreenPage *GetPage(int32 PageIndex) const;

  protected:
    /**
     * Get the page switching widget.
     * @return The widget used to swap what page is shown.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Widgets)
    UWidgetSwitcher *GetPageSwitcher() const;

    int32 GetItemCount_Implementation() const override;
    int32 GetColumnCount_Implementation() const override;
    void OnSelectionChange_Implementation(int32 OldIndex, int32 NewIndex) override;
    void ReceiveMoveCursor(ECursorDirection Direction) override;

  private:
    /**
     * Iterator used to cycle through the part
     */
    FPokemonIterator CurrentPokemon;

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