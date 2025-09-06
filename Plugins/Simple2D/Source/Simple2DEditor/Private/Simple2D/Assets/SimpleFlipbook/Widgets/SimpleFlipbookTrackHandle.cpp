// "Unreal Pokémon" created by Retro & Chill.

#include "Simple2D/Assets/SimpleFlipbook/Widgets/SimpleFlipbookTrackHandle.h"
#include "Simple2D/Assets/SimpleFlipbook.h"
#include "Simple2D/Simple2DStyle.h"
#include "SlateOptMacros.h"

namespace Simple2D
{

    void SSimpleFlipbookTrackHandle::Construct(const FArguments &InArgs)
    {
        SlateUnitsPerFrame = InArgs._SlateUnitsPerFrame;
        FlipbookBeingEdited = InArgs._FlipbookBeingEdited;
        KeyFrameIdx = InArgs._KeyFrameIdx;

        SImage::Construct(
            SImage::FArguments().Image(FSimple2DStyle::Get()->GetBrush("FlipbookEditor.RegionGrabHandle")));
    }

    FReply SSimpleFlipbookTrackHandle::OnMouseButtonDown(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent)
    {
        if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
        {
            DistanceDragged = 0;
            StartingFrameRun = INDEX_NONE;
            return FReply::Handled().CaptureMouse(SharedThis(this)).UseHighPrecisionMouseMovement(SharedThis(this));
        }

        return FReply::Unhandled();
    }

    FReply SSimpleFlipbookTrackHandle::OnMouseButtonUp(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent)
    {
        if (MouseEvent.GetEffectingButton() != EKeys::LeftMouseButton || !this->HasMouseCapture())
        {
            return FReply::Unhandled();
        }
        if (bDragging && (StartingFrameRun != INDEX_NONE))
        {
            if (auto *Flipbook = FlipbookBeingEdited.Get(); Flipbook && Flipbook->IsValidKeyFrameIndex(KeyFrameIdx))
            {

                if (const auto &[Index, FrameRun] = Flipbook->GetKeyFrameChecked(KeyFrameIdx);
                    FrameRun != StartingFrameRun)
                {
                    Flipbook->MarkPackageDirty();
                    Flipbook->PostEditChange();
                }
            }
        }

        bDragging = false;

        FIntPoint NewMousePos((MyGeometry.AbsolutePosition.X + MyGeometry.GetLocalSize().X / 2) * MyGeometry.Scale,
                              (MyGeometry.AbsolutePosition.Y + MyGeometry.GetLocalSize().Y / 2) * MyGeometry.Scale);

        return FReply::Handled().ReleaseMouseCapture().SetMousePos(NewMousePos);
    }

    FReply SSimpleFlipbookTrackHandle::OnMouseMove(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent)
    {
        if (!this->HasMouseCapture())
        {
            return FReply::Unhandled();
        }

        if (auto *Flipbook = FlipbookBeingEdited.Get();
            Flipbook != nullptr && Flipbook->IsValidKeyFrameIndex(KeyFrameIdx))
        {
            DistanceDragged += MouseEvent.GetCursorDelta().X;

            if (!bDragging)
            {
                if (FMath::Abs(DistanceDragged) > FSlateApplication::Get().GetDragTriggerDistance())
                {
                    const auto &[Index, FrameRun] = Flipbook->GetKeyFrameChecked(KeyFrameIdx);
                    StartingFrameRun = FrameRun;
                    bDragging = true;
                }
            }
            else
            {
                if (float LocalSlateUnitsPerFrame = SlateUnitsPerFrame.Get(); LocalSlateUnitsPerFrame != 0)
                {
                    FScopedSimpleFlipbookMutator EditLock(Flipbook);
                    auto &[Index, FrameRun] = EditLock.KeyFrames[KeyFrameIdx];
                    FrameRun = StartingFrameRun + (DistanceDragged / LocalSlateUnitsPerFrame);
                    FrameRun = FMath::Max<int32>(1, FrameRun);
                }
            }
        }

        return FReply::Handled();
    }

    FCursorReply SSimpleFlipbookTrackHandle::OnCursorQuery(const FGeometry &MyGeometry,
                                                           const FPointerEvent &CursorEvent) const
    {
        return FCursorReply::Cursor(bDragging ? EMouseCursor::None : EMouseCursor::ResizeLeftRight);
    }
} // namespace Simple2D