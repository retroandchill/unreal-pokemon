// "Unreal Pokémon" created by Retro & Chill.

#include "Screens/PokemonStorageScreen.h"
#include "Components/Storage/StorageBoxWindow.h"
#include "Components/Storage/StorageInfoPanel.h"
#include "Components/WidgetSwitcher.h"
#include "Managers/PokemonSubsystem.h"
#include "Pokemon/Pokemon.h"
#include "Storage/StorageSystem.h"
#include "Utilities/InteractiveUIUtilities.h"

DEFINE_INJECTABLE_DEPENDENCY(UPokemonStorageScreen)

UPokemonStorageScreen *UPokemonStorageScreen::AddPokemonStorageScreenToStack(const UObject *WorldContextObject)
{
    return UInteractiveUIUtilities::InjectScreenToStack<UPokemonStorageScreen>(WorldContextObject).GetPtrOrNull();
}

void UPokemonStorageScreen::NativeConstruct()
{
    Super::NativeConstruct();
    auto &StorageSystem = UPokemonSubsystem::GetInstance(this).GetStorageSystem();
    StorageBoxWindow->SetStorageBox(StorageSystem->GetCurrentBox());
    StorageBoxWindow->BindToOnSelectedPokemonChanged(this, &UPokemonStorageScreen::OnSelectedPokemonChanged);
}

void UPokemonStorageScreen::OnSelectedPokemonChanged(TOptional<IPokemon &> SelectedPokemon)
{
    if (!SelectedPokemon.IsSet())
    {
        StorageInfoPanel->SetVisibility(ESlateVisibility::Hidden);
        return;
    }

    StorageInfoPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    StorageInfoPanel->SetSelectedPokemon(SelectedPokemon.GetInterface());
}