// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Editors/AnimTimeline/SPaperZDAnimOutliner.h"
#include "Editors/AnimTimeline/PaperZDAnimModel.h"
#include "Editors/AnimTimeline/PaperZDAnimTimelineTrack.h"
#include "Editors/AnimTimeline/SPaperZDAnimOutlinerItem.h"
#include "Editors/AnimTimeline/SPaperZDAnimTrackArea.h"
#include "Editors/AnimTimeline/SPaperZDAnimTrack.h"
#include "Widgets/Input/SButton.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Misc/TextFilterExpressionEvaluator.h"

#define LOCTEXT_NAMESPACE "SPaperZDAnimOutliner"

SPaperZDAnimOutliner::~SPaperZDAnimOutliner()
{
	if (AnimModel.IsValid())
	{
		AnimModel.Pin()->OnTracksChanged().Remove(TracksChangedDelegateHandle);
	}
}

void SPaperZDAnimOutliner::Construct(const FArguments& InArgs, const TSharedRef<FPaperZDAnimModel>& InAnimModel, const TSharedRef<SPaperZDAnimTrackArea>& InTrackArea)
{
	AnimModel = InAnimModel;
	TrackArea = InTrackArea;
	FilterText = InArgs._FilterText;
	bPhysicalTracksNeedUpdate = false;

	TracksChangedDelegateHandle = InAnimModel->OnTracksChanged().AddSP(this, &SPaperZDAnimOutliner::HandleTracksChanged);

	TextFilter = MakeShareable(new FTextFilterExpressionEvaluator(ETextFilterExpressionEvaluatorMode::BasicString));

	HeaderRow = SNew(SHeaderRow)
		.Visibility(EVisibility::Collapsed);

	HeaderRow->AddColumn(
		SHeaderRow::Column(TEXT("Outliner"))
		.FillWidth(1.0f)
	);

	STreeView::Construct
	(
		STreeView::FArguments()
		.TreeItemsSource(&InAnimModel->GetRootTracks())
		.SelectionMode(ESelectionMode::Multi)
		.OnGenerateRow(this, &SPaperZDAnimOutliner::HandleGenerateRow)
		.OnGetChildren(this, &SPaperZDAnimOutliner::HandleGetChildren)
		.HeaderRow(HeaderRow)
		.ExternalScrollbar(InArgs._ExternalScrollbar)
		.OnExpansionChanged(this, &SPaperZDAnimOutliner::HandleExpansionChanged)
		.AllowOverscroll(EAllowOverscroll::No)
		.OnContextMenuOpening(this, &SPaperZDAnimOutliner::HandleContextMenuOpening)
	);

	// expand all
	for (TSharedRef<FPaperZDAnimTimelineTrack>& RootTrack : InAnimModel->GetRootTracks())
	{
		RootTrack->Traverse_ParentFirst([this](FPaperZDAnimTimelineTrack& InTrack) { SetItemExpansion(InTrack.AsShared(), InTrack.IsExpanded()); return true; });
	}
}

void SPaperZDAnimOutliner::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	STreeView::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	// These are updated in both tick and paint since both calls can cause changes to the cached rows and the data needs
	// to be kept synchronized so that external measuring calls get correct and reliable results.
	if (bPhysicalTracksNeedUpdate)
	{
		PhysicalTracks.Reset();
		CachedTrackGeometry.GenerateValueArray(PhysicalTracks);

		PhysicalTracks.Sort([](const FCachedGeometry& A, const FCachedGeometry& B)
			{
				return A.Top < B.Top;
			});

		bPhysicalTracksNeedUpdate = false;
	}
}

int32 SPaperZDAnimOutliner::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	LayerId = STreeView::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	// These are updated in both tick and paint since both calls can cause changes to the cached rows and the data needs
	// to be kept synchronized so that external measuring calls get correct and reliable results.
	if (bPhysicalTracksNeedUpdate)
	{
		PhysicalTracks.Reset();
		CachedTrackGeometry.GenerateValueArray(PhysicalTracks);

		PhysicalTracks.Sort([](const FCachedGeometry& A, const FCachedGeometry& B)
			{
				return A.Top < B.Top;
			});

		bPhysicalTracksNeedUpdate = false;
	}

	return LayerId + 1;
}

