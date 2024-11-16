// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Editors/AnimTimeline/PaperZDAnimModel_AnimSequence.h"
#include "Editors/AnimTimeline/PaperZDAnimTimelineTrack_Notifies.h"
#include "Editors/AnimTimeline/PaperZDAnimTimelineTrack_NotifiesPanel.h"
#include "Editors/Util/PaperZDEditorCommands.h"
#include "Editors/Util/PaperZDEditorSettings.h"
#include "AnimSequences/PaperZDAnimSequence.h"
#include "Notifies/PaperZDAnimNotifyState.h"
#include "Framework/Commands/UICommandList.h"
#include "Framework/Commands/GenericCommands.h"
#include "FrameNumberDisplayFormat.h"
#include "PaperFlipbook.h"
#include "Editor.h"

#define LOCTEXT_NAMESPACE "FPaperZDAnimModel_AnimSequence"

FPaperZDAnimModel_AnimSequence::FPaperZDAnimModel_AnimSequence(const TSharedRef<FUICommandList>& InCommandList, UPaperZDAnimSequence* InAnimSequence, UPaperZDAnimationSource* InAnimSource, const TSharedPtr<FPaperZDAnimationSourceViewportClient>& InViewportClient)
	: FPaperZDAnimModel(InCommandList, InAnimSource, InViewportClient)
	, AnimSequence(InAnimSequence)
{
	check(AnimSequence);

 	SnapTypes.Add(FPaperZDAnimModel::FSnapType::Frames.Type, FPaperZDAnimModel::FSnapType::Frames);
 	SnapTypes.Add(FPaperZDAnimModel::FSnapType::Notifies.Type, FPaperZDAnimModel::FSnapType::Notifies);

	UpdateRange();
}

void FPaperZDAnimModel_AnimSequence::UpdateRange()
{
	FAnimatedRange OldPlaybackRange = PlaybackRange;

	// update playback range
	PlaybackRange = FAnimatedRange(0.0,(double)AnimSequence->GetTotalDuration());

	if (OldPlaybackRange != PlaybackRange)
	{
		// Update view/range if playback range changed
		SetViewRange(PlaybackRange);
	}
}

FPaperZDAnimModel_AnimSequence::~FPaperZDAnimModel_AnimSequence()
{
	if (GEditor)
	{
		GEditor->UnregisterForUndo(this);
	}

	FCoreUObjectDelegates::OnObjectPropertyChanged.RemoveAll(this);

// 	AnimSequenceBase->UnregisterOnNotifyChanged(this);
// 	AnimSequenceBase->UnregisterOnAnimTrackCurvesChanged(this);
}

void FPaperZDAnimModel_AnimSequence::Initialize()
{
	//Register property changes
	FCoreUObjectDelegates::OnObjectPropertyChanged.Add(FOnPropertyChangedSignature::CreateSP(this, &FPaperZDAnimModel_AnimSequence::OnPropertyChanged));

	//Register UNDO
	if (GEditor)
	{
		GEditor->RegisterForUndo(this);
	}

	//Create commands
	TSharedRef<FUICommandList> CommandList = WeakCommandList.Pin().ToSharedRef();

	const FPaperZDEditorCommands& Commands = FPaperZDEditorCommands::Get();

	CommandList->MapAction(
		FGenericCommands::Get().Rename,
		FExecuteAction::CreateLambda([this]
			{
				SelectedTracks.Array()[0]->RequestRename();
			}),
		FCanExecuteAction::CreateLambda([this]
			{
				return (SelectedTracks.Num() > 0) && (SelectedTracks.Array()[0]->CanRename());
			})
		);

	CommandList->MapAction(
		Commands.DisplayFrames,
		FExecuteAction::CreateSP(this, &FPaperZDAnimModel_AnimSequence::SetDisplayFormat, EFrameNumberDisplayFormats::Frames),
		FCanExecuteAction(),
		FIsActionChecked::CreateSP(this, &FPaperZDAnimModel_AnimSequence::IsDisplayFormatChecked, EFrameNumberDisplayFormats::Frames));

	CommandList->MapAction(
		Commands.DisplaySeconds,
		FExecuteAction::CreateSP(this, &FPaperZDAnimModel_AnimSequence::SetDisplayFormat, EFrameNumberDisplayFormats::Seconds),
		FCanExecuteAction(),
		FIsActionChecked::CreateSP(this, &FPaperZDAnimModel_AnimSequence::IsDisplayFormatChecked, EFrameNumberDisplayFormats::Seconds));

	CommandList->MapAction(
		Commands.DisplayPercentage,
		FExecuteAction::CreateSP(this, &FPaperZDAnimModel_AnimSequence::ToggleDisplayPercentage),
		FCanExecuteAction(),
		FIsActionChecked::CreateSP(this, &FPaperZDAnimModel_AnimSequence::IsDisplayPercentageChecked));

	CommandList->MapAction(
		Commands.SnapToFrames,
		FExecuteAction::CreateSP(this, &FPaperZDAnimModel_AnimSequence::ToggleSnap, FPaperZDAnimModel::FSnapType::Frames.Type),
		FCanExecuteAction(),
		FIsActionChecked::CreateSP(this, &FPaperZDAnimModel_AnimSequence::IsSnapChecked, FPaperZDAnimModel::FSnapType::Frames.Type),
		FIsActionButtonVisible::CreateSP(this, &FPaperZDAnimModel_AnimSequence::IsSnapAvailable, FPaperZDAnimModel::FSnapType::Frames.Type));

	CommandList->MapAction(
		Commands.SnapToNotifies,
		FExecuteAction::CreateSP(this, &FPaperZDAnimModel_AnimSequence::ToggleSnap, FPaperZDAnimModel::FSnapType::Notifies.Type),
		FCanExecuteAction(),
		FIsActionChecked::CreateSP(this, &FPaperZDAnimModel_AnimSequence::IsSnapChecked, FPaperZDAnimModel::FSnapType::Notifies.Type),
		FIsActionButtonVisible::CreateSP(this, &FPaperZDAnimModel_AnimSequence::IsSnapAvailable, FPaperZDAnimModel::FSnapType::Notifies.Type));
}

