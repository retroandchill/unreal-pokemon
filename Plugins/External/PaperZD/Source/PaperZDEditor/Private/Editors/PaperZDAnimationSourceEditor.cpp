// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Editors/PaperZDAnimationSourceEditor.h"
#include "Editors/Util/PaperZDAnimationModeTabSpawners.h"
#include "Editors/Slate/SPaperZDAnimAssetBrowser.h"
#include "Editors/Slate/SPaperZDAnimationSourceViewport.h"
#include "Editors/Slate/SPaperZDAnimSequenceEditor.h"
#include "Editors/Slate/SPaperZDModeSelectorWidget.h"
#include "Editors/PaperZDAnimationSourceViewportClient.h"
#include "WorkflowOrientedApp/ApplicationMode.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"
#include "AnimSequences/Sources/PaperZDAnimationSource.h"
#include "AnimSequences/PaperZDAnimSequence.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "Widgets/Docking/SDockTab.h"

//static vars
const FName AnimationSourceEditorAppIdentifier = FName(TEXT("PaperZDAnimationSourceEditorApp"));

const FName PaperZDAnimationSourceModes::AnimationMode(TEXT("PaperZDAnimationSourceMode"));

const FName PaperZDAnimationSourceTabs::SourceDetailsTab(TEXT("SourceDetailsTab"));
const FName PaperZDAnimationSourceTabs::AssetBrowserTab(TEXT("AssetBrowser"));
const FName PaperZDAnimationSourceTabs::AssetDetailsTab(TEXT("AssetDetails"));
const FName PaperZDAnimationSourceTabs::ViewportTab(TEXT("Viewport"));
const FName PaperZDAnimationSourceTabs::DocumentTab(TEXT("Document"));
const FName PaperZDAnimationSourceTabs::DetailsViewTab(TEXT("DetailsView"));
const FName PaperZDAnimationSourceTabs::SourceNotifiesTab(TEXT("SourceNotifies"));

#define LOCTEXT_NAMESPACE "PaperZDAnimationSourceEditor"

//////////////////////////////////////////////////////////////////////////
/// Animation mode
//////////////////////////////////////////////////////////////////////////
class FPaperZDAnimationSourceMode : public FApplicationMode
{
	/* The hosting app */
	TWeakPtr<FPaperZDAnimationSourceEditor> AnimSourceEditorPtr;

	/* Animation source being edited. */
	UPaperZDAnimationSource* AnimSource;

