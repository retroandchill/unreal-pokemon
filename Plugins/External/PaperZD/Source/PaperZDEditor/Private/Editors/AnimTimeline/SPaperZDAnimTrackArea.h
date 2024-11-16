// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SPanel.h"
#include "SlotBase.h"
#include "Layout/Children.h"
#include "Layout/BasicLayoutWidgetSlot.h"

class FPaperZDAnimModel;
class FPaperZDAnimTimeSliderController;
class SPaperZDAnimOutliner;
class SPaperZDAnimTrack;
class FPaperZDAnimTimelineTrack;

/**
 * Structure representing a slot in the track area.
 */
class FPaperZDAnimTrackAreaSlot : public TSlotBase<FPaperZDAnimTrackAreaSlot>, public TAlignmentWidgetSlotMixin<FPaperZDAnimTrackAreaSlot>
{
public:

	/** Construction from a track lane */
	FPaperZDAnimTrackAreaSlot(const TSharedPtr<SPaperZDAnimTrack>& InSlotContent);

	/** Get the vertical position of this slot inside its parent. */
	float GetVerticalOffset() const;

	/** Horizontal/Vertical alignment for the slot. */
	EHorizontalAlignment HAlignment;
	EVerticalAlignment VAlignment;

	/** The track that we represent. */
	TWeakPtr<SPaperZDAnimTrack> TrackWidget;
};


/**
 * The area where tracks are displayed.
 */
class SPaperZDAnimTrackArea : public SPanel
{
public:
	SLATE_BEGIN_ARGS(SPaperZDAnimTrackArea)
	{
		_Clipping = EWidgetClipping::ClipToBounds;
	}
	SLATE_END_ARGS()

	//ctor
	SPaperZDAnimTrackArea()
		: Children(this)
	{}

	/* Constructs this widget. */
	void Construct(const FArguments& InArgs, const TSharedRef<FPaperZDAnimModel>& InModel, const TSharedRef<FPaperZDAnimTimeSliderController>& InTimeSliderController);

public:

	/** Empty the track area */
	void Empty();

	/** Add a new track slot to this area for the given node. The slot will be automatically cleaned up when all external references to the supplied slot are removed. */
	void AddTrackSlot(const TSharedRef<FPaperZDAnimTimelineTrack>& InTrack, const TSharedPtr<SPaperZDAnimTrack>& InTrackWidget);

	/** Attempt to find an existing slot relating to the given node */
	TSharedPtr<SPaperZDAnimTrack> FindTrackSlot(const TSharedRef<FPaperZDAnimTimelineTrack>& InTrack);

	/** Access the cached geometry for this track area */
	const FGeometry& GetCachedGeometry() const
	{
		return CachedGeometry;
	}

	/** Assign an outliner to this track area. */
	void SetOutliner(const TSharedPtr<SPaperZDAnimOutliner>& InOutliner);

public:

	//~ Begin SWidget Interface
// 	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
// 	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
// 	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
// 	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;
// 	virtual FReply OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
 	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
// 	virtual FCursorReply OnCursorQuery(const FGeometry& MyGeometry, const FPointerEvent& CursorEvent) const override;
// 	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
 	virtual void OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const override;
 	virtual FVector2D ComputeDesiredSize(float) const override;
 	virtual FChildren* GetChildren() override;
	//~End SWidget Interface

private:
	/** The track area's children. */
	TPanelChildren<FPaperZDAnimTrackAreaSlot> Children;

	/** Cached geometry. */
	FGeometry CachedGeometry;

	/** A map of child slot content that exists in our view. */
	TMap<TWeakPtr<FPaperZDAnimTimelineTrack>, TWeakPtr<SPaperZDAnimTrack>> TrackSlots;

	/** Weak pointer to the model. */
	TWeakPtr<FPaperZDAnimModel> WeakModel;

	/** Weak pointer to the time slider. */
	TWeakPtr<FPaperZDAnimTimeSliderController> WeakTimeSliderController;

	/** Weak pointer to the outliner (used for scrolling interactions). */
	TWeakPtr<SPaperZDAnimOutliner> WeakOutliner;
};
