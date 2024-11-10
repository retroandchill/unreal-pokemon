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
class POKEMONUI_API USummaryPages : public UPokemonInfoWidget {
    GENERATED_BODY()

  protected:
    void NativeConstruct() override;

    void OnPokemonSet_Implementation(const TScriptInterface<IPokemon> &NewPokemon) override;

  public:
    /**
     * Get the delegate that is called when a Pokémon is changed.
     * @return Delegate that is called when a Pokémon is changed.
     */
    FOnPokemonChange &GetOnPokemonChange();

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
     * Called when a Pokémon is changed
     */
    FOnPokemonChange OnPokemonChange;

    /**
     * The widget used to actually switch the pages
     */
    UPROPERTY(BlueprintGetter = GetPageSwitcher, Category = Widgets, meta = (BindWidget))
    TObjectPtr<UWidgetSwitcher> PageSwitcher;
};