	/** The tab factories we support */
	FWorkflowAllowedTabSet TabFactories;

public:
	//ctor
	FPaperZDAnimationSourceMode(TSharedRef<FPaperZDAnimationSourceEditor> InAnimationSourceEditor, UPaperZDAnimationSource* InAnimSource)
		: FApplicationMode(PaperZDAnimationSourceModes::AnimationMode, GetLocalizedMode)
		, AnimSourceEditorPtr(InAnimationSourceEditor)
		, AnimSource(InAnimSource)
	{
		//Register the tab factories
		TabFactories.RegisterFactory(MakeShareable(new FPaperZDSourceDetailsTabSummoner(InAnimationSourceEditor)));
		TabFactories.RegisterFactory(MakeShareable(new FPaperZDAssetBrowserTabSummoner(InAnimationSourceEditor,
													FPaperZDEditorDelegates::OnOpenAssetSignature::CreateSP(InAnimationSourceEditor, &FPaperZDAnimationSourceEditor::OpenAsset),
													FPaperZDEditorDelegates::OnAssetBrowserCreatedSignature::CreateSP(InAnimationSourceEditor, &FPaperZDAnimationSourceEditor::HandleAssetBrowserCreated)
													)));
		TabFactories.RegisterFactory(MakeShareable(new FPaperZDAssetDetailsTabSummoner(InAnimationSourceEditor,
													FPaperZDEditorDelegates::OnGetAssetSignature::CreateSP(InAnimationSourceEditor, &FPaperZDAnimationSourceEditor::HandleGetSelectedAnimationAsset)
													)));
		TabFactories.RegisterFactory(MakeShareable(new FPaperZDAnimationViewportTabSummoner(InAnimationSourceEditor,
													FPaperZDEditorDelegates::OnAnimationSourceViewportCreatedSignature::CreateSP(InAnimationSourceEditor, &FPaperZDAnimationSourceEditor::HandleViewportCreated)
													)));
		TabFactories.RegisterFactory(MakeShareable(new FPaperZDDetailsViewTabSummoner(InAnimationSourceEditor,
													FPaperZDEditorDelegates::OnDetailsViewCreatedSignature::CreateSP(InAnimationSourceEditor, &FPaperZDAnimationSourceEditor::HandleDetailsCreated)
													)));

		TabFactories.RegisterFactory(MakeShareable(new FPaperZDSourceNotifiesTabSummoner(InAnimationSourceEditor)));


		//View configuration
		TabLayout = FTabManager::NewLayout("PaperZD_AnimationSourceEditor_Layout_v0.0")
			->AddArea
			(
				FTabManager::NewPrimaryArea()
				->SetOrientation(Orient_Vertical)
				->Split
				(
					FTabManager::NewSplitter()
					->SetSizeCoefficient(0.9f)
					->SetOrientation(Orient_Horizontal)
					->Split
					(
						//Left area ~ Source and sequences
						FTabManager::NewSplitter()
						->SetSizeCoefficient(0.2f)
						->SetOrientation(Orient_Vertical)
						->Split
						(
							FTabManager::NewStack()
							->SetSizeCoefficient(0.6f)
							->AddTab(PaperZDAnimationSourceTabs::AssetBrowserTab, ETabState::OpenedTab)
						)
						->Split
						(
							FTabManager::NewStack()
							->SetSizeCoefficient(0.4f)
							->AddTab(PaperZDAnimationSourceTabs::SourceDetailsTab, ETabState::OpenedTab)
							->AddTab(PaperZDAnimationSourceTabs::SourceNotifiesTab, ETabState::ClosedTab)
						)
					)
					->Split
					(
						//Center area ~ Document
						FTabManager::NewSplitter()
						->SetSizeCoefficient(0.6f)
						->SetOrientation(Orient_Vertical)
						->Split
						(
							FTabManager::NewStack()
							->SetSizeCoefficient(0.6f)
							->SetHideTabWell(true)
							->AddTab(PaperZDAnimationSourceTabs::ViewportTab, ETabState::OpenedTab)
						)
						->Split
						(
							FTabManager::NewStack()
							->SetSizeCoefficient(0.4f)
							->SetHideTabWell(false)
							->AddTab(PaperZDAnimationSourceTabs::DocumentTab, ETabState::ClosedTab)
						)
					)
					->Split
					(
						//Right area ~ Details
						FTabManager::NewSplitter()
						->SetSizeCoefficient(0.2f)
						->SetOrientation(Orient_Vertical)
						->Split
						(
							FTabManager::NewStack()
							->SetSizeCoefficient(0.6f)
							->AddTab(PaperZDAnimationSourceTabs::AssetDetailsTab, ETabState::OpenedTab)
						)
						->Split
						(
							FTabManager::NewStack()
							->SetSizeCoefficient(0.4f)
							->AddTab(PaperZDAnimationSourceTabs::DetailsViewTab, ETabState::OpenedTab)
						)
					)
				)
			);
	}

	static FText GetLocalizedMode(const FName InMode)
	{
		return NSLOCTEXT("PaperZDEditor", "AnimationSourceMode", "Animation");
	}

	virtual void RegisterTabFactories(TSharedPtr<FTabManager> InTabManager) override
	{
		TSharedPtr<FPaperZDAnimationSourceEditor> Editor = AnimSourceEditorPtr.Pin();
		Editor->RegisterTabSpawners(InTabManager.ToSharedRef());
		Editor->PushTabFactories(TabFactories);

		FApplicationMode::RegisterTabFactories(InTabManager);
	}
};

//////////////////////////////////////////////////////////////////////////
/// Animation source editor
//////////////////////////////////////////////////////////////////////////
FPaperZDAnimationSourceEditor::FPaperZDAnimationSourceEditor()
{
	IAssetRegistry::Get()->OnAssetRemoved().AddRaw(this, &FPaperZDAnimationSourceEditor::HandleAssetRemoved);
}

