// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Editors/AnimTimeline/SPaperZDAnimTrackArea.h"
#include "Editors/AnimTimeline/SPaperZDAnimTrack.h"
#include "Editors/AnimTimeline/PaperZDAnimTimeSliderController.h"
#include "Editors/AnimTimeline/PaperZDAnimModel.h"
#include "Widgets/SWeakWidget.h"
#include "Layout/ArrangedChildren.h"
#include "Layout/LayoutUtils.h"

////
//Track area slot
FPaperZDAnimTrackAreaSlot::FPaperZDAnimTrackAreaSlot(const TSharedPtr<SPaperZDAnimTrack>& InSlotContent)
{
	TrackWidget = InSlotContent;

	HAlignment = HAlign_Fill;
	VAlignment = VAlign_Top;

	AttachWidget(
		SNew(SWeakWidget)
		.Clipping(EWidgetClipping::ClipToBounds)
		.PossiblyNullContent(InSlotContent)
	);
}

float FPaperZDAnimTrackAreaSlot::GetVerticalOffset() const
{
	TSharedPtr<SPaperZDAnimTrack> PinnedTrackWidget = TrackWidget.Pin();
	return PinnedTrackWidget.IsValid() ? PinnedTrackWidget->GetPhysicalPosition() : 0.f;
}

////
//Track area
void SPaperZDAnimTrackArea::Construct(const FArguments& InArgs, const TSharedRef<FPaperZDAnimModel>& InModel, const TSharedRef<FPaperZDAnimTimeSliderController>& InTimeSliderController)
{
	WeakModel = InModel;
	WeakTimeSliderController = InTimeSliderController;
}

void SPaperZDAnimTrackArea::SetOutliner(const TSharedPtr<SPaperZDAnimOutliner>& InOutliner)
{
	WeakOutliner = InOutliner;
}

void SPaperZDAnimTrackArea::Empty()
{
	TrackSlots.Empty();
	Children.Empty();
}

void SPaperZDAnimTrackArea::AddTrackSlot(const TSharedRef<FPaperZDAnimTimelineTrack>& InTrack, const TSharedPtr<SPaperZDAnimTrack>& InSlot)
{
	TrackSlots.Add(InTrack, InSlot);
	Children.AddSlot(FPaperZDAnimTrackAreaSlot::FSlotArguments(MakeUnique<FPaperZDAnimTrackAreaSlot>(InSlot)));
	//Children.Add(new FPaperZDAnimTrackAreaSlot(InSlot));
	//Children.Add(MakeUnique<FPaperZDAnimTrackAreaSlot>(InSlot));
	//Children.Add(FPaperZDAnimTrackAreaSlot::FSlotArguments(MakeUnique<FPaperZDAnimTrackAreaSlot>(InSlot)));
}

TSharedPtr<SPaperZDAnimTrack> SPaperZDAnimTrackArea::FindTrackSlot(const TSharedRef<FPaperZDAnimTimelineTrack>& InTrack)
{
	// Remove stale entries
	TrackSlots.Remove(TWeakPtr<FPaperZDAnimTimelineTrack>());

	return TrackSlots.FindRef(InTrack).Pin();
}

void SPaperZDAnimTrackArea::OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const
{
	for (int32 ChildIndex = 0; ChildIndex < Children.Num(); ++ChildIndex)
	{
		const FPaperZDAnimTrackAreaSlot& CurChild = Children[ChildIndex];

		const EVisibility ChildVisibility = CurChild.GetWidget()->GetVisibility();
		if (!ArrangedChildren.Accepts(ChildVisibility))
		{
			continue;
		}

		const FMargin Padding(0, CurChild.GetVerticalOffset(), 0, 0);

		AlignmentArrangeResult XResult = AlignChild<Orient_Horizontal>(AllottedGeometry.GetLocalSize().X, CurChild, Padding, 1.0f, false);
		AlignmentArrangeResult YResult = AlignChild<Orient_Vertical>(AllottedGeometry.GetLocalSize().Y, CurChild, Padding, 1.0f, false);

		ArrangedChildren.AddWidget(ChildVisibility,
			AllottedGeometry.MakeChild(
				CurChild.GetWidget(),
				FVector2D(XResult.Offset, YResult.Offset),
				FVector2D(XResult.Size, YResult.Size)
			)
		);
	}
}

FVector2D SPaperZDAnimTrackArea::ComputeDesiredSize(float) const
{
	FVector2D MaxSize(0.0f, 0.0f);
	for (int32 ChildIndex = 0; ChildIndex < Children.Num(); ++ChildIndex)
	{
		const FPaperZDAnimTrackAreaSlot& CurChild = Children[ChildIndex];

		const EVisibility ChildVisibilty = CurChild.GetWidget()->GetVisibility();
		if (ChildVisibilty != EVisibility::Collapsed)
		{
			FVector2D ChildDesiredSize = CurChild.GetWidget()->GetDesiredSize();
			MaxSize.X = FMath::Max(MaxSize.X, ChildDesiredSize.X);
			MaxSize.Y = FMath::Max(MaxSize.Y, ChildDesiredSize.Y);
		}
	}

	return MaxSize;
}

FChildren* SPaperZDAnimTrackArea::GetChildren()
{
	return &Children;
}

int32 SPaperZDAnimTrackArea::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	// paint the child widgets
	FArrangedChildren ArrangedChildren(EVisibility::Visible);
	ArrangeChildren(AllottedGeometry, ArrangedChildren);

	const FPaintArgs NewArgs = Args.WithNewParent(this);

	for (int32 ChildIndex = 0; ChildIndex < ArrangedChildren.Num(); ++ChildIndex)
	{
		FArrangedWidget& CurWidget = ArrangedChildren[ChildIndex];
		FSlateRect ChildClipRect = MyCullingRect.IntersectionWith(CurWidget.Geometry.GetLayoutBoundingRect());
		const int32 ThisWidgetLayerId = CurWidget.Widget->Paint(NewArgs, CurWidget.Geometry, ChildClipRect, OutDrawElements, LayerId + 2, InWidgetStyle, bParentEnabled);

		LayerId = FMath::Max(LayerId, ThisWidgetLayerId);
	}

	return LayerId;
}
