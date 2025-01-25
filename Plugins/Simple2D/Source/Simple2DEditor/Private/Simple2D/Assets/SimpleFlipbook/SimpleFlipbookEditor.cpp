// "Unreal Pokémon" created by Retro & Chill.


#include "Simple2D/Assets/SimpleFlipbook/SimpleFlipbookEditor.h"
#include "Simple2DEditor.h"
#include "Editor/KismetWidgets/Public/SScrubControlPanel.h"
#include "Framework/Commands/GenericCommands.h"
#include "Simple2D/Assets/SimpleFlipbook/SimpleFlipbookEditorCommands.h"
#include "Simple2D/Assets/SimpleFlipbook/Widgets/SimpleFlipbookEditorViewport.h"
#include "Simple2D/Components/SimpleFlipbookComponent.h"
#include "Widgets/SimpleFlipbookTimeline.h"

namespace Simple2D {

    namespace FlipbookTabs {
        const FName DetailsID = TEXT("Details");
        const FName ViewportID = TEXT("Viewport");
    }

    const FName FlipbookEditorAppName = FName(TEXT("FlipbookEditorApp"));

    void FSimpleFlipbookEditor::RegisterTabSpawners(const TSharedRef<FTabManager> &TabManager) {
        WorkspaceMenuCategory = TabManager->AddLocalWorkspaceMenuCategory(NSLOCTEXT("Simple2D", "WorkspaceMenu_FlipbookEditor", "Flipbook Editor"));
        auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

        FAssetEditorToolkit::RegisterTabSpawners(TabManager);

        TabManager->RegisterTabSpawner(FlipbookTabs::ViewportID, FOnSpawnTab::CreateSP(this, &FSimpleFlipbookEditor::SpawnTab_Viewport))
            .SetDisplayName( NSLOCTEXT("Simple2D", "ViewportTab", "Viewport") )
            .SetGroup(WorkspaceMenuCategoryRef)
            .SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Viewports"));

        TabManager->RegisterTabSpawner(FlipbookTabs::DetailsID, FOnSpawnTab::CreateSP(this, &FSimpleFlipbookEditor::SpawnTab_Details))
            .SetDisplayName( NSLOCTEXT("Simple2D", "DetailsTabLabel", "Details") )
            .SetGroup(WorkspaceMenuCategoryRef)
            .SetIcon(FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Details"));
    }

    void FSimpleFlipbookEditor::UnregisterTabSpawners(const TSharedRef<FTabManager> &TabManager) {
        FAssetEditorToolkit::UnregisterTabSpawners(TabManager);

        TabManager->UnregisterTabSpawner(FlipbookTabs::ViewportID);
        TabManager->UnregisterTabSpawner(FlipbookTabs::DetailsID);
    }

    FName FSimpleFlipbookEditor::GetToolkitFName() const {
        return "SimpleFlipbookEditor";
    }

    FText FSimpleFlipbookEditor::GetBaseToolkitName() const {
        return NSLOCTEXT("Simple2D", "SimpleFlipbookEditorAppLabel", "Simple Flipbook Editor");
    }

    FText FSimpleFlipbookEditor::GetToolkitName() const {
	    return FText::FromString(FlipbookBeingEdited->GetName());
    }

    FText FSimpleFlipbookEditor::GetToolkitToolTipText() const {
        return GetToolTipTextForObject(FlipbookBeingEdited);
    }

    FLinearColor FSimpleFlipbookEditor::GetWorldCentricTabColorScale() const {
	    return FLinearColor::White;
    }

    FString FSimpleFlipbookEditor::GetWorldCentricTabPrefix() const {
        return TEXT("SimpleFlipbookEditor");
    }

    void FSimpleFlipbookEditor::AddReferencedObjects(FReferenceCollector &Collector) {
	    Collector.AddReferencedObject(FlipbookBeingEdited);
    }

    void FSimpleFlipbookEditor::InitFlipbookEditor(const EToolkitMode::Type Mode,
                                                   const TSharedPtr<IToolkitHost> &InitToolkitHost, USimpleFlipbook *InitFlipbook) {
        GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->CloseOtherEditors(InitFlipbook, this);
        FlipbookBeingEdited = InitFlipbook;
        CurrentSelectedKeyframe = INDEX_NONE;

        FSimpleFlipbookEditorCommands::Register();

        BindCommands();

        ViewportPtr = SNew(SSimpleFlipbookEditorViewport)
            .FlipbookBeingEdited(this, &FSimpleFlipbookEditor::GetFlipbookBeingEdited);
	
        // Default layout
        const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("Standalone_FlipbookEditor_Layout_v2")
            ->AddArea
            (
                FTabManager::NewPrimaryArea()
                ->SetOrientation(Orient_Vertical)
                ->Split
                (
                    FTabManager::NewSplitter()
                    ->SetOrientation(Orient_Horizontal)
                    ->SetSizeCoefficient(0.9f)
                    ->Split
                    (
                        FTabManager::NewStack()
                        ->SetSizeCoefficient(0.8f)
                        ->SetHideTabWell(true)
                        ->AddTab(FlipbookTabs::ViewportID, ETabState::OpenedTab)
                    )
                    ->Split
                    (
                        FTabManager::NewStack()
                        ->SetSizeCoefficient(0.2f)
                        ->AddTab(FlipbookTabs::DetailsID, ETabState::OpenedTab)
                    )
                )
            );

        // Initialize the asset editor and spawn nothing (dummy layout)
        InitAssetEditor(Mode, InitToolkitHost, FlipbookEditorAppName, StandaloneDefaultLayout, /*bCreateDefaultStandaloneMenu=*/ true, /*bCreateDefaultToolbar=*/ true, InitFlipbook);

        // Extend things
        ExtendMenu();
        ExtendToolbar();
        RegenerateMenusAndToolbars();
    }

    USimpleFlipbookComponent * FSimpleFlipbookEditor::GetPreviewComponent() const {
        auto* PreviewComponent = ViewportPtr->GetPreviewComponent();
        check(PreviewComponent != nullptr);
        return PreviewComponent;
        
    }

    void FSimpleFlipbookEditor::BindCommands() {
        const auto& Commands = FSimpleFlipbookEditorCommands::Get();

        const TSharedRef<FUICommandList>& UICommandList = GetToolkitCommands();

        UICommandList->MapAction(FGenericCommands::Get().Delete,
            FExecuteAction::CreateSP(this, &FSimpleFlipbookEditor::DeleteSelection),
            FCanExecuteAction::CreateSP(this, &FSimpleFlipbookEditor::HasValidSelection));

        UICommandList->MapAction(FGenericCommands::Get().Duplicate,
            FExecuteAction::CreateSP(this, &FSimpleFlipbookEditor::DuplicateSelection),
            FCanExecuteAction::CreateSP(this, &FSimpleFlipbookEditor::HasValidSelection));

        UICommandList->MapAction(Commands.AddNewFrame,
            FExecuteAction::CreateSP(this, &FSimpleFlipbookEditor::AddNewKeyFrameAtEnd),
            FCanExecuteAction());
        UICommandList->MapAction(Commands.AddNewFrameBefore,
            FExecuteAction::CreateSP(this, &FSimpleFlipbookEditor::AddNewKeyFrameBefore),
            FCanExecuteAction());
        UICommandList->MapAction(Commands.AddNewFrameAfter,
            FExecuteAction::CreateSP(this, &FSimpleFlipbookEditor::AddNewKeyFrameAfter),
            FCanExecuteAction());

        UICommandList->MapAction(Commands.AddKeyFrame,
            FExecuteAction::CreateSP(this, &FSimpleFlipbookEditor::AddKeyFrameAtCurrentTime));
    }

    void FSimpleFlipbookEditor::ExtendMenu() {
        // Does nothing
    }

    void FSimpleFlipbookEditor::ExtendToolbar() {
        constexpr auto FillToolbar = [](FToolBarBuilder& ToolbarBuilder) {
            ToolbarBuilder.BeginSection("Command");
            {
                ToolbarBuilder.AddToolBarButton(FSimpleFlipbookEditorCommands::Get().AddKeyFrame);
            }
            ToolbarBuilder.EndSection();
        };

        TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);

        ToolbarExtender->AddToolBarExtension(
            "Asset",
            EExtensionHook::After,
            GetToolkitCommands(),
            FToolBarExtensionDelegate::CreateStatic(FillToolbar)
            );

        AddToolbarExtender(ToolbarExtender);

        auto* Paper2DEditorModule = &FModuleManager::LoadModuleChecked<FSimple2DEditorModule>("Simple2DEditor");
        AddToolbarExtender(Paper2DEditorModule->GetFlipbookEditorToolBarExtensibilityManager()->GetAllExtenders());
    }

