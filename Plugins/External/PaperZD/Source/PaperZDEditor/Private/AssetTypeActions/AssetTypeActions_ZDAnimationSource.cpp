// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "AssetTypeActions/AssetTypeActions_ZDAnimationSource.h"
#include "AnimSequences/Sources/PaperZDAnimationSource.h"
#include "Editors/Util/PaperZDEditorStyle.h"
#include "Toolkits/IToolkitHost.h"
#include "Modules/ModuleManager.h"
#include "PaperZDEditor.h"

#define LOCTEXT_NAMESPACE "AssetTypeActionsPaperZDAnimationSource"

//static defines
const FName FAssetTypeActions_ZDAnimationSource::CategoryKey = FName("PaperZD_AssetActionsKey");
const FText FAssetTypeActions_ZDAnimationSource::CategoryDisplayName = FText::FromString("PaperZD");

FAssetTypeActions_ZDAnimationSource::FAssetTypeActions_ZDAnimationSource(EAssetTypeCategories::Type InAssetCategory) :
MyAssetCategory(InAssetCategory)
{}

FText FAssetTypeActions_ZDAnimationSource::GetName() const 
{ 
	return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_ZDAnimationSource", "AnimationSource");
}

UClass* FAssetTypeActions_ZDAnimationSource::GetSupportedClass() const
{
	return UPaperZDAnimationSource::StaticClass();
}

void FAssetTypeActions_ZDAnimationSource::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
	FPaperZDEditorModule* EditorModule = &FModuleManager::LoadModuleChecked<FPaperZDEditorModule>("PaperZDEditor");

	//Open the asset editor for the selected objects
	for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		UPaperZDAnimationSource* AnimSource = Cast<UPaperZDAnimationSource>(*ObjIt);
		if (AnimSource)
		{
			EditorModule->CreateAnimationSourceEditor(Mode, EditWithinLevelEditor, AnimSource);
		}
	}
}

uint32 FAssetTypeActions_ZDAnimationSource::GetCategories()
{
	return MyAssetCategory;
}

#undef LOCTEXT_NAMESPACE
