// "Unreal Pokémon" created by Retro & Chill.
#include "Charset/CharsetAssetActions.h"

#include "Characters/Charset.h"


FCharsetAssetActions::FCharsetAssetActions(EAssetTypeCategories::Type InAssetCategory) : AssetCategory(
	InAssetCategory) {
}

FText FCharsetAssetActions::GetName() const {
	return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_Charset", "Character Set");
}

UClass* FCharsetAssetActions::GetSupportedClass() const {
	return UCharset::StaticClass();
}

FColor FCharsetAssetActions::GetTypeColor() const {
	return FColor::Blue;
}

uint32 FCharsetAssetActions::GetCategories() {
	return AssetCategory;
}
