// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Components/Common/PokemonInfoWidget.h"
#include "Pokemon/Pokemon.h"

#include "SummaryPages.generated.h"

class USummaryScreenPage;

/**
 * Struct that contains a list of characteristics.
 */
USTRUCT(BlueprintType)
struct POKEMONUI_API FCharacteristicList {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characteristics")
    TArray<FText> Characteristics;
};

class UPokemonInfoPage;
class UWidgetSwitcher;

/**
 * The widget that controls allowing the player to scroll between pages on the summary screen.
 */
UCLASS(Abstract)
class POKEMONUI_API USummaryPages : public UPokemonInfoWidget {
    GENERATED_BODY()

  protected:

    void OnPokemonSet_Implementation(const TScriptInterface<IPokemon> &NewPokemon) override;

  public:
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
     * The widget used to actually switch the pages
     */
    UPROPERTY(BlueprintGetter = GetPageSwitcher, Category = Widgets, meta = (BindWidget))
    TObjectPtr<UWidgetSwitcher> PageSwitcher;
};