// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "Widgets/SCompoundWidget.h"

class USimpleFlipbook;

namespace Simple2D {
    /**
     * 
     */
    class SIMPLE2DEDITOR_API SSimpleFlipbookTimelineHeader : public SCompoundWidget {
    public:
        SLATE_BEGIN_ARGS(SSimpleFlipbookTimelineHeader) : _SlateUnitsPerFrame(1) , _FlipbookBeingEdited(nullptr), _PlayTime(0) {}
            
            SLATE_ATTRIBUTE(float, SlateUnitsPerFrame)
            SLATE_ATTRIBUTE(USimpleFlipbook*, FlipbookBeingEdited)
            SLATE_ATTRIBUTE(float, PlayTime)
        SLATE_END_ARGS()

        void Construct(const FArguments &InArgs);
        void Rebuild();

    private:
        TAttribute<float> SlateUnitsPerFrame;
        TAttribute<USimpleFlipbook*> FlipbookBeingEdited;
        TAttribute<float> PlayTime;

        TSharedPtr<SHorizontalBox> MainBoxPtr;

        int32 NumFramesFromLastRebuild = 0;
    };
}
