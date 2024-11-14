// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Editors/Util/PaperZDAnimationModeTabSpawners.h"
#include "Editors/Slate/SPaperZDAnimAssetBrowser.h"
#include "Editors/Slate/SPaperZDAnimationSourceViewport.h"
#include "Editors/Slate/SPaperZDAnimationSourceNotifies.h"
#include "Editors/PaperZDAnimationSourceViewportClient.h"
#include "Editors/PaperZDAnimationSourceEditor.h" 
#include "AnimSequences/Sources/PaperZDAnimationSource.h"
#include "SSingleObjectDetailsPanel.h"
#include "PropertyEditorModule.h"
#include "Styling/AppStyle.h"

#define LOCTEXT_NAMESPACE "PaperZDAnimationTabs"

//////////////////////////////////////////////////////////////////////////
//// Source Details
//////////////////////////////////////////////////////////////////////////

/* Widget for showing the details of a single object. */
class SPaperZDAnimAssetDetails : public SSingleObjectDetailsPanel
{
	/* Getter to the asset we want to inspect. */
	FPaperZDEditorDelegates::OnGetAssetSignature OnGetAsset;

public:
	SLATE_BEGIN_ARGS(SPaperZDAnimAssetDetails)
	{}
	SLATE_ARGUMENT(FPaperZDEditorDelegates::OnGetAssetSignature, OnGetAsset)
	SLATE_END_ARGS()

	/* Constructor. */
	void Construct(const FArguments& InArgs)
	{
		OnGetAsset = InArgs._OnGetAsset;

		//Call "parent"
		SSingleObjectDetailsPanel::Construct(SSingleObjectDetailsPanel::FArguments());
	}

protected:
	//~ Begin SSingleObjectDetailsPanel Interface
	virtual UObject* GetObjectToObserve() const
	{
		return OnGetAsset.IsBound() ? OnGetAsset.Execute() : nullptr;
	}

	virtual EVisibility GetAssetDisplayNameVisibility() const
	{
		return (GetObjectToObserve() != NULL) ? EVisibility::Visible : EVisibility::Collapsed;
	}

	virtual FText GetAssetDisplayName() const
	{
		if (UObject* Object = GetObjectToObserve())
		{
			return FText::FromString(Object->GetName());
		}
		else
		{
			return FText::GetEmpty();
		}
	}
	//~ End SSingleObjectDetailsPanel Interface
};

FPaperZDSourceDetailsTabSummoner::FPaperZDSourceDetailsTabSummoner(TSharedPtr<FPaperZDAnimationSourceEditor> InHostingApp)
	: FWorkflowTabFactory(PaperZDAnimationSourceTabs::SourceDetailsTab, InHostingApp)
	, HostingApp(InHostingApp)
{
	TabLabel = LOCTEXT("TabSumonner_Label_AnimSourceDetails", "Animation Source");
	//TabIcon = FSlateIcon(FAppStyle::GetStyleSetName(), "Persona.Tabs.AnimAssetDetails");

	bIsSingleton = true;

	ViewMenuDescription = LOCTEXT("TabSumonner_MenuTitle_AnimSourceDetails", "AnimationSource Details");
	ViewMenuTooltip = LOCTEXT("TabSumonner_MenuTooltip_AnimSourceDetails", "Shows the animation source properties.");
}

TSharedRef<SWidget> FPaperZDSourceDetailsTabSummoner::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	return SNew(SPaperZDAnimAssetDetails)
			.OnGetAsset(FPaperZDEditorDelegates::OnGetAssetSignature::CreateSP(HostingApp.Pin().ToSharedRef(), &FPaperZDAnimationSourceEditor::HandleGetAnimationSourceAsset));
}

FText FPaperZDSourceDetailsTabSummoner::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return LOCTEXT("TabSumonner_Tooltip_AnimSourceDetails", "Shows the details of the root 'Animation Source' object.");
}

