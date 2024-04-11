// "Unreal Pokémon" created by Retro & Chill.
#include "Windowskin/WindowskinAssetActions.h"

#include "Data/Windowskin.h"

FText FWindowskinAssetActions::GetName() const {
    return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_Windowskin", "Windowskin");
}

UClass *FWindowskinAssetActions::GetSupportedClass() const { return UWindowskin::StaticClass(); }

FColor FWindowskinAssetActions::GetTypeColor() const { return FColor::Silver; }

uint32 FWindowskinAssetActions::GetCategories() { return EAssetTypeCategories::UI; }
