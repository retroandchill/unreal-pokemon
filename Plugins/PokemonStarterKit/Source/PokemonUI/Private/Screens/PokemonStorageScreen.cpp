// "Unreal Pokémon" created by Retro & Chill.


#include "Screens/PokemonStorageScreen.h"
#include "Components/Storage/StorageBoxWindow.h"
#include "Managers/PokemonSubsystem.h"
#include "Storage/StorageSystem.h"

void UPokemonStorageScreen::NativeConstruct() {
    Super::NativeConstruct();
    auto &StorageSystem = UPokemonSubsystem::GetInstance(this).GetStorageSystem();
    StorageBoxWindow->SetStorageBox(StorageSystem->GetCurrentBox());
}