// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Editors/AnimTimeline/SPaperZDAnimOutlinerItem.h"
#include "Editors/AnimTimeline/PaperZDAnimTimelineTrack.h"
#include "Editors/AnimTimeline/SPaperZDAnimOutliner.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/SOverlay.h"

SPaperZDAnimOutlinerItem::~SPaperZDAnimOutlinerItem()
{
	TSharedPtr<SPaperZDAnimOutliner> Outliner = StaticCastSharedPtr<SPaperZDAnimOutliner>(OwnerTablePtr.Pin());
	TSharedPtr<FPaperZDAnimTimelineTrack> PinnedTrack = Track.Pin();
	if (Outliner.IsValid() && PinnedTrack.IsValid())
	{
		Outliner->OnChildRowRemoved(PinnedTrack.ToSharedRef());
	}
}

void SPaperZDAnimOutlinerItem::Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView, const TSharedRef<FPaperZDAnimTimelineTrack>& InTrack)
{
	Track = InTrack;
	OnGenerateWidgetForColumn = InArgs._OnGenerateWidgetForColumn;
	HighlightText = InArgs._HighlightText;

	bHovered = false;
	SetHover(TAttribute<bool>::CreateSP(this, &SPaperZDAnimOutlinerItem::ShouldAppearHovered));

	SMultiColumnTableRow::Construct(
		SMultiColumnTableRow::FArguments()
		.ShowSelection(true),
		InOwnerTableView);
}

TSharedRef<SWidget> SPaperZDAnimOutlinerItem::GenerateWidgetForColumn(const FName& ColumnId)
{
	TSharedPtr<FPaperZDAnimTimelineTrack> PinnedTrack = Track.Pin();
	if (PinnedTrack.IsValid())
	{
		TSharedPtr<SWidget> ColumnWidget = SNullWidget::NullWidget;
		if (OnGenerateWidgetForColumn.IsBound())
		{
			ColumnWidget = OnGenerateWidgetForColumn.Execute(PinnedTrack.ToSharedRef(), ColumnId, SharedThis(this));
		}

		return SNew(SOverlay)
			+ SOverlay::Slot()
			[
				ColumnWidget.ToSharedRef()
			];
	}

	return SNullWidget::NullWidget;
}

void SPaperZDAnimOutlinerItem::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	StaticCastSharedPtr<SPaperZDAnimOutliner>(OwnerTablePtr.Pin())->ReportChildRowGeometry(Track.Pin().ToSharedRef(), AllottedGeometry);
}

FVector2D SPaperZDAnimOutlinerItem::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	TSharedPtr<FPaperZDAnimTimelineTrack> PinnedTrack = Track.Pin();
	if (PinnedTrack.IsValid())
	{
		return FVector2D(100.0f, PinnedTrack->GetHeight() + PinnedTrack->GetPadding().Combined());
	}

	return FVector2D(100.0f, 16.0f);
}

void SPaperZDAnimOutlinerItem::AddTrackAreaReference(const TSharedPtr<SPaperZDAnimTrack>& InTrackWidget)
{
	TrackWidget = InTrackWidget;
}

void SPaperZDAnimOutlinerItem::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	SMultiColumnTableRow<TSharedRef<FPaperZDAnimTimelineTrack>>::OnMouseEnter(MyGeometry, MouseEvent);

	TSharedPtr<FPaperZDAnimTimelineTrack> PinnedTrack = Track.Pin();
	if (PinnedTrack.IsValid())
	{
		PinnedTrack->SetHovered(true);
	}
}

void SPaperZDAnimOutlinerItem::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	SMultiColumnTableRow<TSharedRef<FPaperZDAnimTimelineTrack>>::OnMouseLeave(MouseEvent);

	TSharedPtr<FPaperZDAnimTimelineTrack> PinnedTrack = Track.Pin();
	if (PinnedTrack.IsValid())
	{
		PinnedTrack->SetHovered(false);
	}
}

bool SPaperZDAnimOutlinerItem::ShouldAppearHovered() const
{
	if (TSharedPtr<FPaperZDAnimTimelineTrack> PinnedTrack = Track.Pin())
	{
		return bHovered || PinnedTrack->IsHovered();
	}

	return bHovered;
}
