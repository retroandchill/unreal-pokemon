// "Unreal Pokémon" created by Retro & Chill.


#include "../../../../Simple2D/Public/Simple2D"
#include "Paper2DEditorModule.h"
#include "../../../../Simple2D/Public/Simple2D"

FText FSimpleFlipbookAssetActions::GetName() const {
    return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_SimpleFlipbook", "Simple Flipbook");
}

UClass * FSimpleFlipbookAssetActions::GetSupportedClass() const {
    return USimpleFlipbook::StaticClass();
}

FColor FSimpleFlipbookAssetActions::GetTypeColor() const {
    return FColor(129, 196, 115);
}

uint32 FSimpleFlipbookAssetActions::GetCategories() {
    return FModuleManager::GetModuleChecked<IPaper2DEditorModule>("Paper2DEditor").GetPaper2DAssetCategory();
}