FPaperZDAnimationSourceEditor::~FPaperZDAnimationSourceEditor()
{
	if (AnimSource->IsValidLowLevel())
	{
		AnimSource->GetOnAnimationSourcePropertyChange().RemoveAll(this);
	}
	
	//Unregister from the registry
	IAssetRegistry::Get()->OnAssetRemoved().RemoveAll(this);
}

UObject* FPaperZDAnimationSourceEditor::HandleGetAnimationSourceAsset()
{
	return AnimSource;
}

UObject* FPaperZDAnimationSourceEditor::HandleGetSelectedAnimationAsset()
{
	return SelectedAnimAsset.Get();
}

void FPaperZDAnimationSourceEditor::HandleObjectsSelected(const TArray<UObject*>& InObjects)
{
	if (DetailsView.IsValid())
	{
		DetailsView->SetObjects(InObjects);
	}
}

void FPaperZDAnimationSourceEditor::HandleAssetRemoved(const FAssetData& RemovedAssetData)
{
	if (RemovedAssetData == AnimAssetData && SharedAnimDocumentTab.IsValid())
	{
		SharedAnimDocumentTab.Pin()->RequestCloseTab();
		
		//Potentially remove from the viewport too
		if (ViewportClientPtr.IsValid())
		{
			ViewportClientPtr.Pin()->SetAnimSequence(nullptr);
		}
	}
}

void FPaperZDAnimationSourceEditor::RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
}

void FPaperZDAnimationSourceEditor::UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);
}

FName FPaperZDAnimationSourceEditor::GetToolkitFName() const
{
	return FName("PaperZDAnimationSourceEditor");
}

FText FPaperZDAnimationSourceEditor::GetBaseToolkitName() const
{
	return LOCTEXT("AppLabel", "AnimationEditor");
}

FString FPaperZDAnimationSourceEditor::GetWorldCentricTabPrefix() const
{
	return LOCTEXT("WorldCentricTabPrefix", "AnimationEditor ").ToString();
}

FLinearColor FPaperZDAnimationSourceEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0.3f, 0.2f, 0.5f, 0.5f);
}

void FPaperZDAnimationSourceEditor::InitAnimationSourceEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UPaperZDAnimationSource* InAnimationSource)
{
	AnimSource = InAnimationSource;

	//Common initialization
	const bool bCreateDefaultStandaloneMenu = true;
	const bool bCreateDefaultToolbar = true;
	FAssetEditorToolkit::InitAssetEditor(Mode, InitToolkitHost, AnimationSourceEditorAppIdentifier, FTabManager::FLayout::NullLayout, bCreateDefaultStandaloneMenu, bCreateDefaultToolbar, AnimSource);

	//Setup and add the mode
	AddApplicationMode(
		PaperZDAnimationSourceModes::AnimationMode,
		MakeShareable(new FPaperZDAnimationSourceMode(SharedThis(this), AnimSource)));

	SetCurrentMode(PaperZDAnimationSourceModes::AnimationMode);

	//Setup toolbar and menus
	ExtendToolbar();
	RegenerateMenusAndToolbars();

	//Bind to property changes event on the source, so the viewport can react to any important change
	AnimSource->GetOnAnimationSourcePropertyChange().AddSP(this, &FPaperZDAnimationSourceEditor::OnAnimationSourcePropertyChange);
}

