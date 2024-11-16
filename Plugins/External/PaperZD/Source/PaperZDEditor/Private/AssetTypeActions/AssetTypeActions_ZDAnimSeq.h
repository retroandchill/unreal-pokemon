// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once
#include "CoreMinimal.h"
#include "AssetTypeActions_Base.h"

class UPaperZDAnimSequence;
class FAssetTypeActions_ZDAnimSeq : public FAssetTypeActions_Base
{
public:
	//Category Key for registering an advanced category
	static const FName CategoryKey;

	//Display name of the category, will be called ZetaD to avoid confusing the user with Paper2D
	static const FText CategoryDisplayName;

	//Constructor will have the custom asset category passed
	FAssetTypeActions_ZDAnimSeq(EAssetTypeCategories::Type InAssetCategory);

private:
	EAssetTypeCategories::Type MyAssetCategory;

public:
	// IAssetTypeActions Implementation
	virtual FText GetName() const override { return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_ZDAnimSeq", "AnimationSequence"); }
	virtual FColor GetTypeColor() const override { return FColor(0, 175, 255); }
	virtual UClass* GetSupportedClass() const override;
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;
	virtual uint32 GetCategories() override;
	virtual FText GetAssetDescription(const FAssetData& AssetData) const override;
	virtual void GetActions(const TArray<UObject *>& InObjects, FMenuBuilder& MenuBuilder) override;
	virtual bool HasActions(const TArray<UObject *>& InObjects) const override;
	// End IAssetTypeActions Implementation

private:
	void ExecuteReparentSequences(TArray<TWeakObjectPtr<UPaperZDAnimSequence>> InSequences);
};
