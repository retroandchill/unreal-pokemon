// "Unreal Pokémon" created by Retro & Chill.

#include "Assets/Tileset3DAssetTypeActions.h"
#include "Tileset/Tileset3D.h"

FTileset3DAssetTypeActions::FTileset3DAssetTypeActions(EAssetTypeCategories::Type AssetCategory)
    : AssetCategory(AssetCategory) {
}

UClass *FTileset3DAssetTypeActions::GetSupportedClass() const {
    return UTileset3D::StaticClass();
}

FText FTileset3DAssetTypeActions::GetName() const {
    return INVTEXT("3D Tileset");
}

FColor FTileset3DAssetTypeActions::GetTypeColor() const {
    return FColorList::Orange;
}

uint32 FTileset3DAssetTypeActions::GetCategories() {
    return AssetCategory;
}