// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Factories/PaperZDAnimSequenceFactory.h"
#include "AnimSequences/Sources/PaperZDAnimationSource.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SWindow.h"
#include "Widgets/Layout/SBorder.h"
#include "Styling/AppStyle.h"
#include "AnimSequences/PaperZDAnimSequence.h"
#include "Modules/ModuleManager.h"
#include "IContentBrowserSingleton.h"
#include "ContentBrowserModule.h"
#include "Misc/FeedbackContext.h"
#include "Editor.h"
#include "AnimSequences/Sources/PaperZDAnimationSource_Flipbook.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDAnimSequenceFactory)
#endif

#define LOCTEXT_NAMESPACE "ZDAnimSequenceFactory_Flipbook"

//////////////////////////////////////////////////////////////////////////
//// Factory
//////////////////////////////////////////////////////////////////////////
UPaperZDAnimSequenceFactory::UPaperZDAnimSequenceFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCreateNew = true;
	SupportedClass = UPaperZDAnimSequence::StaticClass();
}

bool UPaperZDAnimSequenceFactory::ConfigureProperties()
{
	// Optionally select the AnimBP if it hasn't been provided yet
	if (TargetAnimSource == nullptr)
	{
		// Load the content browser module to display an asset picker
		FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

		FAssetPickerConfig AssetPickerConfig;

		/** The asset picker will only show AnimBP */
		AssetPickerConfig.Filter.ClassPaths.Add(UPaperZDAnimationSource::StaticClass()->GetClassPathName());
		AssetPickerConfig.Filter.bRecursiveClasses = true;

		/** The delegate that fires when an asset was selected */
		AssetPickerConfig.OnAssetSelected = FOnAssetSelected::CreateUObject(this, &UPaperZDAnimSequenceFactory::OnTargetAnimSourceSelected);

		/** The default view mode should be a list view */
		AssetPickerConfig.InitialAssetViewType = EAssetViewType::List;

		PickerWindow = SNew(SWindow)
			.Title(LOCTEXT("CreateAnimSequenceOptions", "Pick Parent Animation Source"))
			.ClientSize(FVector2D(500, 600))
			.SupportsMinimize(false).SupportsMaximize(false)
			[
				SNew(SBorder)
				.BorderImage(FAppStyle::GetBrush("Menu.Background"))
				[
					ContentBrowserModule.Get().CreateAssetPicker(AssetPickerConfig)
				]
			];

		GEditor->EditorAddModalWindow(PickerWindow.ToSharedRef());
		PickerWindow.Reset();
	}

	//Will work if we have an AnimSource
	return TargetAnimSource != nullptr;
}

UObject* UPaperZDAnimSequenceFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	if (TargetAnimSource && TargetAnimSource->GetSupportedAnimSequenceClass())
	{
		UPaperZDAnimSequence* AnimSequence = NewObject<UPaperZDAnimSequence>(InParent, TargetAnimSource->GetSupportedAnimSequenceClass(), Name, Flags);
		AnimSequence->SetAnimSource(TargetAnimSource);
		return AnimSequence;
	}
	else if (TargetAnimSource->GetSupportedAnimSequenceClass() == nullptr)
	{
		Warn->Logf(ELogVerbosity::Error, TEXT("Could not create Animation, Animation Source has no valid default supported animation class."));
	}

	return nullptr;
}

void UPaperZDAnimSequenceFactory::OnTargetAnimSourceSelected(const FAssetData& SelectedAsset)
{
	TargetAnimSource = Cast<UPaperZDAnimationSource>(SelectedAsset.GetAsset());
	PickerWindow->RequestDestroyWindow();
}

#undef LOCTEXT_NAMESPACE