//////////////////////////////////////////////////////////////////////////
//// Asset browser
//////////////////////////////////////////////////////////////////////////
FPaperZDAssetBrowserTabSummoner::FPaperZDAssetBrowserTabSummoner(TSharedPtr<FPaperZDAnimationSourceEditor> InHostingApp, FPaperZDEditorDelegates::OnOpenAssetSignature InOnOpenAsset, FPaperZDEditorDelegates::OnAssetBrowserCreatedSignature InOnAssetBrowserCreated)
	: FWorkflowTabFactory(PaperZDAnimationSourceTabs::AssetBrowserTab, InHostingApp)
	, HostingApp(InHostingApp)
	, OnOpenAsset(InOnOpenAsset)
	, OnAssetBrowserCreated(InOnAssetBrowserCreated)
{
	TabLabel = LOCTEXT("TabSumonner_Label_AssetBrowser", "Asset Browser");
	//TabIcon = FSlateIcon(FAppStyle::GetStyleSetName(), "Persona.Tabs.AnimAssetDetails");

	bIsSingleton = true;

	ViewMenuDescription = LOCTEXT("TabSumonner_MenuTitle_AssetBrowser", "Asset Browser");
	ViewMenuTooltip = LOCTEXT("TabSumonner_MenuTooltip_AssetBrowser", "Shows a list of assets related to the animation source.");
}

TSharedRef<SWidget> FPaperZDAssetBrowserTabSummoner::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedRef<SPaperZDAnimAssetBrowser> NewBrowser = SNew(SPaperZDAnimAssetBrowser, HostingApp.Pin())
		.OnOpenAsset(OnOpenAsset);

	OnAssetBrowserCreated.ExecuteIfBound(NewBrowser);
	return NewBrowser;
}

FText FPaperZDAssetBrowserTabSummoner::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return LOCTEXT("TabSumonner_Tooltip_AssetBrowser", "Shows a list of assets related to the animation source.");
}

//////////////////////////////////////////////////////////////////////////
//// Asset details
//////////////////////////////////////////////////////////////////////////
FPaperZDAssetDetailsTabSummoner::FPaperZDAssetDetailsTabSummoner(TSharedPtr<FPaperZDAnimationSourceEditor> InHostingApp, FPaperZDEditorDelegates::OnGetAssetSignature InOnGetAsset)
	: FWorkflowTabFactory(PaperZDAnimationSourceTabs::AssetDetailsTab, InHostingApp)
	, HostingApp(InHostingApp)
	, OnGetAsset(InOnGetAsset)
{
	TabLabel = LOCTEXT("TabSumonner_Label_AssetDetails", "Asset Details");
	//TabIcon = FSlateIcon(FAppStyle::GetStyleSetName(), "Persona.Tabs.AnimAssetDetails");

	bIsSingleton = true;

	ViewMenuDescription = LOCTEXT("TabSumonner_MenuTitle_AssetDetails", "Asset Details");
	ViewMenuTooltip = LOCTEXT("TabSumonner_MenuTooltip_AssetDetails", "Show the properties of the selected animation asset.");
}

TSharedRef<SWidget> FPaperZDAssetDetailsTabSummoner::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	return SNew(SPaperZDAnimAssetDetails)
		.OnGetAsset(OnGetAsset);
}

FText FPaperZDAssetDetailsTabSummoner::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return LOCTEXT("TabSumonner_Tooltip_AssetDetails", "Show the properties of the selected animation asset.");
}

//////////////////////////////////////////////////////////////////////////
//// Viewport tab
//////////////////////////////////////////////////////////////////////////
FPaperZDAnimationViewportTabSummoner::FPaperZDAnimationViewportTabSummoner(TSharedPtr<FPaperZDAnimationSourceEditor> InHostingApp, FPaperZDEditorDelegates::OnAnimationSourceViewportCreatedSignature InOnViewportCreated)
	: FWorkflowTabFactory(PaperZDAnimationSourceTabs::ViewportTab, InHostingApp)
	, HostingApp(InHostingApp)
	, OnViewportCreated(InOnViewportCreated)
{
	TabLabel = LOCTEXT("TabSumonner_Label_Viewport", "Viewport");
	//TabIcon = FSlateIcon(FAppStyle::GetStyleSetName(), "Persona.Tabs.AnimAssetDetails");

	bIsSingleton = true;

	ViewMenuDescription = LOCTEXT("TabSumonner_MenuTitle_Viewport", "Viewport");
	ViewMenuTooltip = LOCTEXT("TabSumonner_MenuTooltip_Viewport", "Show the animation viewport.");
}

TSharedRef<SWidget> FPaperZDAnimationViewportTabSummoner::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedRef<SPaperZDAnimationSourceViewport> NewViewport = SNew(SPaperZDAnimationSourceViewport, *HostingApp.Pin()->GetAnimationSourceBeingEdited());
	OnViewportCreated.ExecuteIfBound(NewViewport);

	return NewViewport;
}

