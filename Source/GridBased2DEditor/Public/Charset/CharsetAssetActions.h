// Created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"
#include "IAssetTypeActions.h"

/**
 * The actions used for editing the Charset assets
 */
class FCharsetAssetActions : public FAssetTypeActions_Base {
public:
 /**
  * Create an asset action for Charset assets with the specified Asset Category
  * @param InAssetCategory 
  */
 explicit FCharsetAssetActions(EAssetTypeCategories::Type InAssetCategory);
 
 FText GetName() const override;
 UClass* GetSupportedClass() const override;
 FColor GetTypeColor() const override;
 uint32 GetCategories() override;

private:
 /**
  * The category for this asset type
  */
 EAssetTypeCategories::Type AssetCategory;
};
