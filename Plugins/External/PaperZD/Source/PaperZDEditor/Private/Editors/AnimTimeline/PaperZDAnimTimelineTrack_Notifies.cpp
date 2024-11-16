// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Editors/AnimTimeline/PaperZDAnimTimelineTrack_Notifies.h"
#include "Editors/AnimTimeline/PaperZDAnimModel_AnimSequence.h"
#include "Editors/AnimTimeline/PaperZDAnimTimelineTrack_NotifiesPanel.h"
#include "Editors/Util/PaperZDEditorCommands.h"
#include "AnimSequences/PaperZDAnimSequence.h"
#include "Styling/AppStyle.h"
#include "Widgets/SBoxPanel.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Widgets/Layout/SBorder.h"
#include "ScopedTransaction.h"

//#include "SAnimNotifyPanel.h"

#define LOCTEXT_NAMESPACE "FPaperZDAnimTimelineTrack_Notifies"

//ANIMTIMELINE_IMPLEMENT_TRACK(FPaperZDAnimTimelineTrack_Notifies);

FPaperZDAnimTimelineTrack_Notifies::FPaperZDAnimTimelineTrack_Notifies(const TSharedRef<FPaperZDAnimModel>& InModel)
	: FPaperZDAnimTimelineTrack(LOCTEXT("NotifiesRootTrackLabel", "Notifies"), LOCTEXT("NotifiesRootTrackToolTip", "Anim notifies"), InModel)
{}

TSharedRef<SWidget> FPaperZDAnimTimelineTrack_Notifies::GenerateContainerWidgetForOutliner(const TSharedRef<SPaperZDAnimOutlinerItem>& InRow)
{
	TSharedPtr<SBorder> OuterBorder;
	TSharedPtr<SHorizontalBox> InnerHorizontalBox;
	TSharedRef<SWidget> OutlinerWidget = GenerateStandardOutlinerWidget(InRow, true, OuterBorder, InnerHorizontalBox);

	OuterBorder->SetBorderBackgroundColor(FAppStyle::GetColor("AnimTimeline.Outliner.HeaderColor"));

	//@TODO: Make sure this doesn't trigger when montages exist
// 	UAnimMontage* AnimMontage = Cast<UAnimMontage>(GetModel()->GetAnimSequenceBase());
// 	if (!(AnimMontage && AnimMontage->HasParentAsset()))
	{
		InnerHorizontalBox->AddSlot()
			.AutoWidth()
			.HAlign(HAlign_Right)
			.VAlign(VAlign_Center)
			.Padding(OutlinerRightPadding, 1.0f)
			[
				MakeTrackButton(LOCTEXT("AddTrackButtonText", "Track"), FOnGetContent::CreateSP(this, &FPaperZDAnimTimelineTrack_Notifies::BuildNotifiesSubMenu), MakeAttributeSP(this, &FPaperZDAnimTimelineTrack_Notifies::IsHovered))
			];
	}

	return OutlinerWidget;
}

TSharedRef<SWidget> FPaperZDAnimTimelineTrack_Notifies::BuildNotifiesSubMenu()
{
	FMenuBuilder MenuBuilder(true, GetModel()->GetCommandList());

	MenuBuilder.BeginSection("Notifies", LOCTEXT("NotifiesMenuSection", "Notifies"));
	{
		MenuBuilder.AddMenuEntry(
			FPaperZDEditorCommands::Get().AddNotifyTrack->GetLabel(),
			FPaperZDEditorCommands::Get().AddNotifyTrack->GetDescription(),
			FPaperZDEditorCommands::Get().AddNotifyTrack->GetIcon(),
			FUIAction(
				FExecuteAction::CreateSP(this, &FPaperZDAnimTimelineTrack_Notifies::AddTrack)
			)
		);
	}
	MenuBuilder.EndSection();

// 	MenuBuilder.BeginSection("TimingPanelOptions", LOCTEXT("TimingPanelOptionsHeader", "Options"));
// 	{
// 		MenuBuilder.AddMenuEntry(
// 			LOCTEXT("ToggleTimingNodes_Notifies", "Show Notify Timing Nodes"),
// 			LOCTEXT("ShowNotifyTimingNodes", "Show or hide the timing display for notifies in the notify panel"),
// 			FSlateIcon(),
// 			FUIAction(
// 				FExecuteAction::CreateSP(&StaticCastSharedRef<FAnimModel_AnimSequenceBase>(GetModel()).Get(), &FAnimModel_AnimSequenceBase::ToggleNotifiesTimingElementDisplayEnabled, ETimingElementType::QueuedNotify),
// 				FCanExecuteAction(),
// 				FIsActionChecked::CreateSP(&StaticCastSharedRef<FAnimModel_AnimSequenceBase>(GetModel()).Get(), &FAnimModel_AnimSequenceBase::IsNotifiesTimingElementDisplayEnabled, ETimingElementType::QueuedNotify)
// 			),
// 			NAME_None,
// 			EUserInterfaceActionType::ToggleButton
// 		);
// 	}
// 	MenuBuilder.EndSection();

	return MenuBuilder.MakeWidget();
}

void FPaperZDAnimTimelineTrack_Notifies::AddTrack()
{
	//Create a transaction for the track
	UPaperZDAnimSequence* AnimSequence = GetModel()->GetAnimAssetBase();
 	FScopedTransaction Transaction(LOCTEXT("AddNotifyTrack", "Add Notify Track"));
	AnimSequence->Modify();

	//Create the track and set it up
	const int32 TrackIdx = AnimSequence->CreateTrack();
	FPaperZDAnimTrackMetadata& TrackData = AnimSequence->GetMutableTrackMetadataChecked(TrackIdx);
	TrackData.DisplayName = GetNewTrackName(AnimSequence);
	TrackData.Color = FLinearColor::White;

	NotifiesPanel.Pin()->RequestTrackRename(TrackIdx);
	NotifiesPanel.Pin()->Update();
}

FName FPaperZDAnimTimelineTrack_Notifies::GetNewTrackName(UPaperZDAnimSequence* InAnimSequenceBase)
{
	TArray<FName> TrackNames;
	TrackNames.Reserve(50);

	for (int32 i = 0; i < InAnimSequenceBase->GetNumTracks(); i++)
	{
		TrackNames.Add(InAnimSequenceBase->GetTrackMetadataChecked(i).DisplayName);
	}

	//Do a name test to get the first available
	FName NameToTest;
	int32 TrackIndex = 1;
	do
	{
		NameToTest = *FString::FromInt(TrackIndex++);
	} while (TrackNames.Contains(NameToTest));

	return NameToTest;
}

#undef LOCTEXT_NAMESPACE
