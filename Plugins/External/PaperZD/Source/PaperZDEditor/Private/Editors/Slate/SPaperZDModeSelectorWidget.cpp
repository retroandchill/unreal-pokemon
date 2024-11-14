// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Editors/Slate/SPaperZDModeSelectorWidget.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Input/SComboButton.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Text/STextBlock.h"
#include "Editors/Util/PaperZDEditorStyle.h"
#include "Styling/AppStyle.h"
#include "AnimSequences/Sources/PaperZDAnimationSource.h"
#include "PaperZDAnimBP.h"
#include "Editor.h"
#include "Framework/Application/SlateApplication.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Subsystems/AssetEditorSubsystem.h"
#include "IContentBrowserSingleton.h"
#include "ContentBrowserModule.h"
#include "AssetRegistry/AssetData.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/ARFilter.h"


#define LOCTEXT_NAMESPACE "PaperZD_ModeSelector"

/* Delegate called from the mode switcher, used to setup the default Animation Blueprint that triggered an "open asset" call. */
DECLARE_MULTICAST_DELEGATE_OneParam(FPaperZDOnSetupDefaultAnimationBlueprintSignature, UPaperZDAnimBP*);
FPaperZDOnSetupDefaultAnimationBlueprintSignature OnSetupDefaultAnimationBlueprint;

void SPaperZDModeSelectorWidget::Construct(const FArguments& InArgs, UPaperZDAnimationSource* InAnimSource)
{
	AnimationSource = InAnimSource;
	DefaultAnimBP = InArgs._DefaultAnimBP;
	bSourceSelected = InArgs._bSourceSelected;

	//Setup the static delegate
	OnSetupDefaultAnimationBlueprint.AddSP(this, &SPaperZDModeSelectorWidget::OnDefaultAnimationBlueprintSet);

	//Build the widget
	ChildSlot
	[
		SNew(SHorizontalBox)

		//Blueprint mode
		+SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SCheckBox)
			//.Style(FAppStyle::Get(), "ToolBar.ToggleButton")
			.ForegroundColor(FSlateColor::UseForeground())
			.Padding(0.0f)
			.IsChecked_Lambda([this] () -> ECheckBoxState{ return !bSourceSelected ? ECheckBoxState::Checked : ECheckBoxState::Unchecked; } )
			.ToolTipText(LOCTEXT("BlueprintTooltip", "Edit the blueprint logic and animation graph."))
			.OnCheckStateChanged(this, &SPaperZDModeSelectorWidget::OnCheckboxStateChanged, EPaperZDModeSection::Blueprint)
			[
				SNew(SHorizontalBox)

				+SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.AutoWidth()
				[
					SNew(SBorder)
 					.Padding(4.0f)
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						[
							SNew(SImage)
							.Image(FPaperZDEditorStyle::Get().GetBrush("PaperZDEditor.ModeSwitcher.Blueprint"))
						]
					]
				]

				+SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				.Padding(5.0f)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("BlueprintLabel", "Blueprints"))
					.ShadowOffset(FVector2D::UnitVector)
				]
			]
		]

		+ SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Fill)
		.Padding(2.0f, 0.0f, 0.0f, 0.0f)
		[
			SNew(SComboButton)
			.ContentPadding(0)
			.ForegroundColor(FSlateColor::UseForeground())
			.OnGetMenuContent(this, &SPaperZDModeSelectorWidget::HandleGetMenuContent)
			.ToolTipText(LOCTEXT("BlueprintComboTooltip", "Find other assets of this type and perform asset operations./nShift-Click to open in new window."))
		]
		
		//Separator
		+SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(5.0f, 0.0f)
		.VAlign(VAlign_Center)
		[
			SNew(SImage)
			.Image(FPaperZDEditorStyle::Get().GetBrush("PaperZDEditor.ModeSwitcher.Separator"))
		]

		//Animation Source
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SCheckBox)
			.ForegroundColor(FSlateColor::UseForeground())
			.Padding(0.0f)
			.IsChecked_Lambda([this]() -> ECheckBoxState { return bSourceSelected ? ECheckBoxState::Checked : ECheckBoxState::Unchecked; })
			.ToolTipText(LOCTEXT("AnimationTooltip", "Edit the animations and notifies."))
			.OnCheckStateChanged(this, &SPaperZDModeSelectorWidget::OnCheckboxStateChanged, EPaperZDModeSection::Animation)
			[
				SNew(SHorizontalBox)

				+SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.AutoWidth()
				[
					SNew(SBorder)
 					.Padding(4.0f)
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						[
							SNew(SImage)
							.Image(FPaperZDEditorStyle::Get().GetBrush("PaperZDEditor.ModeSwitcher.Animation"))
						]
					]
				]

				+SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				.Padding(5.0f)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("AnimationLabel", "Animations"))
					.ShadowOffset(FVector2D::UnitVector)
				]
			]
		]
	];
}

