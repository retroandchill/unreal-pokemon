// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

/**
 * The actions used for editing the Windowskin assets
 */
class FWindowskinAssetActions : public FAssetTypeActions_Base {
public:
	FText GetName() const override;
	UClass* GetSupportedClass() const override;
	FColor GetTypeColor() const override;
	uint32 GetCategories() override;
};
