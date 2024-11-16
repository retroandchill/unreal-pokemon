// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Editors/AnimTimeline/SPaperZDAnimTrack.h"
#include "Editors/AnimTimeline/SPaperZDAnimOutliner.h"
#include "Editors/AnimTimeline/PaperZDAnimTimelineTrack.h"
#include "Widgets/SOverlay.h"
#include "EditorStyleSet.h"

void SPaperZDAnimTrack::Construct(const FArguments& InArgs, const TSharedRef<FPaperZDAnimTimelineTrack>& InTrack, const TSharedRef<SPaperZDAnimOutliner>& InOutliner)
{
	WeakTrack = InTrack;
	WeakOutliner = InOutliner;
	ViewRange = InArgs._ViewRange;

	ChildSlot
		.HAlign(HAlign_Fill)
		.Padding(0)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			[
				InArgs._Content.Widget
			]
		];
}

int32 SPaperZDAnimTrack::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	static const FName BorderName("AnimTimeline.Outliner.DefaultBorder");
	static const FName SelectionColorName("SelectionColor");

	TSharedPtr<FPaperZDAnimTimelineTrack> Track = WeakTrack.Pin();
	if (Track.IsValid())
	{
		if (Track->IsVisible())
		{
			float TotalNodeHeight = Track->GetHeight() + Track->GetPadding().Combined();

			// draw hovered
			if (Track->IsHovered())
			{
				FSlateDrawElement::MakeBox(
					OutDrawElements,
					LayerId++,
					AllottedGeometry.ToPaintGeometry(
						FVector2f(AllottedGeometry.GetLocalSize().X, TotalNodeHeight),
						FSlateLayoutTransform(FVector2f(0, 0))
					),
					FAppStyle::GetBrush(BorderName),
					ESlateDrawEffect::None,
					FLinearColor(1.0f, 1.0f, 1.0f, 0.05f)
				);
			}

			// Draw track bottom border
			FSlateDrawElement::MakeLines(
				OutDrawElements,
				LayerId++,
				AllottedGeometry.ToPaintGeometry(),
				TArray<FVector2D>({ FVector2D(0.0f, TotalNodeHeight), FVector2D(AllottedGeometry.GetLocalSize().X, TotalNodeHeight) }),
				ESlateDrawEffect::None,
				FLinearColor(0.1f, 0.1f, 0.1f, 0.3f)
			);
		}
	}

	return SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId + 1, InWidgetStyle, bParentEnabled);
}

void SPaperZDAnimTrack::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	FVector2D ThisFrameDesiredSize = GetDesiredSize();

	if (LastDesiredSize.IsSet() && ThisFrameDesiredSize.Y != LastDesiredSize.GetValue().Y)
	{
		TSharedPtr<SPaperZDAnimOutliner> Outliner = WeakOutliner.Pin();
		if (Outliner.IsValid())
		{
			Outliner->RequestTreeRefresh();
		}
	}

	LastDesiredSize = ThisFrameDesiredSize;
}

FVector2D SPaperZDAnimTrack::ComputeDesiredSize(float LayoutScale) const
{
	TSharedPtr<FPaperZDAnimTimelineTrack> Track = WeakTrack.Pin();
	if (Track.IsValid())
	{
		return FVector2D(100.f, Track->GetHeight() + Track->GetPadding().Combined());
	}

	return FVector2D::ZeroVector;
}

float SPaperZDAnimTrack::GetPhysicalPosition() const
{
	TSharedPtr<FPaperZDAnimTimelineTrack> Track = WeakTrack.Pin();
	if (Track.IsValid())
	{
		return WeakOutliner.Pin()->ComputeTrackPosition(Track.ToSharedRef()).Get(0.0f);
	}
	return 0.0f;
}

void SPaperZDAnimTrack::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	SCompoundWidget::OnMouseEnter(MyGeometry, MouseEvent);

	TSharedPtr<FPaperZDAnimTimelineTrack> Track = WeakTrack.Pin();
	if (Track.IsValid())
	{
		Track->SetHovered(true);
	}
}

void SPaperZDAnimTrack::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	SCompoundWidget::OnMouseLeave(MouseEvent);

	TSharedPtr<FPaperZDAnimTimelineTrack> Track = WeakTrack.Pin();
	if (Track.IsValid())
	{
		Track->SetHovered(false);
	}
}

FReply SPaperZDAnimTrack::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	TSharedPtr<FPaperZDAnimTimelineTrack> Track = WeakTrack.Pin();
	TSharedPtr<SPaperZDAnimOutliner> Outliner = WeakOutliner.Pin();
	if (Track.IsValid() && Track->SupportsSelection())
	{
		if (MouseEvent.GetModifierKeys().IsControlDown())
		{
			Outliner->SetItemSelection(Track.ToSharedRef(), true, ESelectInfo::OnMouseClick);
		}
		else if (MouseEvent.GetModifierKeys().IsShiftDown())
		{
			Outliner->Private_SelectRangeFromCurrentTo(Track.ToSharedRef());
		}
		else
		{
			if (MouseEvent.GetEffectingButton() != EKeys::RightMouseButton || !Outliner->IsItemSelected(Track.ToSharedRef()))
			{
				Outliner->ClearSelection();
			}
			Outliner->SetItemSelection(Track.ToSharedRef(), true, ESelectInfo::OnMouseClick);
		}

		return FReply::Handled();
	}

	return FReply::Unhandled();
}
