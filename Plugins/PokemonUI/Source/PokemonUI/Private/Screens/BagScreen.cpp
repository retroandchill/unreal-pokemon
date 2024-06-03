// "Unreal Pokémon" created by Retro & Chill.

#include "Screens/BagScreen.h"
#include "Managers/PokemonSubsystem.h"
#include "Windows/ItemInfoWindow.h"
#include "Windows/ItemSelectionWindow.h"
#include "Windows/PocketWindow.h"

void UBagScreen::NativeConstruct() {
    Super::NativeConstruct();

    // TODO: Add handler for when you confirm an item
    ItemSelectionWindow->GetOnCancel().AddDynamic(this, &UBagScreen::CloseScreen);
    ItemSelectionWindow->GetOnItemChanged().AddDynamic(ItemInfoWindow, &UItemInfoWindow::Refresh);
    ItemSelectionWindow->GetOnNoItemSelected().AddDynamic(ItemInfoWindow, &UItemInfoWindow::ClearItem);
    ItemSelectionWindow->GetOnPocketChanged().AddDynamic(PocketWindow, &UPocketWindow::SetCurrentPocket);

    auto &Bag = GetGameInstance()->GetSubsystem<UPokemonSubsystem>()->GetBag();
    auto PocketName = UItemHelper::GetPocketNames()[0];
    ItemSelectionWindow->SetBag(Bag, PocketName);
    ItemSelectionWindow->SetActive(true);
    ItemSelectionWindow->SetIndex(0);
}