// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Editors/AnimTimeline/PaperZDAnimTimelineTrack_NotifiesPanel.h"
#include "Editors/AnimTimeline/PaperZDAnimTimelineTrack_Notifies.h"
#include "Editors/AnimTimeline/PaperZDAnimModel_AnimSequence.h"
#include "Editors/AnimTimeline/SPaperZDAnimOutlinerItem.h"
#include "Editors/Slate/SPaperZDAnimNotifyPanel.h"
#include "Editors/Util/PaperZDEditorCommands.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"
#include "ScopedTransaction.h"
#include "Widgets/Views/SExpanderArrow.h"
#include "Widgets/Layout/SBorder.h"
#include "Styling/AppStyle.h"

//@NOTE: Remove in favor of montage when its available
#include "AnimSequences/PaperZDAnimSequence.h"

#define LOCTEXT_NAMESPACE "FPaperZDAnimTimelineTrack_NotifiesPanel"

const float FPaperZDAnimTimelineTrack_NotifiesPanel::NotificationTrackHeight = 24.0f;

//ANIMTIMELINE_IMPLEMENT_TRACK(FPaperZDAnimTimelineTrack_NotifiesPanel);

FPaperZDAnimTimelineTrack_NotifiesPanel::FPaperZDAnimTimelineTrack_NotifiesPanel(const TSharedRef<FPaperZDAnimModel>& InModel)
	: FPaperZDAnimTimelineTrack(FText::GetEmpty(), FText::GetEmpty(), InModel)
	, PendingRenameTrackIndex(INDEX_NONE)
{
	SetHeight(InModel->GetAnimAssetBase()->GetNumTracks() * NotificationTrackHeight);
}

TSharedRef<SWidget> FPaperZDAnimTimelineTrack_NotifiesPanel::GenerateContainerWidgetForTimeline()
{
	GetAnimNotifyPanel();
	AnimNotifyPanel->Update();
	return AnimNotifyPanel.ToSharedRef();
}

TSharedRef<SWidget> FPaperZDAnimTimelineTrack_NotifiesPanel::GenerateContainerWidgetForOutliner(const TSharedRef<SPaperZDAnimOutlinerItem>& InRow)
{
	TSharedRef<SWidget> Widget =
		SNew(SHorizontalBox)
		.ToolTipText(this, &FPaperZDAnimTimelineTrack::GetToolTipText)
		+ SHorizontalBox::Slot()
		[
			SAssignNew(OutlinerWidget, SVerticalBox)
		];

	RefreshOutlinerWidget();

	return Widget;
}

void FPaperZDAnimTimelineTrack_NotifiesPanel::RefreshOutlinerWidget()
{
	OutlinerWidget->ClearChildren();

	UPaperZDAnimSequence* AnimSequence = GetModel()->GetAnimAssetBase();
	for (int32 TrackIndex = 0; TrackIndex < AnimSequence->GetNumTracks(); TrackIndex++)
	{
		const FPaperZDAnimTrack& Track = AnimSequence->GetTrackChecked(TrackIndex);
		TSharedPtr<SBox> SlotBox;
		TSharedPtr<SInlineEditableTextBlock> InlineEditableTextBlock;

		OutlinerWidget->AddSlot()
			.AutoHeight()
			[
				SAssignNew(SlotBox, SBox)
				.HeightOverride(NotificationTrackHeight)
			];

		//Horizontal box that shows the name and has the "Track" button
		TSharedPtr<SHorizontalBox> HorizontalBox;
		SlotBox->SetContent(
			SNew(SBorder)
			.BorderImage(FAppStyle::GetBrush("Sequencer.Section.BackgroundTint"))
			.BorderBackgroundColor(FAppStyle::GetColor("AnimTimeline.Outliner.ItemColor"))
			[
				SAssignNew(HorizontalBox, SHorizontalBox)
				+SHorizontalBox::Slot()
				.FillWidth(1.0f)
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Left)
				.Padding(30.0f, 0.0f, 0.0f, 0.0f)
				[
					SAssignNew(InlineEditableTextBlock, SInlineEditableTextBlock)
					.Text_Lambda([TrackIndex, AnimSequence]()
						{ 
							FPaperZDAnimTrackMetadata TrackData;
							return AnimSequence->GetTrackMetadata(TrackIndex, TrackData) ? FText::FromName(TrackData.DisplayName) : FText::GetEmpty();
						}) 
					.IsSelected(FIsSelected::CreateLambda([](){ return true; }))
					.OnTextCommitted(this, &FPaperZDAnimTimelineTrack_NotifiesPanel::OnCommitTrackName, TrackIndex)
				]

			]
		);

		//@TODO: Filter out montages when they are available
// 		UAnimMontage* AnimMontage = Cast<UAnimMontage>(GetModel()->GetAnimSequenceBase());
// 		if(!(AnimMontage && AnimMontage->HasParentAsset()))
		{
			HorizontalBox->AddSlot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				.HAlign(HAlign_Right)
				.Padding(OutlinerRightPadding, 1.0f)
				[
					MakeTrackButton(LOCTEXT("AddTrackButtonText", "Track"), FOnGetContent::CreateSP(this, &FPaperZDAnimTimelineTrack_NotifiesPanel::BuildNotifiesPanelSubMenu, TrackIndex), MakeAttributeSP(SlotBox.Get(), &SWidget::IsHovered))
				];
		}

		if (PendingRenameTrackIndex == TrackIndex)
		{
			TWeakPtr<SInlineEditableTextBlock> WeakInlineEditableTextBlock = InlineEditableTextBlock;
			InlineEditableTextBlock->RegisterActiveTimer(0.0f, FWidgetActiveTimerDelegate::CreateSP(this, &FPaperZDAnimTimelineTrack_NotifiesPanel::HandlePendingRenameTimer, WeakInlineEditableTextBlock));
		}

	}
}

