// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "Editors/AnimTimeline/PaperZDAnimTimelineTrack.h"
//#include "SAnimTimingPanel.h"

class SPaperZDAnimNotifyPanel;
class SPaperZDAnimOutlinerItem;
class FPaperZDAnimModel;
class SInlineEditableTextBlock;
class SVerticalBox;

/** A timeline track that re-uses the legacy panel widget to display notifies */
class FPaperZDAnimTimelineTrack_NotifiesPanel : public FPaperZDAnimTimelineTrack
{
	//ANIMTIMELINE_DECLARE_TRACK(FAnimTimelineTrack_NotifiesPanel, FAnimTimelineTrack);

public:
	static const float NotificationTrackHeight;

	//ctor
	FPaperZDAnimTimelineTrack_NotifiesPanel(const TSharedRef<FPaperZDAnimModel>& InModel);

	/** FAnimTimelineTrack interface */
	virtual TSharedRef<SWidget> GenerateContainerWidgetForTimeline() override;
	virtual TSharedRef<SWidget> GenerateContainerWidgetForOutliner(const TSharedRef<SPaperZDAnimOutlinerItem>& InRow) override;
	virtual bool SupportsFiltering() const override { return false; }

	/* Obtain the legacy notify panel used to render the track. */
	TSharedRef<SPaperZDAnimNotifyPanel> GetAnimNotifyPanel();

	/* Updates the notify panel and tracks. */
	void Update();

	/** Request a rename next update */
	void RequestTrackRename(int32 InTrackIndex) { PendingRenameTrackIndex = InTrackIndex; }

private:
	TSharedRef<SWidget> BuildNotifiesPanelSubMenu(int32 InTrackIndex);
	void InsertTrack(int32 InTrackIndexToInsert);
	void RemoveTrack(int32 InTrackIndexToRemove);
	void RefreshOutlinerWidget();
	void OnCommitTrackName(const FText& InText, ETextCommit::Type CommitInfo, int32 TrackIndexToName);
	EActiveTimerReturnType HandlePendingRenameTimer(double InCurrentTime, float InDeltaTime, TWeakPtr<SInlineEditableTextBlock> InInlineEditableTextBlock);
	void HandleNotifyChanged();

	//EVisibility OnGetTimingNodeVisibility(ETimingElementType::Type ElementType) const;

	void OnSetInputViewRange(float ViewMin, float ViewMax);

	/** The legacy notify panel */
	TSharedPtr<SPaperZDAnimNotifyPanel> AnimNotifyPanel;

	/** The outliner widget to allow for dynamic refresh */
	TSharedPtr<SVerticalBox> OutlinerWidget;

	/** Track index we want to trigger a rename for */
	int32 PendingRenameTrackIndex;
};