void FPaperZDAnimationSourceEditor::OpenAsset(const FAssetData& AssetData)
{
	UPaperZDAnimSequence* AnimAsset = Cast<UPaperZDAnimSequence>(AssetData.GetAsset());
	if (AnimAsset)
	{		
		//Set the new asset
		SelectedAnimAsset = AnimAsset;
		AnimAssetData = AssetData;
	
		//Let every supporting tab know about the change
		AssetBrowserPtr.Pin()->SelectAsset(AssetData);
		ViewportClientPtr.Pin()->SetAnimSequence(SelectedAnimAsset.Get());

		//Create a document tab for this object, depending on its type (currently only AnimSequence supported)
		TSharedRef<SWidget> TabBody = CreateEditorWidgetForAnimAsset(AnimAsset);

		//Logic for binding TAttribute getter
		struct Local
		{
			static FText GetObjectName(UObject* Object)
			{
				return FText::FromString(Object->GetName());
			}
		};
		TAttribute<FText> NameAttribute = TAttribute<FText>::Create(TAttribute<FText>::FGetter::CreateStatic(&Local::GetObjectName, Cast<UObject>(AnimAsset)));

		//Use the already created document tab if it exist
		if (SharedAnimDocumentTab.IsValid())
		{
			TSharedPtr<SDockTab> AnimDocumentTab = SharedAnimDocumentTab.Pin();
			AnimDocumentTab->SetContent(TabBody);
			AnimDocumentTab->ActivateInParent(ETabActivationCause::SetDirectly);
			AnimDocumentTab->SetLabel(NameAttribute);
			//AnimDocumentTab->SetLeftContent(IDocumentation::Get()->CreateAnchor(DocumentLink)); //@TODO: when documentation is ready
		}
		else
		{
			//Need to create the document tab
			TSharedPtr<SDockTab> AnimDocumentTab = SNew(SDockTab)
				.Label(NameAttribute)
				.TabRole(ETabRole::DocumentTab)
				.TabColorScale(GetTabColorScale())
				//@TODO: Remove curves when closing the tab, when they exist
				[
					TabBody
				];

			TabManager->InsertNewDocumentTab(PaperZDAnimationSourceTabs::DocumentTab, FTabManager::ESearchPreference::RequireClosedTab, AnimDocumentTab.ToSharedRef());
			SharedAnimDocumentTab = AnimDocumentTab;
			//OpenedTab->SetLeftContent(IDocumentation::Get()->CreateAnchor(DocumentLink)); //@TODO: when documentation is ready
		}
	}	
}

void FPaperZDAnimationSourceEditor::HandleAssetBrowserCreated(const TSharedRef<SPaperZDAnimAssetBrowser>& AssetBrowser)
{
	AssetBrowserPtr = AssetBrowser;
}

void FPaperZDAnimationSourceEditor::HandleViewportCreated(const TSharedRef<SPaperZDAnimationSourceViewport>& AnimationViewport)
{
	ViewportClientPtr = AnimationViewport->GetEditorViewportClient();
}

void FPaperZDAnimationSourceEditor::HandleDetailsCreated(const TSharedRef<class IDetailsView>& InDetailsView)
{
	DetailsView = InDetailsView;
}

void FPaperZDAnimationSourceEditor::ExtendToolbar()
{
	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
	ToolbarExtender->AddToolBarExtension(
		"Asset",
		EExtensionHook::After,
		NULL,
		FToolBarExtensionDelegate::CreateLambda([this](FToolBarBuilder& ToolbarBuilder)
			{
				AddToolbarWidget(SNew(SPaperZDModeSelectorWidget, AnimSource));
			}));

	//Add the extender
	AddToolbarExtender(ToolbarExtender);
}

void FPaperZDAnimationSourceEditor::OnAnimationSourcePropertyChange()
{
	if (ViewportClientPtr.IsValid())
	{
		ViewportClientPtr.Pin()->RefreshAnimationSource();
	}
}

void FPaperZDAnimationSourceEditor::InvokeTab(const FTabId& TabId)
{
	if (TabManager.IsValid())
	{
		TabManager->TryInvokeTab(TabId);
	}
}

TSharedRef<SWidget> FPaperZDAnimationSourceEditor::CreateEditorWidgetForAnimAsset(UPaperZDAnimSequence* AnimAsset)
{	
	return SNew(SPaperZDAnimSequenceEditor, SharedThis(this))
			.AnimSequence(AnimAsset)
			.OnObjectsSelected(FPaperZDEditorDelegates::OnObjectsSelectedSignature::CreateSP(SharedThis(this), &FPaperZDAnimationSourceEditor::HandleObjectsSelected))
			.OnInvokeTab(FPaperZDEditorDelegates::OnInvokeTabSignature::CreateSP(SharedThis(this), &FPaperZDAnimationSourceEditor::InvokeTab));
}

#undef LOCTEXT_NAMESPACE
