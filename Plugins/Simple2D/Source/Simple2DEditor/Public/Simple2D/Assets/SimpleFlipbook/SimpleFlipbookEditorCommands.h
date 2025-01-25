// "Unreal Pokémon" created by Retro & Chill.

#pragma once
#include "Simple2D/Simple2DStyle.h"


namespace Simple2D {
    /**
     * 
     */
    class SIMPLE2DEDITOR_API FSimpleFlipbookEditorCommands : public TCommands<FSimpleFlipbookEditorCommands> {
    protected:
        FSimpleFlipbookEditorCommands() : TCommands(TEXT("SimpleFlipbookEditor"),NSLOCTEXT("Contexts", "SimpleFlipbookEditor", "Simple Flipbook Editor"),NAME_None, FSimple2DStyle::Get()->GetStyleSetName()) {}

        void RegisterCommands() override;

    public:
        TSharedPtr<FUICommandInfo> AddKeyFrame;

        TSharedPtr<FUICommandInfo> SetShowGrid;
        TSharedPtr<FUICommandInfo> SetShowBounds;
        TSharedPtr<FUICommandInfo> SetShowPivot;

        TSharedPtr<FUICommandInfo> AddNewFrame;
        TSharedPtr<FUICommandInfo> AddNewFrameBefore;
        TSharedPtr<FUICommandInfo> AddNewFrameAfter;
        
    };
}
