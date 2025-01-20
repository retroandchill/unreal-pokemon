// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "AssetTypeActions_Base.h"

/**
 * 
 */
class SIMPLE2DEDITOR_API FSimpleFlipbookAssetActions : public FAssetTypeActions_Base {
public:
    FSimpleFlipbookAssetActions(const FColor& Color, uint32 Categories);
    
    FText GetName() const override;
    UClass *GetSupportedClass() const override;
    FColor GetTypeColor() const override;
    uint32 GetCategories() override;

private:
    FColor Color;
    uint32 Categories;
};
