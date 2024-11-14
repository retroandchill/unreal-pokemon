// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "Editors/AnimTimeline/PaperZDAnimTimelineTrack.h"

class FPaperZDAnimTimelineTrack_NotifiesPanel;
class UPaperZDAnimSequence;

/**
 * Creates the "Notifies" column that serves as root for every anim notify on the Anim Outliner.
 */
class FPaperZDAnimTimelineTrack_Notifies : public FPaperZDAnimTimelineTrack
{
	//ANIMTIMELINE_DECLARE_TRACK(FAnimTimelineTrack_Notifies, FAnimTimelineTrack);

public:
	FPaperZDAnimTimelineTrack_Notifies(const TSharedRef<FPaperZDAnimModel>& InModel);

	/** FAnimTimelineTrack interface */
	virtual TSharedRef<SWidget> GenerateContainerWidgetForOutliner(const TSharedRef<SPaperZDAnimOutlinerItem>& InRow) override;

	/** Get a new, unused track name using the specified anim sequence */
	static FName GetNewTrackName(UPaperZDAnimSequence* InAnimSequence);

	/* Sets up the notify panel to use*/
	void SetAnimNotifyPanel(const TSharedRef<FPaperZDAnimTimelineTrack_NotifiesPanel>& InNotifiesPanel) { NotifiesPanel = InNotifiesPanel; }

	/** Controls timing visibility for notify tracks */
	//EVisibility OnGetTimingNodeVisibility(ETimingElementType::Type InType) const;

private:
	/** Populate the track menu */
	TSharedRef<SWidget> BuildNotifiesSubMenu();

	/** Add a new track */
	void AddTrack();

	/** The legacy notifies panel we are linked to */
	TWeakPtr<FPaperZDAnimTimelineTrack_NotifiesPanel> NotifiesPanel;
};
