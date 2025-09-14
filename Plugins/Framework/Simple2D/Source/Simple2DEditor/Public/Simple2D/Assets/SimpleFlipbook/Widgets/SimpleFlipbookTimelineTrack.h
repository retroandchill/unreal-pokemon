// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "RetroLib/Optionals/OptionalOperations.h"
#include "Simple2D/Assets/SimpleFlipbook.h"
#include "SimpleFlipbookTimeline.h"
#include "Widgets/SCompoundWidget.h"

namespace Simple2D
{
    namespace Flipbooks
    {
        constexpr float HandleWidth = 12.0f;
        constexpr float FrameHeight = 48;
        constexpr float HeightBeforeFrames = 16;
        const FMargin FramePadding(0.0f, 7.0f, 0.0f, 7.0f);
    } // namespace Flipbooks

    class FKeyFrameDragDropOp : public FDragDropOperation
    {
        struct FInitTag
        {
        };

      public:
        explicit FKeyFrameDragDropOp(FInitTag, FSimpleFlipbookKeyFrame KeyFrameData, int32 SourceFrameIndex,
                                     USimpleFlipbook *SourceFlipbook, float WidgetWidth)
            : WidgetWidth(WidgetWidth), KeyFrameData(KeyFrameData), SourceFrameIndex(SourceFrameIndex),
              SourceFlipbook(SourceFlipbook)
        {
        }

        DRAG_DROP_OPERATOR_TYPE(FKeyFrameDragDropOp, FDragDropOperation)

        float WidgetWidth;
        FSimpleFlipbookKeyFrame KeyFrameData;
        int32 SourceFrameIndex;
        FText BodyText;
        TWeakObjectPtr<USimpleFlipbook> SourceFlipbook;
        FScopedTransaction Transaction = NSLOCTEXT("Simple2D", "MovedFramesInTimeline", "Reorder key frames");

        TSharedPtr<SWidget> GetDefaultDecorator() const override;
        void OnDragged(const class FDragDropEvent &DragDropEvent) override;
        void Construct() override;
        void OnDrop(bool bDropWasHandled, const FPointerEvent &MouseEvent) override;

        void AppendToFlipbook(USimpleFlipbook *DestinationFlipbook);

        void InsertInFlipbook(USimpleFlipbook *DestinationFlipbook, int32 Index);

        void SetCanDropHere(bool bCanDropHere)
        {
            MouseCursor = bCanDropHere ? EMouseCursor::TextEditBeam : EMouseCursor::SlashedCircle;
        }

        static TSharedRef<FKeyFrameDragDropOp> New(int32 InWidth, USimpleFlipbook *InFlipbook, int32 InFrameIndex);
    };

    class SSimpleFlipbookKeyframeWidget : public SCompoundWidget
    {
      public:
        SLATE_BEGIN_ARGS(SSimpleFlipbookKeyframeWidget) : _SlateUnitsPerFrame(1), _FlipbookBeingEdited(nullptr)
            {
            }

            SLATE_ATTRIBUTE(float, SlateUnitsPerFrame)
            SLATE_ATTRIBUTE(USimpleFlipbook *, FlipbookBeingEdited)
            SLATE_EVENT(FOnKeyframeSelectionChanged, OnSelectionChanged)

            SLATE_END_ARGS()

            void Construct(const FArguments &InArgs, int32 InFrameIndex);

            FReply OnMouseButtonDown(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent) override;
            FReply OnDragDetected(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent) override;
            FReply OnDrop(const FGeometry &MyGeometry, const FDragDropEvent &DragDropEvent) override;

          protected:
            FReply KeyframeOnMouseButtonUp(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent);

            FText GetKeyframeText() const;
            FText GetKeyframeTooltip() const;

            TSharedRef<SWidget> GenerateContextMenu();

            FOptionalSize GetFrameWidth() const;

            // Can return null
            TOptional<const FSimpleFlipbookKeyFrame &> GetKeyFrameData() const;

          private:
            int32 FrameIndex = 0;
            TAttribute<float> SlateUnitsPerFrame;
            TAttribute<USimpleFlipbook *> FlipbookBeingEdited;
            FOnKeyframeSelectionChanged OnSelectionChanged;
            TSharedPtr<FUICommandList> CommandList;
        };

        /**
         *
         */
        class SIMPLE2DEDITOR_API SSimpleFlipbookTimelineTrack : public SCompoundWidget
        {
          public:
            SLATE_BEGIN_ARGS(SSimpleFlipbookTimelineTrack) : _SlateUnitsPerFrame(1), _FlipbookBeingEdited(nullptr)
                {
                }

                SLATE_ATTRIBUTE(float, SlateUnitsPerFrame)
                SLATE_ATTRIBUTE(USimpleFlipbook *, FlipbookBeingEdited)
                SLATE_EVENT(FOnKeyframeSelectionChanged, OnSelectionChanged)

                SLATE_END_ARGS()

                void Construct(const FArguments &InArgs, const TSharedPtr<FUICommandList> &InCommandList);

                void Rebuild();

              private:
                TAttribute<float> SlateUnitsPerFrame;
                TAttribute<USimpleFlipbook *> FlipbookBeingEdited;

                TSharedPtr<SHorizontalBox> MainBoxPtr;

                FOnKeyframeSelectionChanged OnSelectionChanged;
                TSharedPtr<FUICommandList> CommandList;
            };
        }