// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeCategories.h"
#include "PropertyEditorDelegates.h"
#include "Modules/ModuleManager.h"

class FGridMapEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void RegisterAssetTypeAction(class IAssetTools& AssetTools, TSharedRef<class IAssetTypeActions> Action);
	void RegisterCustomPropertyTypeLayout(FName PropertyTypeName, FOnGetPropertyTypeCustomizationInstance PropertyTypeLayoutDelegate);

	TSharedPtr<class FSlateStyleSet> StyleSet;
	TSet<FName> RegisteredPropertyTypes;
	TArray<TSharedPtr<class IAssetTypeActions>> CreatedAssetTypeActions;

	EAssetTypeCategories::Type GridMapAssetCategory;
};
