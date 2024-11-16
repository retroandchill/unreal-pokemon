// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "AssetTypeActions/AssetTypeActions_ZDAnimSeq.h"
#include "AnimSequences/PaperZDAnimSequence.h"
#include "AnimSequences/Sources/PaperZDAnimationSource.h"
#include "Toolkits/IToolkitHost.h"
#include "Editors/Util/PaperZDEditorStyle.h"
#include "Editors/Slate/SPaperZDConfirmDialog.h"
#include "Modules/ModuleManager.h"
#include "IContentBrowserSingleton.h"
#include "ContentBrowserModule.h"
#include "Widgets/SWindow.h"
#include "Widgets/Layout/SBorder.h"
#include "Notifies/PaperZDAnimNotifyCustom.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Misc/ScopedSlowTask.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "PaperZDEditor.h"

#include "Editor.h"

//Styles
#include "Styling/AppStyle.h"

#define LOCTEXT_NAMESPACE "AssetTypeActionsPaperZDAnimSeq"

const FName FAssetTypeActions_ZDAnimSeq::CategoryKey = FName("PaperZD_AssetActionsKey");
const FText FAssetTypeActions_ZDAnimSeq::CategoryDisplayName = FText::FromString("PaperZD");

FAssetTypeActions_ZDAnimSeq::FAssetTypeActions_ZDAnimSeq(EAssetTypeCategories::Type InAssetCategory) :
MyAssetCategory(InAssetCategory)
{
}

UClass* FAssetTypeActions_ZDAnimSeq::GetSupportedClass() const
{
	return UPaperZDAnimSequence::StaticClass();
}

void FAssetTypeActions_ZDAnimSeq::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
	FPaperZDEditorModule* EditorModule = &FModuleManager::LoadModuleChecked<FPaperZDEditorModule>("PaperZDEditor");

	for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		UPaperZDAnimSequence* Sequence = Cast<UPaperZDAnimSequence>(*ObjIt);
		if (Sequence && Sequence->GetAnimSource())
		{
			EditorModule->CreateAnimationSourceEditor(Mode, EditWithinLevelEditor, Sequence->GetAnimSource(), Sequence);
		}
		else
		{
			//Not a valid Animation source parented to the Sequence, show error message
			TSharedRef<SPaperZDConfirmDialog> Dialog = SNew(SPaperZDConfirmDialog)
				.TitleText(FText::FromString(TEXT("Warning")))
				.DetailText(FText::FromString(TEXT("Selected AnimSequence isn't parented to a valid animation source, please reparent it!")))
				.ShowCancelButton(false);
			Dialog->Show();
		}
	}
}

uint32 FAssetTypeActions_ZDAnimSeq::GetCategories()
{
	return MyAssetCategory;
}

FText FAssetTypeActions_ZDAnimSeq::GetAssetDescription(const FAssetData& AssetData) const
{
	return FText::FromString("Contains render and notify data that can be used to drive a PaperZD Animation Blueprint");
}

bool FAssetTypeActions_ZDAnimSeq::HasActions(const TArray<UObject *>& InObjects) const
{
	return true;
}

void FAssetTypeActions_ZDAnimSeq::GetActions(const TArray<UObject *>& InObjects, FMenuBuilder& MenuBuilder)
{
	TArray<TWeakObjectPtr<UPaperZDAnimSequence>> AnimSequences = GetTypedWeakObjectPtrs<UPaperZDAnimSequence>(InObjects);

	MenuBuilder.AddMenuEntry(
		LOCTEXT("AnimSequence_Reparent", "Reparent Sequence"),
		LOCTEXT("AnimSequence_ReparentTooltip", "Changes the AnimBP parent of this AnimSequence, registering all the custom notifies missing on the target."),
		//FSlateIcon(FEditorStyle::GetStyleSetName(), "ClassIcon.PaperFlipbook"),
		FSlateIcon(FPaperZDEditorStyle::GetStyleSetName(), "ClassThumbnail.PaperZDAnimBP"),
		FUIAction(
			FExecuteAction::CreateSP(this, &FAssetTypeActions_ZDAnimSeq::ExecuteReparentSequences, AnimSequences),
			FCanExecuteAction()
		)
	);
}

