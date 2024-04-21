// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "SelectableWidget.h"

#include "CommandWindow.generated.h"

class UWindow;
class USelectableOption;
class UDisplayTextOption;
class USizeBox;
class UScrollBox;
class UCommand;
class UGridPanel;
class SUniformGridPanel;
class UMenuCommand;
class UUniformGridPanel;
class UDisplayText;

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

    TSharedRef<SWidget> RebuildWidget() override;
    void SynchronizeProperties() override;
    
#if WITH_EDITOR
    void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) override;
#endif

  protected:
    void NativeConstruct() override;
    void NativeTick(const FGeometry &MyGeometry, float InDeltaTime) override;

  public:
    int32 GetItemCount_Implementation() const override;

    /**
     * Get the maximum number of rows that appear at once
     * @return The max number of visible rows
     */
    TOptional<int32> GetPageMax();

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
    void OnSelectionChange_Implementation(int32 OldIndex, int32 NewIndex) override;
    void ProcessConfirm_Implementation(int32 CurrentIndex) override;

  private:
    /**
     * Get the position of a particular cell in the grid based on the given index.
     * @param TargetIndex The index in question to get the position of.
     * @return The row and column of the cell.
     */
    FIntVector2 GetCellPosition(int32 TargetIndex) const;

    /**
     * Add the commands to the window.
     */
    void AddCommands();

    /**
     * Set the visible status of the arrows as needed
     */
    void SetScrollArrowsVisible();

    /**
     * Callback for when a command is selected
     */
    UPROPERTY(BlueprintAssignable, Category = "Selection|Confirm")
    FProcessCommand OnCommandSelected;

    /**
     * The actual area where the window is drawn
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UWindow> Window;

    /**
     * The panel where all the commands and the cursor are placed
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UUniformGridPanel> CommandArea;

    /**
     * The scroll box used to contain the various command elements
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UScrollBox> ScrollBox;

    /**
     * The size box used to constrain the maximum number of displayed lines
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USizeBox> SizeBox;

    /**
     * The widget that acts as the cursor for the window
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UWidget> CursorWidget;

    /**
     * The up arrow widget to display when the window can scroll
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UWidget> UpArrow;

    /**
     * The down arrow widget to display when the window can scroll
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UWidget> DownArrow;

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

    /**
     * The actual widgets that make up the commands
     */
    UPROPERTY()
    TArray<TObjectPtr<UWidget>> CommandWidgets;

    /**
     * The top row being displayed by the widget
     */
    int32 TopRow = 0;

    /**
     * The widget type used for the display text shown to the player
     */
    UPROPERTY(EditAnywhere, Category = Display, meta = (editcondition = "bOverride_MaxLines", UIMin = 1, ClampMin = 1))
    int32 MaxLines = 4;

    /**
     * Override toggle for the max lines of the window. If this is true, then the value set by MaxLines is used to
     * limit how many command lines are shown.
     */
    UPROPERTY(EditAnywhere, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
    bool bOverride_MaxLines = false;

    /**
     * The height of a single command in the window
     */
    TOptional<float> CommandHeight;
};
