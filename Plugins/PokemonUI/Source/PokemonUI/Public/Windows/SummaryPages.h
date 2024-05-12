// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Windows/SelectableWidget.h"
#include "SummaryPages.generated.h"

class IPokemon;
class UPokemonInfoPage;
class UWidgetSwitcher;
/**
 * The widget that controls allowing the player to scroll between pages on the summary screen.
 */
UCLASS(Abstract)
class POKEMONUI_API USummaryPages : public USelectableWidget {
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintImplementableEvent, Category = Events)
    void Refresh(const TScriptInterface<IPokemon>& Pokemon);

protected:
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Widgets)
    UWidgetSwitcher* GetPageSwitcher() const;

private:
    /**
     * The widget used to actually switch the pages
     */
    UPROPERTY(BlueprintGetter = GetPageSwitcher, Category = Widgets, meta = (BindWidget))
    TObjectPtr<UWidgetSwitcher> PageSwitcher;

};