// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "AssetTypeActions_PaperZDAnimBP.h"
#include "PaperZDEditor.h"
#include "PaperZDAnimBP.h"
#include "Toolkits/IToolkitHost.h"
#include "Editors/Util/PaperZDEditorStyle.h"

const FName FAssetTypeActions_PaperZDAnimBP::CategoryKey = FName("PaperZD_AssetActionsKey");
const FText FAssetTypeActions_PaperZDAnimBP::CategoryDisplayName = FText::FromString("PaperZD");

FAssetTypeActions_PaperZDAnimBP::FAssetTypeActions_PaperZDAnimBP(EAssetTypeCategories::Type InAssetCategory) :
MyAssetCategory(InAssetCategory)
{
}

UClass* FAssetTypeActions_PaperZDAnimBP::GetSupportedClass() const
{
	return UPaperZDAnimBP::StaticClass();
}

void FAssetTypeActions_PaperZDAnimBP::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
	FPaperZDEditorModule *EditorModule = &FModuleManager::LoadModuleChecked<FPaperZDEditorModule>("PaperZDEditor");

	for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		UPaperZDAnimBP *AnimBP = Cast<UPaperZDAnimBP>(*ObjIt);
		if (AnimBP != NULL)
		{
			EditorModule->CreateAnimBPEditor(Mode, EditWithinLevelEditor, AnimBP);

		}
	}
}

uint32 FAssetTypeActions_PaperZDAnimBP::GetCategories()
{
	return MyAssetCategory;
}

FText FAssetTypeActions_PaperZDAnimBP::GetAssetDescription(const FAssetData& AssetData) const
{
	return FText::FromString("Drives animation and manages notify triggering by using a StateMachine");
}