    TSharedRef<SDockTab> FSimpleFlipbookEditor::SpawnTab_Viewport(const FSpawnTabArgs &) {
        ViewInputMin = 0.0f;
	    ViewInputMax = GetTotalSequenceLength();
	    LastObservedSequenceLength = ViewInputMax;
            
	    TSharedRef<SWidget> ScrubControl = SNew(SScrubControlPanel)
		    .IsEnabled(true)
		    .Value(this, &FSimpleFlipbookEditor::GetPlaybackPosition)
		    .NumOfKeys(this, &FSimpleFlipbookEditor::GetTotalFrameCountPlusOne)
		    .SequenceLength(this, &FSimpleFlipbookEditor::GetTotalSequenceLength)
		    .OnValueChanged(this, &FSimpleFlipbookEditor::SetPlaybackPosition)
		    .OnClickedForwardPlay(this, &FSimpleFlipbookEditor::OnClick_Forward)
		    .OnClickedForwardStep(this, &FSimpleFlipbookEditor::OnClick_Forward_Step)
		    .OnClickedForwardEnd(this, &FSimpleFlipbookEditor::OnClick_Forward_End)
		    .OnClickedBackwardPlay(this, &FSimpleFlipbookEditor::OnClick_Backward)
		    .OnClickedBackwardStep(this, &FSimpleFlipbookEditor::OnClick_Backward_Step)
		    .OnClickedBackwardEnd(this, &FSimpleFlipbookEditor::OnClick_Backward_End)
		    .OnClickedToggleLoop(this, &FSimpleFlipbookEditor::OnClick_ToggleLoop)
		    .OnGetLooping(this, &FSimpleFlipbookEditor::IsLooping)
		    .OnGetPlaybackMode(this, &FSimpleFlipbookEditor::GetPlaybackMode)
		    .ViewInputMin(this, &FSimpleFlipbookEditor::GetViewRangeMin)
		    .ViewInputMax(this, &FSimpleFlipbookEditor::GetViewRangeMax)
		    .OnSetInputViewRange(this, &FSimpleFlipbookEditor::SetViewRange)
		    .bAllowZoom(true)
		    .IsRealtimeStreamingMode(false);

	    return SNew(SDockTab)
		    .Label(NSLOCTEXT("Simple2D", "ViewportTab_Title", "Viewport"))
		    [
			    SNew(SVerticalBox)
			    
			    +SVerticalBox::Slot()
			    [
				    ViewportPtr.ToSharedRef()
			    ]

			    +SVerticalBox::Slot()
			    .Padding(0, 8, 0, 0)
			    .AutoHeight()
			    [
				    SNew(SSimpleFlipbookTimeline, GetToolkitCommands())
				    .FlipbookBeingEdited(this, &FSimpleFlipbookEditor::GetFlipbookBeingEdited)
				    .OnSelectionChanged(this, &FSimpleFlipbookEditor::SetSelection)
				    .PlayTime(this, &FSimpleFlipbookEditor::GetPlaybackPosition)
			    ]

			    +SVerticalBox::Slot()
			    .Padding(0, 8, 0, 0)
			    .AutoHeight()
			    [
				    ScrubControl
			    ]
		    ];
    }