FText FPaperZDAnimationViewportTabSummoner::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return LOCTEXT("TabSumonner_Tooltip_Viewport", "Show the animation viewport.");
}

//////////////////////////////////////////////////////////////////////////
//// Details view tab
//////////////////////////////////////////////////////////////////////////

/* Simple details view widget, supports multiple elements. */
class SPaperZDSimpleDetails : public SCompoundWidget
{
	/* Details view object created. */
	TSharedPtr<class IDetailsView> DetailsView;

public:
	SLATE_BEGIN_ARGS(SPaperZDSimpleDetails)
	{}
	SLATE_EVENT(FPaperZDEditorDelegates::OnDetailsViewCreatedSignature, OnDetailsViewCreated)
	SLATE_END_ARGS()

	/* Construct the widget. */
	void Construct(const FArguments& InArgs)
	{
		FDetailsViewArgs DetailsViewArgs;
		DetailsViewArgs.bUpdatesFromSelection = false;
		DetailsViewArgs.bLockable = false;
		DetailsViewArgs.bAllowSearch = true;
		DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
		DetailsViewArgs.bHideSelectionTip = true;


		FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);

		//Build the view
		ChildSlot
		[
			DetailsView.ToSharedRef()
		];
		
		//Call the delegate if it exists
		InArgs._OnDetailsViewCreated.ExecuteIfBound(DetailsView.ToSharedRef());
	}

	/* Obtain the details view. */
	TSharedPtr<IDetailsView> GetDetailsView() const { return DetailsView; }
};

FPaperZDDetailsViewTabSummoner::FPaperZDDetailsViewTabSummoner(TSharedPtr<FPaperZDAnimationSourceEditor> InHostingApp, FPaperZDEditorDelegates::OnDetailsViewCreatedSignature InOnDetailsViewCreated)
	: FWorkflowTabFactory(PaperZDAnimationSourceTabs::DetailsViewTab, InHostingApp)
	, HostingApp(InHostingApp)
	, OnDetailsViewCreated(InOnDetailsViewCreated)
{
	TabLabel = LOCTEXT("TabSumonner_Label_DetailsView", "Details");
	TabIcon = FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Details");

	bIsSingleton = true;

	ViewMenuDescription = LOCTEXT("TabSumonner_MenuTitle_DetailsView", "Details");
	ViewMenuTooltip = LOCTEXT("TabSumonner_MenuTooltip_DetailsView", "Show the details view.");
}

TSharedRef<SWidget> FPaperZDDetailsViewTabSummoner::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	return SNew(SPaperZDSimpleDetails)
			.OnDetailsViewCreated(OnDetailsViewCreated);
}

FText FPaperZDDetailsViewTabSummoner::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return LOCTEXT("TabSumonner_Tooltip_DetailsView", "Show the details view.");
}

//////////////////////////////////////////////////////////////////////////
//// AnimNotifies tab
//////////////////////////////////////////////////////////////////////////
FPaperZDSourceNotifiesTabSummoner::FPaperZDSourceNotifiesTabSummoner(TSharedPtr<FPaperZDAnimationSourceEditor> InHostingApp)
	: FWorkflowTabFactory(PaperZDAnimationSourceTabs::SourceNotifiesTab, InHostingApp)
	, HostingApp(InHostingApp)
{
	TabLabel = LOCTEXT("TabSumonner_Label_SourceNotifies", "Source Notifies");
	//TabIcon = FSlateIcon(FAppStyle::GetStyleSetName(), "Persona.Tabs.AnimAssetDetails");

	bIsSingleton = true;

	ViewMenuDescription = LOCTEXT("TabSumonner_MenuTitle_SourceNotifies", "Source Notifies");
	ViewMenuTooltip = LOCTEXT("TabSumonner_MenuTooltip_SourceNotifies", "Show the custom animation source notifies.");
}

TSharedRef<SWidget> FPaperZDSourceNotifiesTabSummoner::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	return SNew(SPaperZDAnimationSourceNotifies, HostingApp.Pin()->GetAnimationSourceBeingEdited(), HostingApp.Pin());
}

FText FPaperZDSourceNotifiesTabSummoner::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return LOCTEXT("TabSumonner_Tooltip_SourceNotifies", "Show the custom animation source notifies.");
}

#undef LOCTEXT_NAMESPACE
