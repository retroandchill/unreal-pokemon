// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Editors/AnimTimeline/SPaperZDAnimTimelineSplitterOverlay.h"
#include "Widgets/Layout/SBox.h"

void SPaperZDAnimTimelineSplitterOverlay::Construct(const FArguments& InArgs)
{
	SetVisibility(EVisibility::SelfHitTestInvisible);

	Splitter = SArgumentNew(InArgs, SSplitter);
	Splitter->SetVisibility(EVisibility::HitTestInvisible);
	AddSlot()
		[
			Splitter.ToSharedRef()
		];

	for (int32 Index = 0; Index < Splitter->GetChildren()->Num() - 1; ++Index)
	{
		AddSlot()
			.Padding(TAttribute<FMargin>::Create(TAttribute<FMargin>::FGetter::CreateSP(this, &SPaperZDAnimTimelineSplitterOverlay::GetSplitterHandlePadding, Index)))
			[
				SNew(SBox)
				.Visibility(EVisibility::Visible)
			];
	}
}

void SPaperZDAnimTimelineSplitterOverlay::OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const
{
	FArrangedChildren SplitterChildren(ArrangedChildren.GetFilter());
	Splitter->ArrangeChildren(AllottedGeometry, SplitterChildren);

	SlotPadding.Reset();

	for (int32 Index = 0; Index < SplitterChildren.Num() - 1; ++Index)
	{
		const auto& ThisGeometry = SplitterChildren[Index].Geometry;
		const auto& NextGeometry = SplitterChildren[Index + 1].Geometry;

		if (Splitter->GetOrientation() == EOrientation::Orient_Horizontal)
		{
			SlotPadding.Add(FMargin(
				ThisGeometry.Position.X + ThisGeometry.GetLocalSize().X,
				0,
				AllottedGeometry.Size.X - NextGeometry.Position.X,
				0)
			);
		}
		else
		{
			SlotPadding.Add(FMargin(
				0,
				ThisGeometry.Position.Y + ThisGeometry.GetLocalSize().Y,
				0,
				AllottedGeometry.Size.Y - NextGeometry.Position.Y)
			);
		}
	}

	SOverlay::OnArrangeChildren(AllottedGeometry, ArrangedChildren);
}

FMargin SPaperZDAnimTimelineSplitterOverlay::GetSplitterHandlePadding(int32 Index) const
{
	if (SlotPadding.IsValidIndex(Index))
	{
		return SlotPadding[Index];
	}

	return 0.f;
}

FCursorReply SPaperZDAnimTimelineSplitterOverlay::OnCursorQuery(const FGeometry& MyGeometry, const FPointerEvent& CursorEvent) const
{
	return Splitter->OnCursorQuery(MyGeometry, CursorEvent);
}

FReply SPaperZDAnimTimelineSplitterOverlay::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FReply Reply = Splitter->OnMouseButtonDown(MyGeometry, MouseEvent);
	if (Reply.GetMouseCaptor().IsValid())
	{
		// Set us to be the mouse captor so we can forward events properly
		Reply.CaptureMouse(SharedThis(this));
		SetVisibility(EVisibility::Visible);
	}
	return Reply;
}

void SPaperZDAnimTimelineSplitterOverlay::OnMouseCaptureLost(const FCaptureLostEvent& CaptureLostEvent)
{
	SetVisibility(EVisibility::SelfHitTestInvisible);
	SOverlay::OnMouseCaptureLost(CaptureLostEvent);
}

FReply SPaperZDAnimTimelineSplitterOverlay::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FReply Reply = Splitter->OnMouseButtonUp(MyGeometry, MouseEvent);
	if (Reply.ShouldReleaseMouse())
	{
		SetVisibility(EVisibility::SelfHitTestInvisible);
	}
	return Reply;
}

FReply SPaperZDAnimTimelineSplitterOverlay::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	return Splitter->OnMouseMove(MyGeometry, MouseEvent);
}

void SPaperZDAnimTimelineSplitterOverlay::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	return Splitter->OnMouseLeave(MouseEvent);
}

