// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "SelectableWidget.h"

#include "CommandWindow.generated.h"

class UWindow;
class UDisplayTextOption;
class USizeBox;
class UScrollBox;
class UCommand;
class UGridPanel;
class SUniformGridPanel;
class UMenuCommand;
class UUniformGridPanel;

/**
 * Delegate for when the user presses confirm
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FProcessCommand, int32, CurrentIndex, UCommand *, SelectedCommand);

/**
 * Basic command window, with a set of commands that can be dispatched
 */
UCLASS(Blueprintable, Abstract)
class RPGMENUS_API UCommandWindow : public USelectableWidget {
    GENERATED_BODY()

  public:
    /**
     * Construct the default version of the window
     * @param ObjectInitializer The initializer used by Unreal Engine to build the object
     */
    explicit UCommandWindow(const FObjectInitializer &ObjectInitializer);

  protected:
    void NativePreConstruct() override;

  public:
    /**
     * Set the commands to the given values
     * @param NewCommands The commands to copy into the list of commands
     */
    void SetCommands(const TArray<TObjectPtr<UCommand>> &NewCommands);

    /**
     * Set the commands to the given values
     * @param NewCommands The commands to move into the list of commands
     */
    void SetCommands(TArray<TObjectPtr<UCommand>> &&NewCommands);

    /**
     * Get the delegate for when a command is selected
     * @return The delegate for when a command is selected
     */
    FProcessCommand &GetOnCommandSelected();

  protected:
    void ProcessConfirm_Implementation(int32 CurrentIndex) override;

  private:
    /**
     * Add the commands to the window.
     */
    void AddCommands();

    /**
     * Callback for when a command is selected
     */
    UPROPERTY(BlueprintAssignable, Category = "Selection|Confirm")
    FProcessCommand OnCommandSelected;

    /**
     * The commands displayed in the window
     */
    UPROPERTY(EditAnywhere, Instanced, Category = Commands)
    TArray<TObjectPtr<UCommand>> Commands;

    /**
     * The widget type used for the display text shown to the player
     */
    UPROPERTY(EditAnywhere, Category = Commands)
    TSubclassOf<UDisplayTextOption> DisplayTextWidgetClass;

    /**
     * The internal list of active commands
     */
    UPROPERTY()
    TArray<TObjectPtr<UCommand>> ActiveCommands;
};
