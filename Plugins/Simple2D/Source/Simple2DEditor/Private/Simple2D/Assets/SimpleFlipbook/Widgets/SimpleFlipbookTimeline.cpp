// "Unreal Pokémon" created by Retro & Chill.


#include "Simple2D/Assets/SimpleFlipbook/Widgets/SimpleFlipbookTimeline.h"
#include "Simple2D/Assets/SimpleFlipbook.h"
#include "SlateOptMacros.h"
#include "Simple2D/Assets/SimpleFlipbook/SimpleFlipbookEditorCommands.h"
#include "Simple2D/Assets/SimpleFlipbook/Widgets/SimpleFlipbookTimelineHeader.h"
#include "Simple2D/Assets/SimpleFlipbook/Widgets/SimpleFlipbookTimelineTrack.h"
#include "Widgets/Colors/SColorBlock.h"
#include <array>

namespace Simple2D {

    void SSimpleFlipbookTimeline::Construct(const FArguments &InArgs, TSharedPtr<FUICommandList> InCommandList) {
        FlipbookBeingEdited = InArgs._FlipbookBeingEdited;
	    PlayTime = InArgs._PlayTime;
	    OnSelectionChanged = InArgs._OnSelectionChanged;
	    CommandList = InCommandList;

	    SlateUnitsPerFrame = 120.0f;

	    BackgroundPerFrameSlices = SNew(SHorizontalBox);

	    TimelineHeader = SNew(SSimpleFlipbookTimelineHeader)
		    .SlateUnitsPerFrame(this, &SSimpleFlipbookTimeline::GetSlateUnitsPerFrame)
		    .FlipbookBeingEdited(FlipbookBeingEdited)
		    .PlayTime(PlayTime);

	    TimelineTrack = SNew(SSimpleFlipbookTimelineTrack, CommandList)
		    .SlateUnitsPerFrame(this, &SSimpleFlipbookTimeline::GetSlateUnitsPerFrame)
		    .FlipbookBeingEdited(FlipbookBeingEdited)
		    .OnSelectionChanged(OnSelectionChanged);

	    ChildSlot
	    [
		    SNew(SBorder)
		    .BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
		    [
			    SNew(SScrollBox)
			    .Orientation(Orient_Horizontal)
			    .ScrollBarAlwaysVisible(true)
			    .OnUserScrolled(this, &SSimpleFlipbookTimeline::AnimationScrollBar_OnUserScrolled)
			    +SScrollBox::Slot()
			    [
				    SNew(SOverlay)

				    // Per-frame background
				    +SOverlay::Slot()
				    .VAlign(VAlign_Fill)
				    [
					    BackgroundPerFrameSlices.ToSharedRef()
				    ]

				    // Flipbook header and track
				    +SOverlay::Slot()
				    [
					    SNew(SVerticalBox)
		    
					    +SVerticalBox::Slot()
					    .AutoHeight()
					    .Padding(0,0,0,2)
					    [
						    TimelineHeader.ToSharedRef()
					    ]

					    +SVerticalBox::Slot()
					    .AutoHeight()
					    [
						    SNew(SBox)
						    .HeightOverride(Flipbooks::FrameHeight)
						    [
							    TimelineTrack.ToSharedRef()
						    ]
					    ]
				    ]

				    // Empty flipbook instructions
				    + SOverlay::Slot()
				    .VAlign(VAlign_Center)
				    .HAlign(HAlign_Center)
				    [
					    SNew(STextBlock)
					    .Visibility(this, &SSimpleFlipbookTimeline::NoFramesWarningVisibility)
					    .Text(NSLOCTEXT("Simple2D", "EmptyTimelineInstruction", "Right-click here or drop in sprites to add key frames"))
				    ]
			    ]
		    ]
	    ];

	    auto* Flipbook = FlipbookBeingEdited.Get();
	    NumKeyFramesFromLastRebuild = Flipbook != nullptr ? Flipbook->GetNumKeyFrames() : 0;
	    NumFramesFromLastRebuild = Flipbook != nullptr ? Flipbook->GetNumFrames() : 0;
	    RebuildPerFrameBG();
    }

