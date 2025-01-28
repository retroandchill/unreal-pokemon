// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class USimpleFlipbook;

namespace Simple2D {
    /**
     *
     */
    class SIMPLE2DEDITOR_API SSimpleFlipbookTrackHandle : public SImage {
      public:
        SLATE_BEGIN_ARGS(SSimpleFlipbookTrackHandle)
                : _SlateUnitsPerFrame(1), _FlipbookBeingEdited(nullptr), _KeyFrameIdx(INDEX_NONE) {
            }

            SLATE_ATTRIBUTE(float, SlateUnitsPerFrame)
            SLATE_ATTRIBUTE(USimpleFlipbook *, FlipbookBeingEdited)
            SLATE_ARGUMENT(int32, KeyFrameIdx)
            SLATE_END_ARGS()

            /** Constructs this widget with InArgs */
            void Construct(const FArguments &InArgs);

            FReply OnMouseButtonDown(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent) override;
            FReply OnMouseButtonUp(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent) override;
            FReply OnMouseMove(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent) override;

            FCursorReply OnCursorQuery(const FGeometry &MyGeometry, const FPointerEvent &CursorEvent) const override;

          private:
            float DistanceDragged = 0;
            int32 StartingFrameRun = INDEX_NONE;
            bool bDragging = false;

            TAttribute<float> SlateUnitsPerFrame;
            TAttribute<USimpleFlipbook *> FlipbookBeingEdited;
            int32 KeyFrameIdx = INDEX_NONE;
        };
    }