    TSharedRef<SDockTab> FSimpleFlipbookEditor::SpawnTab_Details(const FSpawnTabArgs &Args) {
        auto FlipbookEditorPtr = SharedThis(this);

        // Spawn the tab
        return SNew(SDockTab)
            .Label(NSLOCTEXT("Simple2D", "DetailsTab_Title", "Details"))
            [
                SNew(SFlipbookPropertiesTabBody, FlipbookEditorPtr.ToSharedPtr())
            ];
    }

    void FSimpleFlipbookEditor::DeleteSelection() {
        
        if (FlipbookBeingEdited->IsValidKeyFrameIndex(CurrentSelectedKeyframe))
        {
            const FScopedTransaction Transaction(NSLOCTEXT("SImple2D", "DeleteKeyframeTransaction", "Delete Keyframe"));
            FlipbookBeingEdited->Modify();

            FScopedSimpleFlipbookMutator EditLock(FlipbookBeingEdited);
            EditLock.KeyFrames.RemoveAt(CurrentSelectedKeyframe);

            CurrentSelectedKeyframe = INDEX_NONE;
        }
    }

    void FSimpleFlipbookEditor::DuplicateSelection() {
        if (!FlipbookBeingEdited->IsValidKeyFrameIndex(CurrentSelectedKeyframe)) {
            return;
        }
        
        const FScopedTransaction Transaction(NSLOCTEXT("Simple2D", "DuplicateKeyframeTransaction", "Duplicate Keyframe"));
        FlipbookBeingEdited->Modify();

        FScopedSimpleFlipbookMutator EditLock(FlipbookBeingEdited);

        auto NewFrame = EditLock.KeyFrames[CurrentSelectedKeyframe];
        EditLock.KeyFrames.Insert(NewFrame, CurrentSelectedKeyframe);

        CurrentSelectedKeyframe = INDEX_NONE;
    }

