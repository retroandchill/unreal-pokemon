// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Editors/AnimTimeline/PaperZDAnimTimelineTrack.h"
#include "Widgets/Views/STreeView.h"

class FPaperZDAnimModel;
class SPaperZDAnimTrackArea;
class SPaperZDAnimOutlinerItem;
class FTextFilterExpressionEvaluator;

/** A delegate that is executed when adding menu content. */
DECLARE_DELEGATE_OneParam(FOnGetContextMenuContentSignature, FMenuBuilder& /*MenuBuilder*/);

/**
 * Tree view that displays the AnimTimeline tracks in an ordered manner.
 */
class SPaperZDAnimOutliner : public STreeView<TSharedRef<FPaperZDAnimTimelineTrack>>
{
public:
	SLATE_BEGIN_ARGS(SPaperZDAnimOutliner)
	{}
	/** Externally supplied scroll bar */
	SLATE_ARGUMENT(TSharedPtr<SScrollBar>, ExternalScrollbar)

	/** Called to populate the context menu. */
	SLATE_EVENT(FOnGetContextMenuContentSignature, OnGetContextMenuContent)

	/** The filter test used for searching */
	SLATE_ATTRIBUTE(FText, FilterText)
	SLATE_END_ARGS()

	//dtor
	~SPaperZDAnimOutliner();

	/* Constructs this widget. */
	void Construct(const FArguments& InArgs, const TSharedRef<FPaperZDAnimModel>& InAnimModel, const TSharedRef<SPaperZDAnimTrackArea>& InTrackArea);

	//~ Begin SWidget Interface
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime);
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const;
	//~ End SWidget Interface

	//~ Begin STreeView Interface
	virtual void Private_SetItemSelection(TSharedRef<FPaperZDAnimTimelineTrack> TheItem, bool bShouldBeSelected, bool bWasUserDirected = false) override;
	virtual void Private_ClearSelection() override;
	virtual void Private_SelectRangeFromCurrentTo(TSharedRef<FPaperZDAnimTimelineTrack> InRangeSelectionEnd) override;
	//~ End STreeView Interface

	/** Structure used to cache physical geometry for a particular track */
	struct FCachedGeometry
	{
		FCachedGeometry(TSharedRef<FPaperZDAnimTimelineTrack> InTrack, float InTop, float InHeight)
			: Track(MoveTemp(InTrack))
			, Top(InTop)
			, Height(InHeight)
		{}

		TSharedRef<FPaperZDAnimTimelineTrack> Track;
		float Top;
		float Height;
	};

	/** Get the cached geometry for the specified track */
	TOptional<FCachedGeometry> GetCachedGeometryForTrack(const TSharedRef<FPaperZDAnimTimelineTrack>& InTrack) const;

	/** Compute the position of the specified track using its cached geometry */
	TOptional<float> ComputeTrackPosition(const TSharedRef<FPaperZDAnimTimelineTrack>& InTrack) const;

	/** Report geometry for a child row */
	void ReportChildRowGeometry(const TSharedRef<FPaperZDAnimTimelineTrack>& InTrack, const FGeometry& InGeometry);

	/** Called when a child row widget has been added/removed */
	void OnChildRowRemoved(const TSharedRef<FPaperZDAnimTimelineTrack>& InTrack);

	/** Scroll this tree view by the specified number of slate units */
	void ScrollByDelta(float DeltaInSlateUnits);

	/** Apply any changed filter */
	void RefreshFilter();

private:
	/** Generate a row for the outliner */
	TSharedRef<ITableRow> HandleGenerateRow(TSharedRef<FPaperZDAnimTimelineTrack> Item, const TSharedRef<STableViewBase>& OwnerTable);

	/** Get the children of an outliner item */
	void HandleGetChildren(TSharedRef<FPaperZDAnimTimelineTrack> Item, TArray<TSharedRef<FPaperZDAnimTimelineTrack>>& OutChildren);

	/** Record the expansion state when it changes */
	void HandleExpansionChanged(TSharedRef<FPaperZDAnimTimelineTrack> InItem, bool bIsExpanded);

	/** Handle context menu */
	TSharedPtr<SWidget> HandleContextMenuOpening();

	/** Handle tracks changing */
	void HandleTracksChanged();

	/** Generate a widget for the specified column */
	TSharedRef<SWidget> GenerateWidgetForColumn(const TSharedRef<FPaperZDAnimTimelineTrack>& InTrack, const FName& ColumnId, const TSharedRef<SPaperZDAnimOutlinerItem>& Row) const;

private:
	/** The anim timeline model */
	TWeakPtr<FPaperZDAnimModel> AnimModel;

	/** The track area */
	TSharedPtr<SPaperZDAnimTrackArea> TrackArea;

	/** The header row */
	TSharedPtr<SHeaderRow> HeaderRow;

	/** Delegate handle for track changes */
	FDelegateHandle TracksChangedDelegateHandle;

	/** Map of cached geometries for visible tracks */
	TMap<TSharedRef<FPaperZDAnimTimelineTrack>, FCachedGeometry> CachedTrackGeometry;

	/** Linear, sorted array of tracks that we currently have generated widgets for */
	mutable TArray<FCachedGeometry> PhysicalTracks;

	/** A flag indicating that the physical tracks need to be updated. */
	mutable bool bPhysicalTracksNeedUpdate;

	/** The filter text used when we are searching the tree */
	TAttribute<FText> FilterText;

	/** Compiled filter search terms. */
	TSharedPtr<FTextFilterExpressionEvaluator> TextFilter;

};