void FPaperZDAnimModel_AnimSequence::RefreshTracks()
{
	ClearTrackSelection();

	// Clear all tracks
	RootTracks.Empty();

 	// Add notifies
 	RefreshNotifyTracks();
// 
// 	// Add curves
// 	RefreshCurveTracks();
 
 	// Snaps
 	RefreshSnapTimes();

	// Tell the UI to refresh
	OnTracksChangedDelegate.Broadcast();

	UpdateRange();
}

void FPaperZDAnimModel_AnimSequence::SetDisplayFormat(EFrameNumberDisplayFormats InFormat)
{
	GetMutableDefault<UPaperZDEditorSettings>()->TimelineDisplayFormat = InFormat;
}

bool FPaperZDAnimModel_AnimSequence::IsDisplayFormatChecked(EFrameNumberDisplayFormats InFormat) const
{
	return GetDefault<UPaperZDEditorSettings>()->TimelineDisplayFormat == InFormat;
}

void FPaperZDAnimModel_AnimSequence::ToggleDisplayPercentage()
{
	GetMutableDefault<UPaperZDEditorSettings>()->bTimelineDisplayPercentage = !GetDefault<UPaperZDEditorSettings>()->bTimelineDisplayPercentage;
}

bool FPaperZDAnimModel_AnimSequence::IsDisplayPercentageChecked() const
{
	return GetDefault<UPaperZDEditorSettings>()->bTimelineDisplayPercentage;
}

void FPaperZDAnimModel_AnimSequence::RefreshNotifyTracks()
{
	AnimSequence->InitTracks();
	
	if (!NotifyRoot.IsValid())
	{
		// Add a root track for notifies & then the main 'panel' legacy track
		NotifyRoot = MakeShared<FPaperZDAnimTimelineTrack_Notifies>(SharedThis(this));
	}

	NotifyRoot->ClearChildren();
	RootTracks.Add(NotifyRoot.ToSharedRef());
 
	//Legacy panel which we use to render the tracks
	if (!NotifyPanel.IsValid())
	{
		NotifyPanel = MakeShared<FPaperZDAnimTimelineTrack_NotifiesPanel>(SharedThis(this));
		NotifyRoot->SetAnimNotifyPanel(NotifyPanel.ToSharedRef());
	}

	NotifyRoot->AddChild(NotifyPanel.ToSharedRef());
}

void FPaperZDAnimModel_AnimSequence::RefreshSnapTimes()
{
	SnapTimes.Reset();
	for (const UPaperZDAnimNotify_Base* Notify : AnimSequence->GetAnimNotifies())
	{
		SnapTimes.Emplace(FPaperZDAnimModel::FSnapType::Notifies.Type, (double)Notify->Time);
		if (const UPaperZDAnimNotifyState* StateNotify = Cast<const UPaperZDAnimNotifyState>(Notify))
		{
			SnapTimes.Emplace(FPaperZDAnimModel::FSnapType::Notifies.Type, (double)(StateNotify->Time + StateNotify->Duration));
		}
	}
}

void FPaperZDAnimModel_AnimSequence::PostUndo(bool bSuccess)
{
	UpdateRange();
}

void FPaperZDAnimModel_AnimSequence::PostRedo(bool bSuccess)
{
	UpdateRange();
}

void FPaperZDAnimModel_AnimSequence::OnPropertyChanged(UObject* ChangedObject, FPropertyChangedEvent& PropertyEvent)
{
	if (ChangedObject == AnimSequence)
	{
		UpdateRange();
	}
	else if (ChangedObject->GetClass() == UPaperFlipbook::StaticClass())
	{
		//An edge case, in which the user modified the flipbook itself, we just update range
		UpdateRange();
	}
}

#undef LOCTEXT_NAMESPACE