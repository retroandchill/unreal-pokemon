// "Unreal Pokémon" created by Retro & Chill.

#include "Simple2D/Assets/SimpleFlipbook/Widgets/SimpleFlipbookTimelineTrack.h"
#include "Framework/Commands/GenericCommands.h"
#include "Simple2D/Assets/SimpleFlipbook/SimpleFlipbookEditorCommands.h"
#include "Simple2D/Assets/SimpleFlipbook/Widgets/SimpleFlipbookTrackHandle.h"
#include "Simple2D/Simple2DStyle.h"
#include "SlateOptMacros.h"

namespace Simple2D
{

    TSharedPtr<SWidget> FKeyFrameDragDropOp::GetDefaultDecorator() const
    {
        const FLinearColor BorderColor = FLinearColor::White;

        return SNew(SBox)
            .WidthOverride(WidgetWidth - Flipbooks::FramePadding.GetTotalSpaceAlong<Orient_Horizontal>())
            .HeightOverride(Flipbooks::FrameHeight - Flipbooks::FramePadding.GetTotalSpaceAlong<Orient_Vertical>())
                [SNew(SBorder)
                     .BorderImage(FSimple2DStyle::Get()->GetBrush("FlipbookEditor.RegionBody"))
                     .BorderBackgroundColor(BorderColor)
                     .HAlign(HAlign_Center)
                     .VAlign(VAlign_Center)[SNew(STextBlock).ColorAndOpacity(FLinearColor::Black).Text(BodyText)]];
    }

    void FKeyFrameDragDropOp::OnDragged(const FDragDropEvent &DragDropEvent)
    {
        if (CursorDecoratorWindow.IsValid())
        {
            CursorDecoratorWindow->MoveWindowTo(DragDropEvent.GetScreenSpacePosition());
        }
    }

    void FKeyFrameDragDropOp::Construct()
    {
        MouseCursor = EMouseCursor::GrabHandClosed;
        BodyText = FText::FormatOrdered(NSLOCTEXT("Simple2D", "SpriteIndex", "Index {0}"), SourceFrameIndex);
        FDragDropOperation::Construct();
    }

    void FKeyFrameDragDropOp::OnDrop(bool bDropWasHandled, const FPointerEvent &MouseEvent)
    {
        if (!bDropWasHandled)
        {
            // Add us back to our source, the drop fizzled
            InsertInFlipbook(SourceFlipbook.Get(), SourceFrameIndex);
            Transaction.Cancel();
        }
    }

    void FKeyFrameDragDropOp::AppendToFlipbook(USimpleFlipbook *DestinationFlipbook)
    {
        DestinationFlipbook->Modify();
        FScopedSimpleFlipbookMutator EditLock(DestinationFlipbook);
        EditLock.KeyFrames.Add(KeyFrameData);
    }

    void FKeyFrameDragDropOp::InsertInFlipbook(USimpleFlipbook *DestinationFlipbook, int32 Index)
    {
        DestinationFlipbook->Modify();
        FScopedSimpleFlipbookMutator EditLock(DestinationFlipbook);
        EditLock.KeyFrames.Insert(KeyFrameData, Index);
    }

    TSharedRef<FKeyFrameDragDropOp> FKeyFrameDragDropOp::New(int32 InWidth, USimpleFlipbook *InFlipbook,
                                                             int32 InFrameIndex)
    {
        // Create the drag-drop op containing the key
        auto Operation = MakeShared<FKeyFrameDragDropOp>(FInitTag(), InFlipbook->GetKeyFrameChecked(InFrameIndex),
                                                         InFrameIndex, InFlipbook, InWidth);
        Operation->Construct();

        // Remove the key from the flipbook
        {
            InFlipbook->Modify();
            FScopedSimpleFlipbookMutator EditLock(InFlipbook);
            EditLock.KeyFrames.RemoveAt(InFrameIndex);
        }

        return Operation;
    }

