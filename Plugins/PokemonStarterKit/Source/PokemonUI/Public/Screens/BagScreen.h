// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Bag/Item.h"
#include "Field/FieldItemEffect.h"
#include "InventoryScreen.h"
#include "ItemEffectLookup.h"
#include "Screens/Screen.h"

#include "BagScreen.generated.h"

class UFieldItemEffect;
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

  public:
    UFUNCTION(BlueprintCallable, Category = Screens, meta = (WorldContext = WorldContextObject))
    static UBagScreen *AddBagScreenToStack(const UObject *WorldContextObject);

  protected:
    void NativeConstruct() override;

  public:
    UFUNCTION(BlueprintCallable, Category = "Items|Selection")
    void ApplyItemFilter(const FItemFilter &ItemFilter) override;

    UE5Coro::TCoroutine<TOptional<FSelectedItemHandle>> PromptItemSelection() override;

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
    UItemSelectionWindow *GetItemSelectionWindow() const;

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Widgets)
    UItemInfoWindow *GetItemInfoWindow() const;

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Widgets)
    UPocketTabWidget *GetPocketTabWidget() const;

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Widgets)
    UPocketWindow *GetPocketWindow() const;

    /**
     * Called when an item is selected by the player
     * @param Item The item that has been selected.
     * @param Quantity The quantity that has been selected
     */
    UFUNCTION(BlueprintCallable, Category = Items)
    void SelectItem(const FItem &Item, int32 Quantity);

    UFUNCTION(BlueprintImplementableEvent, Category = Items)
    void ShowItemCommands();

  public:
    template <typename T, typename... A>
        requires std::is_base_of_v<UFieldItemEffect, T>
    void TryUseItem(const FItem &Item, int32 Quantity, FOnItemEffectComplete::FDelegate &&CompletionDelegate,
                    A &&...Args) {
        auto EffectClass = Pokemon::Items::FieldItemEffects.LoadClass<T>(Item.ID);
        if (!EffectClass.IsSet()) {
            OnItemEffectConclude(false, Item.ID);
            CompletionDelegate.Execute(false);
            return;
        }

        auto Effect = NewObject<T>(this, EffectClass.GetValue());
        Effect->BindToOnEffectComplete(
            this, [this, ItemID = Item.ID, Callback = std::move(CompletionDelegate)](bool bSuccess) {
                OnItemEffectConclude(bSuccess, ItemID);
                Callback.Execute(bSuccess);
            });
        Effect->Use(Item, Quantity, std::forward<A>(Args)...);
        CurrentItemEffect = Effect;
    }

  private:
    void OnItemEffectConclude(bool bSuccess, FName ItemID);

    /**
     * The window that displays the pockets
     */
    UPROPERTY(BlueprintGetter = GetPocketWindow, Category = Widgets, meta = (BindWidget))
    TObjectPtr<UPocketWindow> PocketWindow;

    /**
     * The window that displays the item list
     */
    UPROPERTY(BlueprintGetter = GetItemSelectionWindow, Category = Widgets, meta = (BindWidget))
    TObjectPtr<UItemSelectionWindow> ItemSelectionWindow;

    /**
     * The window that displays the item information
     */
    UPROPERTY(BlueprintGetter = GetItemInfoWindow, Category = Widgets, meta = (BindWidget))
    TObjectPtr<UItemInfoWindow> ItemInfoWindow;

    UPROPERTY(BlueprintGetter = GetPocketTabWidget, Category = Widgets, meta = (BindWidget))
    TObjectPtr<UPocketTabWidget> PocketTabWidget;

    /**
     * Callback for when an item is selected
     */
    FOnItemSelected OnItemSelected;

    UPROPERTY()
    TObjectPtr<UFieldItemEffect> CurrentItemEffect;
};

DECLARE_INJECTABLE_DEPENDENCY(POKEMONUI_API, UBagScreen)