    void SSimpleFlipbookTimeline::OnDragEnter(const FGeometry &MyGeometry, const FDragDropEvent &DragDropEvent) {
        SCompoundWidget::OnDragEnter(MyGeometry, DragDropEvent);

        auto Operation = DragDropEvent.GetOperation();
        if (!Operation.IsValid())
        {
            return;
        }
        
        if (Operation->IsOfType<FKeyFrameDragDropOp>())
        {
            const auto& FrameDragDropOp = StaticCastSharedPtr<FKeyFrameDragDropOp>(Operation);
            FrameDragDropOp->SetCanDropHere(true);
        }
    }

    void SSimpleFlipbookTimeline::OnDragLeave(const FDragDropEvent &DragDropEvent) {
        SCompoundWidget::OnDragLeave(DragDropEvent);

        auto Operation = DragDropEvent.GetOperation();
        if (!Operation.IsValid())
        {
            return;
        }
        
        if (Operation->IsOfType<FKeyFrameDragDropOp>())
        {
            const auto& FrameDragDropOp = StaticCastSharedPtr<FKeyFrameDragDropOp>(Operation);
            FrameDragDropOp->SetCanDropHere(false);
        }
    }

    FReply SSimpleFlipbookTimeline::OnDrop(const FGeometry &MyGeometry, const FDragDropEvent &DragDropEvent) {
        bool bWasDropHandled = false;

        TSharedPtr<FDragDropOperation> Operation = DragDropEvent.GetOperation();
        if (!Operation.IsValid())
        {
            return FReply::Unhandled();
        }

        if (Operation->IsOfType<FKeyFrameDragDropOp>())
        {
            const auto& FrameDragDropOp = StaticCastSharedPtr<FKeyFrameDragDropOp>(Operation);
            if (auto* ThisFlipbook = FlipbookBeingEdited.Get(); ThisFlipbook != nullptr)
            {
                FrameDragDropOp->AppendToFlipbook(ThisFlipbook);
                bWasDropHandled = true;
            }
        }

        return bWasDropHandled ? FReply::Handled() : FReply::Unhandled();
    }

    int32 SSimpleFlipbookTimeline::OnPaint(const FPaintArgs &Args, const FGeometry &AllottedGeometry,
        const FSlateRect &MyCullingRect, FSlateWindowElementList &OutDrawElements, int32 LayerId,
        const FWidgetStyle &InWidgetStyle, bool bParentEnabled) const {
        LayerId = SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

        const float CurrentTimeSecs = PlayTime.Get();
        auto* Flipbook = FlipbookBeingEdited.Get();
        const float TotalTimeSecs = Flipbook != nullptr ? Flipbook->GetTotalDuration() : 0.0f;
        const int32 TotalNumFrames = Flipbook != nullptr ? Flipbook->GetNumFrames() : 0;

        const float SlateTotalDistance = SlateUnitsPerFrame * TotalNumFrames;
        const float CurrentTimeXPos = CurrentTimeSecs / TotalTimeSecs * SlateTotalDistance - AnimationScrollBarPosition + FMath::Clamp((AllottedGeometry.GetLocalSize().X + AnimationScrollBarPosition) - SlateTotalDistance, 0.0f, AnimationScrollBarPosition);

        // Draw a line for the current scrub cursor
        ++LayerId;
        TArray<FVector2D> LinePoints;
        LinePoints.Add(FVector2D(CurrentTimeXPos, 0.f));
        LinePoints.Add(FVector2D(CurrentTimeXPos, AllottedGeometry.GetLocalSize().Y));

        FSlateDrawElement::MakeLines(
            OutDrawElements,
            LayerId,
            AllottedGeometry.ToPaintGeometry(),
            LinePoints,
            ESlateDrawEffect::None,
            FLinearColor::Red
            );

        return LayerId;
    }