EActiveTimerReturnType FPaperZDAnimTimelineTrack_NotifiesPanel::HandlePendingRenameTimer(double InCurrentTime, float InDeltaTime, TWeakPtr<SInlineEditableTextBlock> InInlineEditableTextBlock)
{
	if (InInlineEditableTextBlock.IsValid())
	{
		InInlineEditableTextBlock.Pin()->EnterEditingMode();
	}

	PendingRenameTrackIndex = INDEX_NONE;

	return EActiveTimerReturnType::Stop;
}

TSharedRef<SWidget> FPaperZDAnimTimelineTrack_NotifiesPanel::BuildNotifiesPanelSubMenu(int32 InTrackIndex)
{
	UPaperZDAnimSequence* AnimSequence = GetModel()->GetAnimAssetBase();

	FMenuBuilder MenuBuilder(true, GetModel()->GetCommandList());
	MenuBuilder.BeginSection("NotifyTrack", LOCTEXT("NotifyTrackMenuSection", "Notify Track"));
	{
		MenuBuilder.AddMenuEntry(
			FPaperZDEditorCommands::Get().InsertNotifyTrack->GetLabel(),
			FPaperZDEditorCommands::Get().InsertNotifyTrack->GetDescription(),
			FPaperZDEditorCommands::Get().InsertNotifyTrack->GetIcon(),
			FUIAction(
				FExecuteAction::CreateSP(this, &FPaperZDAnimTimelineTrack_NotifiesPanel::InsertTrack, InTrackIndex)
			)
		);

		if (AnimSequence->GetNumTracks() > 1)
		{
			MenuBuilder.AddMenuEntry(
				FPaperZDEditorCommands::Get().RemoveNotifyTrack->GetLabel(),
				FPaperZDEditorCommands::Get().RemoveNotifyTrack->GetDescription(),
				FPaperZDEditorCommands::Get().RemoveNotifyTrack->GetIcon(),
				FUIAction(
					FExecuteAction::CreateSP(this, &FPaperZDAnimTimelineTrack_NotifiesPanel::RemoveTrack, InTrackIndex)
				)
			);
		}
	}
	MenuBuilder.EndSection();

	return MenuBuilder.MakeWidget();
}

void FPaperZDAnimTimelineTrack_NotifiesPanel::InsertTrack(int32 InTrackIndexToInsert)
{
	UPaperZDAnimSequence* AnimSequence = GetModel()->GetAnimAssetBase();

	FScopedTransaction Transaction(LOCTEXT("InsertNotifyTrack", "Insert Notify Track"));
	AnimSequence->Modify();

	//Create the track itself and initialize its name
	const int32 TrackIdx = AnimSequence->CreateTrack(InTrackIndexToInsert);
	FPaperZDAnimTrackMetadata& TrackData = AnimSequence->GetMutableTrackMetadataChecked(TrackIdx);
	TrackData.DisplayName = FPaperZDAnimTimelineTrack_Notifies::GetNewTrackName(AnimSequence);
	TrackData.Color = FLinearColor::White;

	// Request a rename on rebuild
	PendingRenameTrackIndex = InTrackIndexToInsert;

	Update();
}