    void SSimpleFlipbookKeyframeWidget::Construct(const FArguments &InArgs, int32 InFrameIndex)
    {
        FrameIndex = InFrameIndex;

        SlateUnitsPerFrame = InArgs._SlateUnitsPerFrame;
        FlipbookBeingEdited = InArgs._FlipbookBeingEdited;
        OnSelectionChanged = InArgs._OnSelectionChanged;

        // Color each region based on whether a sprite has been set or not for it
        const auto BorderColorDelegate = [](TAttribute<USimpleFlipbook *>, int32) -> FSlateColor {
            return FLinearColor::White;
        };

        ChildSlot[SNew(SOverlay) +
                  SOverlay::Slot()
                      [SNew(SBox)
                           .Padding(Flipbooks::FramePadding)
                           .WidthOverride(this, &SSimpleFlipbookKeyframeWidget::GetFrameWidth)
                               [SNew(SBorder)
                                    .BorderImage(FSimple2DStyle::Get()->GetBrush("FlipbookEditor.RegionBody"))
                                    .BorderBackgroundColor_Static(BorderColorDelegate, FlipbookBeingEdited, FrameIndex)
                                    .OnMouseButtonUp(this, &SSimpleFlipbookKeyframeWidget::KeyframeOnMouseButtonUp)
                                    .ToolTipText(this, &SSimpleFlipbookKeyframeWidget::GetKeyframeTooltip)
                                    .HAlign(HAlign_Center)
                                    .VAlign(VAlign_Center)
                                        [SNew(STextBlock)
                                             .ColorAndOpacity(FLinearColor::Black)
                                             .Text(this, &SSimpleFlipbookKeyframeWidget::GetKeyframeText)]]] +
                  SOverlay::Slot().HAlign(HAlign_Right)[SNew(SBox).WidthOverride(
                      Flipbooks::HandleWidth)[SNew(SSimpleFlipbookTrackHandle)
                                                  .SlateUnitsPerFrame(SlateUnitsPerFrame)
                                                  .FlipbookBeingEdited(FlipbookBeingEdited)
                                                  .KeyFrameIdx(FrameIndex)]]];
    }

    FReply SSimpleFlipbookKeyframeWidget::OnMouseButtonDown(const FGeometry &MyGeometry,
                                                            const FPointerEvent &MouseEvent)
    {
        if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
        {
            return FReply::Handled().DetectDrag(SharedThis(this), EKeys::LeftMouseButton);
        }

        return FReply::Unhandled();
    }