void FAssetTypeActions_ZDAnimSeq::ExecuteReparentSequences(TArray<TWeakObjectPtr<UPaperZDAnimSequence>> InSequences)
{
	//First we need the user to select the target AnimBP
	// Load the content browser module to display an asset picker
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	FAssetPickerConfig AssetPickerConfig;

	/** The asset picker will only show animation sources */
	AssetPickerConfig.Filter.ClassPaths.Add(UPaperZDAnimationSource::StaticClass()->GetClassPathName());
	AssetPickerConfig.Filter.bRecursiveClasses = true;

	/** The delegate that fires when an asset was selected */
	UPaperZDAnimationSource* SelectedAnimSource = nullptr;
	TSharedPtr<SWindow> ReparentPickerWindow;
	AssetPickerConfig.OnAssetSelected = FOnAssetSelected::CreateLambda(
		[&SelectedAnimSource, &ReparentPickerWindow](const FAssetData SelectedAsset)
		{
			SelectedAnimSource = Cast<UPaperZDAnimationSource>(SelectedAsset.GetAsset());
			ReparentPickerWindow->RequestDestroyWindow();
		}
	);

	/** The default view mode should be a list view */
	AssetPickerConfig.InitialAssetViewType = EAssetViewType::List;
	
	ReparentPickerWindow = SNew(SWindow)
		.Title(LOCTEXT("ReparentAnimSequenceOptions", "Pick Parent Animation Blueprint"))
		.ClientSize(FVector2D(500, 600))
		.SupportsMinimize(false).SupportsMaximize(false)
		[
			SNew(SBorder)
			.BorderImage(FAppStyle::GetBrush("Menu.Background"))
			[
				ContentBrowserModule.Get().CreateAssetPicker(AssetPickerConfig)
			]
		];

	GEditor->EditorAddModalWindow(ReparentPickerWindow.ToSharedRef());
	ReparentPickerWindow.Reset();

	//Actually perform the re-parenting
	if (SelectedAnimSource)
	{
		FScopedSlowTask SlowTask(InSequences.Num(), LOCTEXT("ImportSlowTask", "Importing"));
		SlowTask.MakeDialog();

		bool bAnimBPNeedsCompile = false;
		for (TWeakObjectPtr<UPaperZDAnimSequence> SequencePtr : InSequences)
		{
			if (SequencePtr.IsValid())
			{
				SlowTask.EnterProgressFrame(1, FText::Format(LOCTEXT("PaperZD_ReparentingProgress", "Reparenting \"{0}\"..."), FText::FromString(SequencePtr->GetName())));

				//No sense on applying changes if the sequence is already parented to the target animbp
				if (SequencePtr->GetAnimSource() != SelectedAnimSource)
				{
					TArray<UPaperZDAnimNotify_Base*> AnimNotifies = SequencePtr->GetAnimNotifies();
					for (UPaperZDAnimNotify_Base* Notify : AnimNotifies)
					{
						//We only care about the custom notifies, as they need a registered function on the AnimBPs that are using this source to work
						if (UPaperZDAnimNotifyCustom* CustomNotify = Cast<UPaperZDAnimNotifyCustom>(Notify))
						{
							//On custom notifies, the name points to the bound function identifier
							bAnimBPNeedsCompile |= SelectedAnimSource->RegisterCustomNotify(CustomNotify->Name);
						}
					}

					//Actually re-parent and set to save
					SequencePtr->SetAnimSource(SelectedAnimSource);
					SequencePtr->MarkPackageDirty();
				}
			}
		}

		if (bAnimBPNeedsCompile)
		{
			//@TODO: Grab all the blueprints that need recompile
			//COULD THIS BE CHANGED TO A "ADD SKELETON NOTIFY" KIND OF DEAL?
			//FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(SelectedAnimSource);
		}
	}
}

#undef LOCTEXT_NAMESPACE