    FReply SSimpleFlipbookTimeline::OnMouseWheel(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent) {
        if (MouseEvent.IsControlDown())
        {
            constexpr float DirectionScale = 0.08f;
            constexpr float MinFrameSize = 16.0f;
            const float Direction = MouseEvent.GetWheelDelta();
            const float NewUnitsPerFrame = FMath::Max(MinFrameSize, SlateUnitsPerFrame * (1.0f + Direction * DirectionScale));
            SlateUnitsPerFrame = NewUnitsPerFrame;
		
            CheckForRebuild(/*bRebuildAll=*/ true);

            return FReply::Handled();
        }
        
        return FReply::Unhandled();
    }

    FReply SSimpleFlipbookTimeline::OnMouseButtonUp(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent) {
        if (MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
        {
            auto MenuContents = GenerateContextMenu();
            auto WidgetPath = MouseEvent.GetEventPath() != nullptr ? *MouseEvent.GetEventPath() : FWidgetPath();
            FSlateApplication::Get().PushMenu(AsShared(), WidgetPath, MenuContents, MouseEvent.GetScreenSpacePosition(), FPopupTransitionEffect(FPopupTransitionEffect::ContextMenu));

            return FReply::Handled();
        }
        
        return FReply::Unhandled();
    }

    void SSimpleFlipbookTimeline::Tick(const FGeometry &AllottedGeometry, const double InCurrentTime,
        const float InDeltaTime) {
        CheckForRebuild();
    }

    void SSimpleFlipbookTimeline::RebuildPerFrameBG() {
        constexpr std::array BackgroundColors = { FLinearColor(1.0f, 1.0f, 1.0f, 0.05f), FLinearColor(0.0f, 0.0f, 0.0f, 0.05f) };

        BackgroundPerFrameSlices->ClearChildren();
        for (int32 FrameIndex = 0; FrameIndex < NumFramesFromLastRebuild; ++FrameIndex)
        {
            const FLinearColor& BackgroundColorForFrameIndex = BackgroundColors[FrameIndex & 1];

            BackgroundPerFrameSlices->AddSlot()
            .AutoWidth()
            [
                SNew(SBox)
                .WidthOverride(SlateUnitsPerFrame)
                [
                    SNew(SColorBlock)
                    .Color(BackgroundColorForFrameIndex)
                ]
            ];
        }
    }

    TSharedRef<SWidget> SSimpleFlipbookTimeline::GenerateContextMenu() {
        FMenuBuilder MenuBuilder(true, CommandList);
        MenuBuilder.BeginSection("KeyframeActions", NSLOCTEXT("Simple2D", "KeyframeActionsSectionHeader", "Keyframe Actions"));
        
        MenuBuilder.AddMenuEntry(FSimpleFlipbookEditorCommands::Get().AddNewFrame);

        MenuBuilder.EndSection();

        return MenuBuilder.MakeWidget();
    }

    EVisibility SSimpleFlipbookTimeline::NoFramesWarningVisibility() const {
        auto* Flipbook = FlipbookBeingEdited.Get();
        const int32 TotalNumFrames = Flipbook != nullptr ? Flipbook->GetNumFrames() : 0;
        return TotalNumFrames == 0 ? EVisibility::Visible : EVisibility::Collapsed;
    }

    void SSimpleFlipbookTimeline::CheckForRebuild(bool bRebuildAll) {
        auto* Flipbook = FlipbookBeingEdited.Get();

        const int32 NewNumKeyframes = (Flipbook != nullptr) ? Flipbook->GetNumKeyFrames() : 0;
        if ((NewNumKeyframes != NumKeyFramesFromLastRebuild) || bRebuildAll)
        {
            NumKeyFramesFromLastRebuild = NewNumKeyframes;
            TimelineTrack->Rebuild();
        }

        if (const int32 NewNumFrames = Flipbook != nullptr ? Flipbook->GetNumFrames() : 0; NewNumFrames != NumFramesFromLastRebuild || bRebuildAll)
        {
            NumFramesFromLastRebuild = NewNumFrames;
            TimelineHeader->Rebuild();
            RebuildPerFrameBG();
        }
    }

    void SSimpleFlipbookTimeline::AnimationScrollBar_OnUserScrolled(float ScrollOffset) {
        AnimationScrollBarPosition = ScrollOffset;
    }
}