    void FSimpleFlipbookEditor::SetSelection(int32 NewSelection) {
        CurrentSelectedKeyframe = NewSelection;
    }

    bool FSimpleFlipbookEditor::HasValidSelection() const {
        return FlipbookBeingEdited->IsValidKeyFrameIndex(CurrentSelectedKeyframe);
    }

    void FSimpleFlipbookEditor::AddKeyFrameAtCurrentTime() {
        const FScopedTransaction Transaction(NSLOCTEXT("Simple2D", "InsertKeyFrameTransaction", "Insert Key Frame"));
        FlipbookBeingEdited->Modify();

        const float CurrentTime = GetPlaybackPosition();
        const int32 KeyFrameIndex = FlipbookBeingEdited->GetKeyFrameIndexAtTime(CurrentTime);
        const int32 ClampedIndex = FMath::Clamp<int32>(KeyFrameIndex, 0, FlipbookBeingEdited->GetNumFrames());

        FScopedSimpleFlipbookMutator EditLock(FlipbookBeingEdited);
        FSimpleFlipbookKeyFrame NewFrame;
        EditLock.KeyFrames.Insert(NewFrame, ClampedIndex);
    }

    void FSimpleFlipbookEditor::AddNewKeyFrameAtEnd() {
        const FScopedTransaction Transaction(NSLOCTEXT("Simple2D", "AddKeyFrameTransaction", "Add Key Frame"));
        FlipbookBeingEdited->Modify();

        FScopedSimpleFlipbookMutator EditLock(FlipbookBeingEdited);

        auto& NewFrame = *new (EditLock.KeyFrames) FSimpleFlipbookKeyFrame();
    }

    void FSimpleFlipbookEditor::AddNewKeyFrameBefore() {
        if (FlipbookBeingEdited->IsValidKeyFrameIndex(CurrentSelectedKeyframe))
        {
            const FScopedTransaction Transaction(NSLOCTEXT("Simple2D", "InsertKeyFrameBeforeTransaction", "Insert Key Frame Before"));
            FlipbookBeingEdited->Modify();

            FScopedSimpleFlipbookMutator EditLock(FlipbookBeingEdited);

            FSimpleFlipbookKeyFrame NewFrame;
            EditLock.KeyFrames.Insert(NewFrame, CurrentSelectedKeyframe);

            CurrentSelectedKeyframe = INDEX_NONE;
        }
    }

    void FSimpleFlipbookEditor::AddNewKeyFrameAfter() {
        if (!FlipbookBeingEdited->IsValidKeyFrameIndex(CurrentSelectedKeyframe)) {
            return;
        }
        
        const FScopedTransaction Transaction(NSLOCTEXT("Simple 2D", "InsertKeyFrameAfterTransaction", "Insert Key Frame After"));
        FlipbookBeingEdited->Modify();

        FScopedSimpleFlipbookMutator EditLock(FlipbookBeingEdited);

        FSimpleFlipbookKeyFrame NewFrame;
        EditLock.KeyFrames.Insert(NewFrame, CurrentSelectedKeyframe + 1);

        CurrentSelectedKeyframe = INDEX_NONE;
    }

    FReply FSimpleFlipbookEditor::OnClick_Forward() {
        auto* PreviewComponent = GetPreviewComponent();

        const bool bIsReverse = PreviewComponent->IsReversing();
        const bool bIsPlaying = PreviewComponent->IsPlaying();
		
        if (bIsReverse && bIsPlaying)
        {
            // Play forwards instead of backwards
            PreviewComponent->Play();
        }
        else if (bIsPlaying)
        {
            // Was already playing forwards, so pause
            PreviewComponent->Stop();
        }
        else
        {
            // Was paused, start playing
            PreviewComponent->Play();
        }

        return FReply::Handled();
    }

