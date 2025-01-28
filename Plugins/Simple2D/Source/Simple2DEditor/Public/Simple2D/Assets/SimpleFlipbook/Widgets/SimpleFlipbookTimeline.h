// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class USimpleFlipbook;

namespace Simple2D {
    class SSimpleFlipbookTimelineTrack;
    class SSimpleFlipbookTimelineHeader;

    DECLARE_DELEGATE_OneParam(FOnKeyframeSelectionChanged, int32);

    /**
     *
     */
    class SIMPLE2DEDITOR_API SSimpleFlipbookTimeline : public SCompoundWidget {
      public:
        SLATE_BEGIN_ARGS(SSimpleFlipbookTimeline) : _FlipbookBeingEdited(nullptr), _PlayTime(0) {
            }

            SLATE_ATTRIBUTE(USimpleFlipbook *, FlipbookBeingEdited)
            SLATE_ATTRIBUTE(float, PlayTime)
            SLATE_EVENT(FOnKeyframeSelectionChanged, OnSelectionChanged)
            SLATE_END_ARGS()

            /** Constructs this widget with InArgs */
            void Construct(const FArguments &InArgs, TSharedPtr<FUICommandList> InCommandList);

            void OnDragEnter(const FGeometry &MyGeometry, const FDragDropEvent &DragDropEvent) override;
            void OnDragLeave(const FDragDropEvent &DragDropEvent) override;
            FReply OnDrop(const FGeometry &MyGeometry, const FDragDropEvent &DragDropEvent) override;
            int32 OnPaint(const FPaintArgs &Args, const FGeometry &AllottedGeometry, const FSlateRect &MyCullingRect,
                          FSlateWindowElementList &OutDrawElements, int32 LayerId, const FWidgetStyle &InWidgetStyle,
                          bool bParentEnabled) const override;
            FReply OnMouseWheel(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent) override;
            FReply OnMouseButtonUp(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent) override;
            void Tick(const FGeometry &AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

          private:
            void RebuildPerFrameBG();

            TSharedRef<SWidget> GenerateContextMenu();
            EVisibility NoFramesWarningVisibility() const;
            void CheckForRebuild(bool bRebuildAll = false);

            float GetSlateUnitsPerFrame() const {
                return SlateUnitsPerFrame;
            }

            void AnimationScrollBar_OnUserScrolled(float ScrollOffset);

            TSharedPtr<SHorizontalBox> BackgroundPerFrameSlices;
            TSharedPtr<SSimpleFlipbookTimelineHeader> TimelineHeader;
            TSharedPtr<SSimpleFlipbookTimelineTrack> TimelineTrack;

            TAttribute<USimpleFlipbook *> FlipbookBeingEdited;
            TAttribute<float> PlayTime;
            TSharedPtr<FUICommandList> CommandList;
            FOnKeyframeSelectionChanged OnSelectionChanged;

            float SlateUnitsPerFrame = 120.0f;
            float AnimationScrollBarPosition = 0.0f;

            // Observer values (used to detect if the widget needs to be rebuilt)
            int32 NumFramesFromLastRebuild = 0;
            int32 NumKeyFramesFromLastRebuild = 0;
        };
    }