    FReply SSimpleFlipbookKeyframeWidget::OnDragDetected(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent)
    {
        if (MouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
        {
            if (auto *Flipbook = FlipbookBeingEdited.Get();
                (Flipbook != nullptr) && Flipbook->IsValidKeyFrameIndex(FrameIndex))
            {
                auto Operation = FKeyFrameDragDropOp::New(GetFrameWidth().Get(), Flipbook, FrameIndex);

                return FReply::Handled().BeginDragDrop(Operation);
            }
        }

        return FReply::Unhandled();
    }

    FReply SSimpleFlipbookKeyframeWidget::OnDrop(const FGeometry &MyGeometry, const FDragDropEvent &DragDropEvent)
    {
        bool bWasDropHandled = false;

        if (auto *Flipbook = FlipbookBeingEdited.Get();
            Flipbook != nullptr && Flipbook->IsValidKeyFrameIndex(FrameIndex))
        {

            auto Operation = DragDropEvent.GetOperation();
            if (!Operation.IsValid())
            {
                return FReply::Unhandled();
            }

            if (Operation->IsOfType<FKeyFrameDragDropOp>())
            {
                const auto &FrameDragDropOp = StaticCastSharedPtr<FKeyFrameDragDropOp>(Operation);
                FrameDragDropOp->InsertInFlipbook(Flipbook, FrameIndex);
                bWasDropHandled = true;
            }
        }

        return bWasDropHandled ? FReply::Handled() : FReply::Unhandled();
    }

    FReply SSimpleFlipbookKeyframeWidget::KeyframeOnMouseButtonUp(const FGeometry &MyGeometry,
                                                                  const FPointerEvent &MouseEvent)
    {
        if (MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
        {
            TSharedRef<SWidget> MenuContents = GenerateContextMenu();
            FWidgetPath WidgetPath = MouseEvent.GetEventPath() != nullptr ? *MouseEvent.GetEventPath() : FWidgetPath();
            FSlateApplication::Get().PushMenu(AsShared(), WidgetPath, MenuContents, MouseEvent.GetScreenSpacePosition(),
                                              FPopupTransitionEffect(FPopupTransitionEffect::ContextMenu));

            return FReply::Handled();
        }

        return FReply::Unhandled();
    }

    FText SSimpleFlipbookKeyframeWidget::GetKeyframeText() const
    {
        if (auto KeyFrame = GetKeyFrameData(); KeyFrame.IsSet())
        {
            return FText::FormatOrdered(NSLOCTEXT("Simple2D", "SpriteIndex", "Index {0}"), KeyFrame->Index);
        }

        return FText::GetEmpty();
    }

    FText SSimpleFlipbookKeyframeWidget::GetKeyframeTooltip() const
    {
        if (auto KeyFrame = GetKeyFrameData(); KeyFrame.IsSet())
        {
            const FText SpriteLine =
                (KeyFrame->Index != INDEX_NONE)
                    ? FText::FormatOrdered(NSLOCTEXT("Simple2D", "SpriteFrame", "Frame {0}"), KeyFrame->Index)
                    : NSLOCTEXT("Simple2D", "NoSprite", "(none)");

            const FText FramesText = (KeyFrame->FrameRun == 1) ? NSLOCTEXT("Simple2D", "SingularFrames", "frame")
                                                               : NSLOCTEXT("Simple2D", "PluralFrames", "frames");

            return FText::Format(NSLOCTEXT("Simple2D", "KeyFrameTooltip", "Sprite: {0}\nIndex: {1}\nDuration: {2} {3}"),
                                 SpriteLine, FText::AsNumber(FrameIndex), FText::AsNumber(KeyFrame->FrameRun),
                                 FramesText);
        }

        return NSLOCTEXT("Simple2D", "KeyFrameTooltip_Invalid", "Invalid key frame index");
    }

    TSharedRef<SWidget> SSimpleFlipbookKeyframeWidget::GenerateContextMenu()
    {
        const auto &Commands = FSimpleFlipbookEditorCommands::Get();

        OnSelectionChanged.ExecuteIfBound(FrameIndex);

        FMenuBuilder MenuBuilder(true, CommandList);
        {
            FNumberFormattingOptions NoCommas;
            NoCommas.UseGrouping = false;

            const FText KeyframeSectionTitle =
                FText::Format(NSLOCTEXT("Simple2D", "KeyframeXActionsSectionHeader", "Keyframe #{0} Actions"),
                              FText::AsNumber(FrameIndex, &NoCommas));
            MenuBuilder.BeginSection("KeyframeActions", KeyframeSectionTitle);

            MenuBuilder.AddMenuEntry(FGenericCommands::Get().Duplicate);
            MenuBuilder.AddMenuEntry(FGenericCommands::Get().Delete);

            MenuBuilder.AddMenuSeparator();

            MenuBuilder.AddMenuEntry(Commands.AddNewFrameBefore);
            MenuBuilder.AddMenuEntry(Commands.AddNewFrameAfter);

            MenuBuilder.EndSection();
        }

        return MenuBuilder.MakeWidget();
    }

    FOptionalSize SSimpleFlipbookKeyframeWidget::GetFrameWidth() const
    {
        if (auto KeyFrame = GetKeyFrameData(); KeyFrame.IsSet())
        {
            return FMath::Max<float>(0, KeyFrame->FrameRun * SlateUnitsPerFrame.Get());
        }

        return 1;
    }

    TOptional<const FSimpleFlipbookKeyFrame &> SSimpleFlipbookKeyframeWidget::GetKeyFrameData() const
    {
        if (auto *Flipbook = FlipbookBeingEdited.Get();
            Flipbook != nullptr && Flipbook->IsValidKeyFrameIndex(FrameIndex))
        {
            return Flipbook->GetKeyFrameChecked(FrameIndex);
        }

        return nullptr;
    }

    void SSimpleFlipbookTimelineTrack::Construct(const FArguments &InArgs,
                                                 const TSharedPtr<FUICommandList> &InCommandList)
    {
        CommandList = InCommandList;
        SlateUnitsPerFrame = InArgs._SlateUnitsPerFrame;
        FlipbookBeingEdited = InArgs._FlipbookBeingEdited;
        OnSelectionChanged = InArgs._OnSelectionChanged;

        ChildSlot[SAssignNew(MainBoxPtr, SHorizontalBox)];

        Rebuild();
    }

    void SSimpleFlipbookTimelineTrack::Rebuild()
    {
        MainBoxPtr->ClearChildren();

        // Create the sections for each keyframe
        auto *Flipbook = FlipbookBeingEdited.Get();
        if (Flipbook == nullptr)
        {
            return;
        }

        for (int32 KeyFrameIdx = 0; KeyFrameIdx < Flipbook->GetNumKeyFrames(); ++KeyFrameIdx)
        {
            MainBoxPtr->AddSlot().AutoWidth()[SNew(SSimpleFlipbookKeyframeWidget, KeyFrameIdx)
                                                  .SlateUnitsPerFrame(this->SlateUnitsPerFrame)
                                                  .FlipbookBeingEdited(this->FlipbookBeingEdited)
                                                  .OnSelectionChanged(this->OnSelectionChanged)];
        }
    }
} // namespace Simple2D