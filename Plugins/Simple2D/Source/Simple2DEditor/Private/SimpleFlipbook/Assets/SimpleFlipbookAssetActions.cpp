// "Unreal Pokémon" created by Retro & Chill.


#include "SimpleFlipbook/Assets/SimpleFlipbookAssetActions.h"
#include "SimpleFlipbook/Assets/SimpleFlipbook.h"

FSimpleFlipbookAssetActions::FSimpleFlipbookAssetActions(const FColor &Color, uint32 Categories) : Color(Color), Categories(Categories) {
}

FText FSimpleFlipbookAssetActions::GetName() const {
    return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_SimpleFlipbook", "Simple Flipbook");
}

UClass * FSimpleFlipbookAssetActions::GetSupportedClass() const {
    return USimpleFlipbook::StaticClass();
}

FColor FSimpleFlipbookAssetActions::GetTypeColor() const {
    return Color;
}

uint32 FSimpleFlipbookAssetActions::GetCategories() {
    return Categories;
}