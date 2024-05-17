// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Primatives/SelectableOption.h"
#include "SelectablePanel.h"

#include "PartySelectCancelPanel.generated.h"

/**
 * The panel for the cancel options in the window.
 */
UCLASS(Abstract, Blueprintable)
class POKEMONUI_API UPartySelectCancelPanel : public USelectableOption, public ISelectablePanel {
    GENERATED_BODY()

  public:
    void SetOwner(USelectableWidget *NewOwner) override;

    /**
     * Returns if this panel represents the current index of the selection
     * @return Is this panel being selected?
     */
    UFUNCTION(BlueprintPure, Category = "User Interface|Visuals")
    bool IsPanelSelected() const;

    void Refresh() override;

  protected:
    /**
     * Update the visuals of the widget.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "User Interface|Visuals")
    void RefreshVisuals();

  private:
    /**
     * The panel class that owns this one
     */
    TObjectPtr<USelectableWidget> Owner;
};
