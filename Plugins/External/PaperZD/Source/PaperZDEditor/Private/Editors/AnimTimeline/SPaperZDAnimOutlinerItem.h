// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Views/STableViewBase.h"
#include "Widgets/Views/STableRow.h"
#include "Widgets/Views/STreeView.h"
#include "Editors/AnimTimeline/PaperZDAnimTimelineTrack.h"

class SPaperZDAnimTrack;

/**
 * Row displayed by the AnimOutliner.
 */
class SPaperZDAnimOutlinerItem : public SMultiColumnTableRow<TSharedRef<FPaperZDAnimTimelineTrack>>
{
public:
	/** Delegate to invoke to create a new column for this row */
	DECLARE_DELEGATE_RetVal_ThreeParams(TSharedRef<SWidget>, FOnGenerateWidgetForColumn, const TSharedRef<FPaperZDAnimTimelineTrack>&, const FName&, const TSharedRef<SPaperZDAnimOutlinerItem>&);

	SLATE_BEGIN_ARGS(SPaperZDAnimOutlinerItem) {}

	/** Delegate to invoke to create a new column for this row */
	SLATE_EVENT(FOnGenerateWidgetForColumn, OnGenerateWidgetForColumn)

	/** Text to highlight when searching */
	SLATE_ATTRIBUTE(FText, HighlightText)

	SLATE_END_ARGS()

	//dtor
	~SPaperZDAnimOutlinerItem();

	/* Constructs this widget. */
	void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView, const TSharedRef<FPaperZDAnimTimelineTrack>& InTrack);

	/** SWidget interface */
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
	virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;
	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;

	/** SMultiColumnTableRow interface */
	virtual TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnId) override;

	/** Get the track this item represents */
	TSharedRef<FPaperZDAnimTimelineTrack> GetTrack() const { return Track.Pin().ToSharedRef(); }

	/** Add a reference to the specified track, keeping it alive until this row is destroyed */
	void AddTrackAreaReference(const TSharedPtr<SPaperZDAnimTrack>& InTrackWidget);

	/** Get the text to highlight when searching */
	TAttribute<FText> GetHighlightText() const { return HighlightText; }

private:
	/** Get the label text for this item */
	FText GetLabelText() const;

	/** Get the hover state for this item */
	bool ShouldAppearHovered() const;

private:
	/** The track that we represent */
	TWeakPtr<FPaperZDAnimTimelineTrack> Track;

	/** Cached reference to a track lane that we relate to. This keeps the track lane alive (it's a weak widget) as long as we are in view. */
	TSharedPtr<SPaperZDAnimTrack> TrackWidget;

	/** Delegate to invoke to create a new column for this row */
	FOnGenerateWidgetForColumn OnGenerateWidgetForColumn;

	/** Text to highlight when searching */
	TAttribute<FText> HighlightText;

	/** Keep an internal IsHovered flag*/
	bool bHovered;
};