SPaperZDModeSelectorWidget::~SPaperZDModeSelectorWidget()
{
	OnSetupDefaultAnimationBlueprint.RemoveAll(this);
}

void SPaperZDModeSelectorWidget::OnCheckboxStateChanged(ECheckBoxState InState, EPaperZDModeSection Section)
{
	if (Section == EPaperZDModeSection::Blueprint)
	{
		//We should check if we have a default AnimBP to trigger
		if (DefaultAnimBP)
		{
			GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(DefaultAnimBP);
		}
		else
		{
			//We should select the first available asset
			FARFilter Filter;
			Filter.bRecursiveClasses = true;
			Filter.ClassPaths.Add(UPaperZDAnimBP::StaticClass()->GetClassPathName());
			Filter.TagsAndValues.Add(UPaperZDAnimBP::GetAnimationSourceMemberName(), FAssetData(AnimationSource).GetExportTextName());

			// Load the asset registry module
			TArray<FAssetData> AssetDataArray;
			FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
			AssetRegistryModule.Get().GetAssets(Filter, AssetDataArray);

			if (AssetDataArray.Num() > 0)
			{
				GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(AssetDataArray[0].GetSoftObjectPath());
			}
		}
	}
	else
	{
		//We only have one possible animation source, open it
		GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(AnimationSource);

		//By this point, the asset editor should be opened and loaded. Instead of casting the editor, we will just relay a delegate call and let it setup its own values
		if (DefaultAnimBP)
		{
			OnSetupDefaultAnimationBlueprint.Broadcast(DefaultAnimBP);
		}
	}
}

void SPaperZDModeSelectorWidget::OnDefaultAnimationBlueprintSet(UPaperZDAnimBP* InDefaultBP)
{
	if (InDefaultBP && bSourceSelected && AnimationSource == InDefaultBP->GetSupportedAnimationSource())
	{
		DefaultAnimBP = InDefaultBP;
	}
}

TSharedRef<SWidget> SPaperZDModeSelectorWidget::HandleGetMenuContent()
{
	const bool bInShouldCloseWindowAfterMenuSelection = true;
	FMenuBuilder MenuBuilder(bInShouldCloseWindowAfterMenuSelection, nullptr);

	MenuBuilder.BeginSection("AssetSelection", LOCTEXT("AssetSelectionSection", "Select Asset"));
	{
		//Create a filter that only shows AnimBPs bound to the animation source
		FARFilter Filter;
 		Filter.bRecursiveClasses = true;
 		Filter.ClassPaths.Add(UPaperZDAnimBP::StaticClass()->GetClassPathName());
 		Filter.TagsAndValues.Add(UPaperZDAnimBP::GetAnimationSourceMemberName(), FAssetData(AnimationSource).GetExportTextName());

		//Create the asset picker
		FAssetPickerConfig AssetPickerConfig;
		AssetPickerConfig.Filter = Filter;
		AssetPickerConfig.SelectionMode = ESelectionMode::SingleToggle;
		AssetPickerConfig.OnAssetSelected = FOnAssetSelected::CreateSP(this, &SPaperZDModeSelectorWidget::HandleAssetSelectedFromPicker);
		AssetPickerConfig.bAllowNullSelection = false;
		AssetPickerConfig.ThumbnailLabel = EThumbnailLabel::ClassName;
		AssetPickerConfig.InitialAssetViewType = EAssetViewType::List;


		//Create the entry
		FContentBrowserModule& ContentBrowserModule = FModuleManager::Get().LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
		MenuBuilder.AddWidget(
			SNew(SBox)
			.WidthOverride(300)
			.HeightOverride(600)
			[
				ContentBrowserModule.Get().CreateAssetPicker(AssetPickerConfig)
			],
		FText(), true);
	}
	MenuBuilder.EndSection();

	return MenuBuilder.MakeWidget();
}

void SPaperZDModeSelectorWidget::HandleAssetSelectedFromPicker(const FAssetData& SelectedAsset)
{
	FSlateApplication::Get().DismissAllMenus();
	GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(SelectedAsset.GetSoftObjectPath());
}

#undef LOCTEXT_NAMESPACE