TSharedRef<ITableRow> SPaperZDAnimOutliner::HandleGenerateRow(TSharedRef<FPaperZDAnimTimelineTrack> InTrack, const TSharedRef<STableViewBase>& OwnerTable)
{
	TSharedRef<SPaperZDAnimOutlinerItem> Row =
		SNew(SPaperZDAnimOutlinerItem, OwnerTable, InTrack)
		.OnGenerateWidgetForColumn(this, &SPaperZDAnimOutliner::GenerateWidgetForColumn)
		.HighlightText(FilterText);

	// Ensure the track area is kept up to date with the virtualized scroll of the tree view
	TSharedPtr<SPaperZDAnimTrack> TrackWidget = TrackArea->FindTrackSlot(InTrack);

	if (!TrackWidget.IsValid())
	{
		// Add a track slot for the row
		TrackWidget = SNew(SPaperZDAnimTrack, InTrack, SharedThis(this))
			.ViewRange(AnimModel.Pin().Get(), &FPaperZDAnimModel::GetViewRange)
			[
				InTrack->GenerateContainerWidgetForTimeline()
			];

		TrackArea->AddTrackSlot(InTrack, TrackWidget);
	}

	if (ensure(TrackWidget.IsValid()))
	{
		Row->AddTrackAreaReference(TrackWidget);
	}

	return Row;
}

TSharedRef<SWidget> SPaperZDAnimOutliner::GenerateWidgetForColumn(const TSharedRef<FPaperZDAnimTimelineTrack>& InTrack, const FName& ColumnId, const TSharedRef<SPaperZDAnimOutlinerItem>& Row) const
{
	return InTrack->GenerateContainerWidgetForOutliner(Row);
}

void SPaperZDAnimOutliner::HandleGetChildren(TSharedRef<FPaperZDAnimTimelineTrack> Item, TArray<TSharedRef<FPaperZDAnimTimelineTrack>>& OutChildren)
{
	if (!FilterText.Get().IsEmpty())
	{
		for (const TSharedRef<FPaperZDAnimTimelineTrack>& Child : Item->GetChildren())
		{
			if (!Child->SupportsFiltering() || TextFilter->TestTextFilter(FBasicStringFilterExpressionContext(Child->GetLabel().ToString())))
			{
				OutChildren.Add(Child);
			}
		}
	}
	else
	{
		OutChildren.Append(Item->GetChildren());
	}
}

void SPaperZDAnimOutliner::HandleExpansionChanged(TSharedRef<FPaperZDAnimTimelineTrack> InTrack, bool bIsExpanded)
{
	InTrack->SetExpanded(bIsExpanded);

	// Expand any children that are also expanded
	for (const TSharedRef<FPaperZDAnimTimelineTrack>& Child : InTrack->GetChildren())
	{
		if (Child->IsExpanded())
		{
			SetItemExpansion(Child, true);
		}
	}
}

TSharedPtr<SWidget> SPaperZDAnimOutliner::HandleContextMenuOpening()
{
	const bool bShouldCloseWindowAfterMenuSelection = true;
	FMenuBuilder MenuBuilder(bShouldCloseWindowAfterMenuSelection, AnimModel.Pin()->GetCommandList());

	AnimModel.Pin()->BuildContextMenu(MenuBuilder);

	// > 1 because the search widget is always added
	return MenuBuilder.GetMultiBox()->GetBlocks().Num() > 1 ? MenuBuilder.MakeWidget() : TSharedPtr<SWidget>();
}

void SPaperZDAnimOutliner::HandleTracksChanged()
{
	RequestTreeRefresh();
}

