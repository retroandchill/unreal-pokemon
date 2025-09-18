// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Components/SelectableWidget.h"

#include "BattleSwitchPane.generated.h"

class IBattler;
class IBattle;
class UBattlerSwitchPanel;

/**
 * Delegate called when the selected battler is changed in the widget.
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectedBattlerChanged, const TScriptInterface<IBattler> &, Battler);

/**
 * The pane that holds the battler options for switching in combat
 */
UCLASS(Abstract)
class POKEMONBATTLEUI_API UBattleSwitchPane : public USelectableWidget
{
    GENERATED_BODY()

  public:
    /**
     * Set the current battle for this pane
     * @param Battle The battle for this particular pane
     */
    void SetBattle(const TScriptInterface<IBattle> &Battle);

    /**
     * Refresh the contents of the window
     */
    UFUNCTION(BlueprintCallable, Category = Content)
    void RefreshContent();

  protected:
    void NativeOnSelectionChange(int32 OldIndex, int32 NewIndex) override;

  private:
    /**
     * The widget class for the panels that will be created.
     */
    UPROPERTY(EditAnywhere, Category = Content)
    TSubclassOf<UBattlerSwitchPanel> PanelClass;

    UPROPERTY()
    TScriptInterface<IBattle> CurrentBattle;

    /**
     * Delegate called when the selected battler is changed in the widget.
     */
    UPROPERTY(BlueprintAssignable, Category = Events)
    FOnSelectedBattlerChanged OnSelectedBattlerChanged;
};
