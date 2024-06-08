﻿// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Screens/Screen.h"

#include "PokemonSummaryScreen.generated.h"

class UHoldItemInfo;
class IPokemon;
class USummaryPages;
class USummaryNameInfo;
class UImage;
/**
 * Screen for displaying a Pokémon's summary.
 */
UCLASS(Abstract)
class POKEMONUI_API UPokemonSummaryScreen : public UScreen {
    GENERATED_BODY()

  protected:
    void NativeConstruct() override;

  public:
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

  private:
    /**
     * Set the Pokémon for the widgets
     * @param Pokemon The Pokémon to set to.
     */
    void SetPokemon(const TScriptInterface<IPokemon> &Pokemon);

    /**
     * Called when confirming on the summary page.
     * @param Index The index that the window is on
     */
    UFUNCTION()
    void SummaryPageConfirm(int32 Index);

    /**
     * The widget containing the name information
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USummaryNameInfo> NameInfo;

    /**
     * The widget containing the item information
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UHoldItemInfo> ItemInfo;

    /**
     * The widget that displays the Pokémon sprite
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> PokemonSprite;

    /**
     * The widget containing the name information
     */
    UPROPERTY(BlueprintGetter = GetSummaryPages, Category = Widgets, meta = (BindWidget))
    TObjectPtr<USummaryPages> SummaryPages;
};
