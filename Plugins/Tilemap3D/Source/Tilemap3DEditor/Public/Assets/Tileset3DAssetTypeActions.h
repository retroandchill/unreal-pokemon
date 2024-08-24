// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

/**
 * 
 */
class TILEMAP3DEDITOR_API FTileset3DAssetTypeActions : public FAssetTypeActions_Base {
public:
    explicit FTileset3DAssetTypeActions(EAssetTypeCategories::Type AssetCategory);
    UClass* GetSupportedClass() const override;
    FText GetName() const override;
    FColor GetTypeColor() const override;
    uint32 GetCategories() override;

private:
    EAssetTypeCategories::Type AssetCategory;
};
