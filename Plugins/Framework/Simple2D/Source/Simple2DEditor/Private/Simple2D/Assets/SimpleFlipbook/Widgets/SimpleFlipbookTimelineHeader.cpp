// "Unreal Pokémon" created by Retro & Chill.

#include "Simple2D/Assets/SimpleFlipbook/Widgets/SimpleFlipbookTimelineHeader.h"
#include "Simple2D/Assets/SimpleFlipbook.h"
#include "SlateOptMacros.h"

namespace Simple2D
{

    void SSimpleFlipbookTimelineHeader::Construct(const FArguments &InArgs)
    {
        SlateUnitsPerFrame = InArgs._SlateUnitsPerFrame;
        FlipbookBeingEdited = InArgs._FlipbookBeingEdited;
        PlayTime = InArgs._PlayTime;

        NumFramesFromLastRebuild = 0;

        ChildSlot[SAssignNew(MainBoxPtr, SHorizontalBox)];

        Rebuild();
    }

    void SSimpleFlipbookTimelineHeader::Rebuild()
    {
        MainBoxPtr->ClearChildren();

        auto *Flipbook = FlipbookBeingEdited.Get();
        if (const float LocalSlateUnitsPerFrame = SlateUnitsPerFrame.Get();
            Flipbook != nullptr && (LocalSlateUnitsPerFrame > 0))
        {
            const int32 NumFrames = Flipbook->GetNumFrames();
            for (int32 FrameIndex = 0; FrameIndex < NumFrames; ++FrameIndex)
            {
                MainBoxPtr->AddSlot()
                    .AutoWidth()[SNew(SBox)
                                     .WidthOverride(LocalSlateUnitsPerFrame)
                                     .HAlign(HAlign_Center)[SNew(STextBlock).Text(FText::AsNumber(FrameIndex))]];
            }

            NumFramesFromLastRebuild = NumFrames;
        }
        else
        {
            NumFramesFromLastRebuild = 0;
        }
    }
} // namespace Simple2D