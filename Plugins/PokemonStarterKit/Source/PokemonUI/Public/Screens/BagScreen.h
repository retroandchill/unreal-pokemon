// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "InventoryScreen.h"
#include "Screens/Screen.h"

#include "BagScreen.generated.h"

class UPokemonSelectionPane;
class UPocketTabWidget;
class UBagMenuHandlerSet;
class UCommand;
class UBagMenuHandler;
class UCommandWindow;
struct FItem;
class UPocketWindow;
class UItemInfoWindow;
class UItemSelectionWindow;

/**
 * The screen that contains the basic outline of the player's bag
 */
UCLASS(Abstract, Blueprintable)
class POKEMONUI_API UBagScreen : public UScreen, public IInventoryScreen {
    GENERATED_BODY()

  protected:
    void NativeConstruct() override;

  public:
    UFUNCTION(BlueprintCallable, Category = "Items|Selection")
    void ApplyItemFilter(const FItemFilter &ItemFilter) override;

    FOnItemSelected &GetOnItemSelected() final;

    UFUNCTION(BlueprintCallable, Category = "Items|Selection")
    void ToggleItemSelection(bool bCanSelect) final;

    UFUNCTION(BlueprintCallable, Category = Navigation)
    void RemoveFromStack() override;

    void CloseScreen() override;

    UFUNCTION(BlueprintCallable, Category = Display)
    void RefreshScene() override;

  protected:
    void RefreshSelf_Implementation() override;

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Widgets)
    UItemSelectionWindow* GetItemSelectionWindow() const;

  private:
    /**
     * Create the commands for the given item
     * @param Item The selected item
     * @param Quantity How many of the item you have
     */
    void CreateCommands(const FItem &Item, int32 Quantity);

    /**
     * Called when an item is selected by the player
     * @param Item The item that has been selected.
     * @param Quantity The quantity that has been selected
     */
    UFUNCTION()
    void SelectItem(const FItem &Item, int32 Quantity);

    /**
     * Called when a command is selected.
     * @param CommandIndex The index of the command
     * @param Command The command object
     */
    UFUNCTION()
    void OnItemCommandSelected(int32 CommandIndex, UCommand *Command);

    /**
     * Called then the player cancels in the item command window
     */
    UFUNCTION()
    void OnItemCommandCanceled();

    /**
     * The window that displays the pockets
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UPocketWindow> PocketWindow;

    /**
     * The window that displays the item list
     */
    UPROPERTY(BlueprintGetter = GetItemSelectionWindow, Category = Widgets, meta = (BindWidget))
    TObjectPtr<UItemSelectionWindow> ItemSelectionWindow;

    /**
     * The window that displays the item information
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UItemInfoWindow> ItemInfoWindow;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UPocketTabWidget> PocketTabWidget;

    /**
     * The window that contains the command to call on an item
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCommandWindow> CommandWindow;

    /**
     * The handlers in the menu to apply to item selections
     */
    UPROPERTY(EditAnywhere, Category = Selection)
    TObjectPtr<UBagMenuHandlerSet> CommandHandlers;

    /**
     * Callback for when an item is selected
     */
    FOnItemSelected OnItemSelected;
};
