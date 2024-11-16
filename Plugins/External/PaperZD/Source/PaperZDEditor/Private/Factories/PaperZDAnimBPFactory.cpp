// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "PaperZDAnimBPFactory.h"
#include "PaperZDAnimInstance.h"
#include "PaperZDAnimBPGeneratedClass.h"
#include "AnimSequences/Sources/PaperZDAnimationSource.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Modules/ModuleManager.h"
#include "UObject/UObjectIterator.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SWindow.h"
#include "Widgets/Layout/SBorder.h"
#include "Styling/AppStyle.h"
#include "IContentBrowserSingleton.h"
#include "ContentBrowserModule.h"
#include "Editor.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDAnimBPFactory)
#endif

#define LOCTEXT_NAMESPACE "PaperZDAnimBlueprintFactory"

UPaperZDAnimBPFactory::UPaperZDAnimBPFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UPaperZDAnimBP::StaticClass();
	ParentClass = UPaperZDAnimInstance::StaticClass();
	BlueprintType = EBlueprintType::BPTYPE_Normal;
	SupportedAnimationSource = nullptr;
}

bool UPaperZDAnimBPFactory::ConfigureProperties()
{
	// Optionally select the AnimBP if it hasn't been provided yet
	if (SupportedAnimationSource == nullptr)
	{
		// Load the content browser module to display an asset picker
		FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

		FAssetPickerConfig AssetPickerConfig;

		/** The asset picker will only show AnimBP */
		AssetPickerConfig.Filter.ClassPaths.Add(UPaperZDAnimationSource::StaticClass()->GetClassPathName());
		AssetPickerConfig.Filter.bRecursiveClasses = true;

		/** The delegate that fires when an asset was selected */
		AssetPickerConfig.OnAssetSelected = FOnAssetSelected::CreateUObject(this, &UPaperZDAnimBPFactory::OnTargetAnimSourceSelected);

		/** The default view mode should be a list view */
		AssetPickerConfig.InitialAssetViewType = EAssetViewType::List;

		PickerWindow = SNew(SWindow)
			.Title(LOCTEXT("CreateAnimBPOptions", "Pick Parent Animation Source"))
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
	return SupportedAnimationSource != nullptr;
}

UObject* UPaperZDAnimBPFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	check(SupportedAnimationSource);
	UPaperZDAnimBP* NewBP = CastChecked<UPaperZDAnimBP>(FKismetEditorUtilities::CreateBlueprint(ParentClass, InParent, Name, BlueprintType, UPaperZDAnimBP::StaticClass(), UPaperZDAnimBPGeneratedClass::StaticClass()));
	NewBP->SupportedAnimationSource = SupportedAnimationSource;

	return NewBP;
}

void UPaperZDAnimBPFactory::OnTargetAnimSourceSelected(const FAssetData& SelectedAsset)
{
	SupportedAnimationSource = Cast<UPaperZDAnimationSource>(SelectedAsset.GetAsset());
	PickerWindow->RequestDestroyWindow();
}

#undef LOCTEXT_NAMESPACE