    FReply FSimpleFlipbookEditor::OnClick_Forward_Step() {
        GetPreviewComponent()->Stop();
        SetCurrentFrame(GetCurrentFrame() + 1);
        return FReply::Handled();
    }

    FReply FSimpleFlipbookEditor::OnClick_Forward_End() {
        auto* PreviewComponent = GetPreviewComponent();
        PreviewComponent->Stop();
        PreviewComponent->SetPlaybackPosition(PreviewComponent->GetFlipbookLength(), false);
        return FReply::Handled();
    }

    FReply FSimpleFlipbookEditor::OnClick_Backward() {
        auto* PreviewComponent = GetPreviewComponent();

        const bool bIsReverse = PreviewComponent->IsReversing();
        const bool bIsPlaying = PreviewComponent->IsPlaying();

        if (bIsReverse && bIsPlaying)
        {
            // Was already playing backwards, so pause
            PreviewComponent->Stop();
        }
        else if (bIsPlaying)
        {
            // Play backwards instead of forwards
            PreviewComponent->Reverse();
        }
        else
        {
            // Was paused, start reversing
            PreviewComponent->Reverse();
        }

        return FReply::Handled();
    }

    FReply FSimpleFlipbookEditor::OnClick_Backward_Step() {
        GetPreviewComponent()->Stop();
        SetCurrentFrame(GetCurrentFrame() - 1);
        return FReply::Handled();
    }

    FReply FSimpleFlipbookEditor::OnClick_Backward_End() {
        auto* PreviewComponent = GetPreviewComponent();
        PreviewComponent->Stop();
        PreviewComponent->SetPlaybackPosition(0.0f, false);
        return FReply::Handled();
    }

    FReply FSimpleFlipbookEditor::OnClick_ToggleLoop() {
        auto PreviewComponent = GetPreviewComponent();
        PreviewComponent->SetLooping(!PreviewComponent->IsLooping());
        return FReply::Handled();
    }

    uint32 FSimpleFlipbookEditor::GetTotalFrameCount() const {
	    return FlipbookBeingEdited->GetNumFrames();
    }

    uint32 FSimpleFlipbookEditor::GetTotalFrameCountPlusOne() const {
        return FlipbookBeingEdited->GetNumFrames() + 1;
    }

    float FSimpleFlipbookEditor::GetTotalSequenceLength() const {
        return FlipbookBeingEdited->GetTotalDuration();
    }

    float FSimpleFlipbookEditor::GetPlaybackPosition() const {
        return GetPreviewComponent()->GetPlaybackPosition();
    }

    void FSimpleFlipbookEditor::SetPlaybackPosition(float NewTime) {
        NewTime = FMath::Clamp<float>(NewTime, 0.0f, GetTotalSequenceLength());
        GetPreviewComponent()->SetPlaybackPosition(NewTime, false);
    }

    bool FSimpleFlipbookEditor::IsLooping() const {
        return GetPreviewComponent()->IsLooping();
    }

    EPlaybackMode::Type FSimpleFlipbookEditor::GetPlaybackMode() const {
        if (auto* PreviewComponent = GetPreviewComponent(); PreviewComponent->IsPlaying())
        {
            return PreviewComponent->IsReversing() ? EPlaybackMode::PlayingReverse : EPlaybackMode::PlayingForward;
        }
        
        return EPlaybackMode::Stopped;
    }

    float FSimpleFlipbookEditor::GetViewRangeMin() const {
        return ViewInputMin;
    }

    float FSimpleFlipbookEditor::GetViewRangeMax() const {
        if (const float SequenceLength = GetTotalSequenceLength(); SequenceLength != LastObservedSequenceLength)
        {
            LastObservedSequenceLength = SequenceLength;
            ViewInputMin = 0.0f;
            ViewInputMax = SequenceLength;
        }

        return ViewInputMax;
    }

    void FSimpleFlipbookEditor::SetViewRange(float NewMin, float NewMax) {
        ViewInputMin = FMath::Max<float>(NewMin, 0.0f);
        ViewInputMax = FMath::Min<float>(NewMax, GetTotalSequenceLength());
    }
}
