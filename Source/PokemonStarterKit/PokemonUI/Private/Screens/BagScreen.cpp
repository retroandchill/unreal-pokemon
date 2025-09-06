// "Unreal Pokémon" created by Retro & Chill.

#include "Screens/BagScreen.h"
#include "Components/Bag/ItemSelectionWindow.h"
#include "Components/Bag/PocketTabWidget.h"
#include "DataManager.h"
#include "Engine/GameInstance.h"
#include "Field/FieldItemEffectDirect.h"
#include "Field/FieldItemEffectUseOnPokemon.h"
#include "ItemEffectLookup.h"
#include "Managers/PokemonSubsystem.h"
#include "Player/Bag.h"
#include "Utilities/RPGMenuUtilities.h"
#include "Utilities/TrainerHelpers.h"

DEFINE_INJECTABLE_DEPENDENCY(UBagScreen)

UBagScreen *UBagScreen::AddBagScreenToStack(const UObject *WorldContextObject)
{
    return URPGMenuUtilities::InjectScreenToStack<UBagScreen>(WorldContextObject).GetPtrOrNull();
}

void UBagScreen::NativeConstruct()
{
    Super::NativeConstruct();

    auto &Bag = GetGameInstance()->GetSubsystem<UPokemonSubsystem>()->GetBag();
    ItemSelectionWindow->SetBag(Bag);
    PocketTabWidget->SetItemSelectionWindow(ItemSelectionWindow);
    ItemSelectionWindow->ActivateWidget();
}

void UBagScreen::ApplyItemFilter(const FItemFilter &ItemFilter)
{
    RefreshScene();
}

UE5Coro::TCoroutine<TOptional<FSelectedItemHandle>> UBagScreen::PromptItemSelection()
{
    auto [Result] = co_await OnItemSelected;
    co_return Result;
}

void UBagScreen::ToggleItemSelection(bool bCanSelect)
{
    if (bCanSelect)
    {
        ItemSelectionWindow->ActivateWidget();
    }
    else
    {
        ItemSelectionWindow->DeactivateWidget();
    }
}

void UBagScreen::RemoveFromStack()
{
    CloseScreen();
}

void UBagScreen::CloseScreen()
{
    Super::CloseScreen();
    (void)OnItemSelected.ExecuteIfBound({});
}

void UBagScreen::RefreshScene()
{
    RefreshSelf();
}

void UBagScreen::RefreshSelf_Implementation()
{
    Super::RefreshSelf_Implementation();
    ItemSelectionWindow->RefreshWindow();
}

UItemSelectionWindow *UBagScreen::GetItemSelectionWindow() const
{
    return ItemSelectionWindow;
}

UItemInfoWindow *UBagScreen::GetItemInfoWindow() const
{
    return ItemInfoWindow;
}

UPocketTabWidget *UBagScreen::GetPocketTabWidget() const
{
    return PocketTabWidget;
}

UPocketWindow *UBagScreen::GetPocketWindow() const
{
    return PocketWindow;
}

void UBagScreen::SelectItem(const FItem &Item, int32 Quantity)
{
    if (OnItemSelected.IsBound())
    {
        OnItemSelected.Execute(FSelectedItemHandle(this, Item, Quantity));
        return;
    }

    ToggleItemSelection(false);
    ShowItemCommands();
}

void UBagScreen::OnItemEffectConclude(bool bSuccess, FName ItemID)
{
    auto &Item = FDataManager::GetInstance().GetDataTable<FItem>().GetDataChecked(ItemID);
    if (bSuccess && Item.Consumable)
    {
        UTrainerHelpers::GetBag(this)->RemoveItem(ItemID, 1);
    }

    RefreshScene();
    CurrentItemEffect = nullptr;
}