void SPaperZDAnimOutliner::ReportChildRowGeometry(const TSharedRef<FPaperZDAnimTimelineTrack>& InTrack, const FGeometry& InGeometry)
{
	float ChildOffset = TransformPoint(
		Concatenate(
			InGeometry.GetAccumulatedLayoutTransform(),
			GetCachedGeometry().GetAccumulatedLayoutTransform().Inverse()
		),
		FVector2D(0, 0)
	).Y;

	FCachedGeometry* ExistingGeometry = CachedTrackGeometry.Find(InTrack);
	if (ExistingGeometry == nullptr || (ExistingGeometry->Top != ChildOffset || ExistingGeometry->Height != InGeometry.Size.Y))
	{
		CachedTrackGeometry.Add(InTrack, FCachedGeometry(InTrack, ChildOffset, InGeometry.Size.Y));
		bPhysicalTracksNeedUpdate = true;
	}
}

void SPaperZDAnimOutliner::OnChildRowRemoved(const TSharedRef<FPaperZDAnimTimelineTrack>& InTrack)
{
	CachedTrackGeometry.Remove(InTrack);
	bPhysicalTracksNeedUpdate = true;
}

TOptional<SPaperZDAnimOutliner::FCachedGeometry> SPaperZDAnimOutliner::GetCachedGeometryForTrack(const TSharedRef<FPaperZDAnimTimelineTrack>& InTrack) const
{
	if (const FCachedGeometry* FoundGeometry = CachedTrackGeometry.Find(InTrack))
	{
		return *FoundGeometry;
	}

	return TOptional<FCachedGeometry>();
}

TOptional<float> SPaperZDAnimOutliner::ComputeTrackPosition(const TSharedRef<FPaperZDAnimTimelineTrack>& InTrack) const
{
	// Positioning strategy:
	// Attempt to root out any visible track in the specified track's sub-hierarchy, and compute the track's offset from that
	float NegativeOffset = 0.f;
	TOptional<float> Top;

	// Iterate parent first until we find a tree view row we can use for the offset height
	auto Iter = [&](FPaperZDAnimTimelineTrack& InTrack)
	{
		TOptional<FCachedGeometry> ChildRowGeometry = GetCachedGeometryForTrack(InTrack.AsShared());
		if (ChildRowGeometry.IsSet())
		{
			Top = ChildRowGeometry->Top;
			// Stop iterating
			return false;
		}

		NegativeOffset -= InTrack.GetHeight() + InTrack.GetPadding().Combined();
		return true;
	};

	InTrack->TraverseVisible_ParentFirst(Iter);

	if (Top.IsSet())
	{
		return NegativeOffset + Top.GetValue();
	}

	return Top;
}

void SPaperZDAnimOutliner::ScrollByDelta(float DeltaInSlateUnits)
{
	ScrollBy(GetCachedGeometry(), DeltaInSlateUnits, EAllowOverscroll::No);
}

void SPaperZDAnimOutliner::Private_SetItemSelection(TSharedRef<FPaperZDAnimTimelineTrack> TheItem, bool bShouldBeSelected, bool bWasUserDirected)
{
	if (TheItem->SupportsSelection())
	{
		AnimModel.Pin()->SetTrackSelected(TheItem, bShouldBeSelected);
		STreeView::Private_SetItemSelection(TheItem, bShouldBeSelected, bWasUserDirected);
	}
}

void SPaperZDAnimOutliner::Private_ClearSelection()
{
	AnimModel.Pin()->ClearTrackSelection();

	STreeView::Private_ClearSelection();
}

void SPaperZDAnimOutliner::Private_SelectRangeFromCurrentTo(TSharedRef<FPaperZDAnimTimelineTrack> InRangeSelectionEnd)
{
	STreeView::Private_SelectRangeFromCurrentTo(InRangeSelectionEnd);

	for (TSet<TSharedRef<FPaperZDAnimTimelineTrack>>::TIterator Iter = SelectedItems.CreateIterator(); Iter; ++Iter)
	{
		if (!(*Iter)->SupportsSelection())
		{
			Iter.RemoveCurrent();
		}
	}

	for (const TSharedRef<FPaperZDAnimTimelineTrack>& SelectedItem : SelectedItems)
	{
		AnimModel.Pin()->SetTrackSelected(SelectedItem, true);
	}
}

void SPaperZDAnimOutliner::RefreshFilter()
{
	TextFilter->SetFilterText(FilterText.Get());

	RequestTreeRefresh();
}

#undef LOCTEXT_NAMESPACE