void FPaperZDAnimTimelineTrack_NotifiesPanel::RemoveTrack(int32 InTrackIndexToRemove)
{
	UPaperZDAnimSequence* AnimSequence = GetModel()->GetAnimAssetBase();
	FScopedTransaction Transaction(LOCTEXT("RemoveNotifyTrack", "Remove Notify Track"));
	AnimSequence->Modify();

	AnimSequence->RemoveTrack(InTrackIndexToRemove);
	Update();
}

void FPaperZDAnimTimelineTrack_NotifiesPanel::Update()
{
	SetHeight(GetModel()->GetAnimAssetBase()->GetNumTracks() * NotificationTrackHeight);
	RefreshOutlinerWidget();
	if (AnimNotifyPanel.IsValid())
	{
		AnimNotifyPanel->Update();
	}
}

void FPaperZDAnimTimelineTrack_NotifiesPanel::HandleNotifyChanged()
{
	SetHeight(GetModel()->GetAnimAssetBase()->GetNumTracks() * NotificationTrackHeight);
	RefreshOutlinerWidget();
}

void FPaperZDAnimTimelineTrack_NotifiesPanel::OnCommitTrackName(const FText& InText, ETextCommit::Type CommitInfo, int32 TrackIndexToName)
{
	UPaperZDAnimSequence* AnimSequence = GetModel()->GetAnimAssetBase();

	FScopedTransaction Transaction(FText::Format(LOCTEXT("RenameNotifyTrack", "Rename Notify Track to '{0}'"), InText));
	AnimSequence->Modify();

	//Obtain a mutable reference of the track metadata and modify it
	FText TrimText = FText::TrimPrecedingAndTrailing(InText);
	FPaperZDAnimTrackMetadata& TrackData = AnimSequence->GetMutableTrackMetadataChecked(TrackIndexToName);
	TrackData.DisplayName = FName(*TrimText.ToString());
}

void FPaperZDAnimTimelineTrack_NotifiesPanel::OnSetInputViewRange(float ViewMin, float ViewMax)
{
	GetModel()->SetViewRange(TRange<double>(ViewMin, ViewMax));
}

TSharedRef<SPaperZDAnimNotifyPanel> FPaperZDAnimTimelineTrack_NotifiesPanel::GetAnimNotifyPanel()
{
	if (!AnimNotifyPanel.IsValid())
	{
		AnimNotifyPanel = SNew(SPaperZDAnimNotifyPanel, GetModel())
					.ViewInputMin(this, &FPaperZDAnimTimelineTrack_NotifiesPanel::GetViewMinInput)
		 			.ViewInputMax(this, &FPaperZDAnimTimelineTrack_NotifiesPanel::GetViewMaxInput)
 					.OnSelectionChanged(this, &FPaperZDAnimTimelineTrack_NotifiesPanel::SelectObjects)
 					.OnSetViewRange(this, &FPaperZDAnimTimelineTrack_NotifiesPanel::OnSetInputViewRange)
		 			.OnSnapPosition(&GetModel().Get(), &FPaperZDAnimModel::Snap)
					.OnRefreshSnapTimes(&GetModel().Get(), &FPaperZDAnimModel::RefreshSnapTimes)
		 			.OnInvokeTab(GetModel()->OnInvokeTab);
//		 			.OnGetTimingNodeVisibility(this, &FPaperZDAnimTimelineTrack_NotifiesPanel::OnGetTimingNodeVisibility)
//		 			.OnNotifiesChanged_Lambda([this]()
// 					{
// 						Update();
// 						GetModel()->OnTracksChanged().Broadcast();
//	 				});

		//GetModel()->GetAnimSequenceBase()->RegisterOnNotifyChanged(UAnimSequenceBase::FOnNotifyChanged::CreateSP(this, &FPaperZDAnimTimelineTrack_NotifiesPanel::HandleNotifyChanged));
	}

	return AnimNotifyPanel.ToSharedRef();
}

#undef LOCTEXT_NAMESPACE
