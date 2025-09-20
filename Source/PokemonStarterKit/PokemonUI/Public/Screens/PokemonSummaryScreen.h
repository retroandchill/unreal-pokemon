// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Screens/Screen.h"

#include "PokemonSummaryScreen.generated.h"

class UPaperFlipbookUserWidget;
class UPokemonSelectionPaneBase;
class USummaryTabWidget;
class ITrainer;
class UHoldItemInfo;
class IPokemon;
class USummaryPages;
class USummaryNameInfo;
class UImage;
/**
 * Screen for displaying a Pokémon's summary.
 */
UCLASS(Abstract)
class POKEMONUI_API UPokemonSummaryScreen : public UScreen
{
    GENERATED_BODY()

  public:
    UFUNCTION(BlueprintCallable, Category = Screens, meta = (WorldContext = WorldContextObject))
    static UPokemonSummaryScreen *AddPokemonSummaryScreenToStack(const UObject *WorldContextObject);

  protected:
    void NativePreConstruct() override;

  public:
    /**
     * Set the initial position in the party for cycling
     * @param Trainer The party to display the summaries of
     * @param InitialIndex The index to start at
     */
    UFUNCTION(BlueprintCallable, Category = Pokemon)
    void SetInitialPokemon(const TScriptInterface<ITrainer> &Trainer, int32 InitialIndex);

    /**
     * Set the initial position in the party for cycling
     * @param Party The party to display the summaries of
     * @param InitialIndex The index to start at
     */
    void SetInitialPokemon(TConstArrayView<TScriptInterface<IPokemon>> Party, int32 InitialIndex);

    /**
     * Get the summary pages widget.
     * @return The summary pages widget.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Widgets)
    USummaryPages *GetSummaryPages() const;

    /*
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Widgets)
    UPokemonSelectionPaneBase *GetPokemonSelection() const;
    */

  private:
    /**
     * The widget containing the name information
     */
    UPROPERTY(BlueprintGetter = GetSummaryPages, Category = Widgets, meta = (BindWidget))
    TObjectPtr<USummaryPages> SummaryPages;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USummaryTabWidget> SummaryTabs;

    /*
    UPROPERTY(BlueprintGetter = GetPokemonSelection, Category = Widgets, meta = (BindWidget))
    TObjectPtr<UPokemonSelectionPaneBase> PokemonSelection;
    */
};

DECLARE_INJECTABLE_DEPENDENCY(POKEMONUI_API, UPokemonSummaryScreen)