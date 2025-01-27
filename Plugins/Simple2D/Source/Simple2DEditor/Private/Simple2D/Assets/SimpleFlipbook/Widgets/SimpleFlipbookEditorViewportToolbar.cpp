// "Unreal Pokémon" created by Retro & Chill.

#include "Simple2D/Assets/SimpleFlipbook/Widgets/SimpleFlipbookEditorViewportToolbar.h"
#include "SEditorViewport.h"
#include "Simple2D/Assets/SimpleFlipbook/SimpleFlipbookEditorCommands.h"
#include "SlateOptMacros.h"

namespace Simple2D {
    void SSimpleFlipbookEditorViewportToolbar::Construct(
        const FArguments &, const TSharedPtr<ICommonEditorViewportToolbarInfoProvider> &InInfoProvider) {
        SCommonEditorViewportToolbarBase::Construct(SCommonEditorViewportToolbarBase::FArguments(), InInfoProvider);
    }

    TSharedRef<SWidget> SSimpleFlipbookEditorViewportToolbar::GenerateShowMenu() const {
        GetInfoProvider().OnFloatingButtonClicked();

        TSharedRef<SEditorViewport> ViewportRef = GetInfoProvider().GetViewportWidget();

        constexpr bool bInShouldCloseWindowAfterMenuSelection = true;
        FMenuBuilder ShowMenuBuilder(bInShouldCloseWindowAfterMenuSelection, ViewportRef->GetCommandList());
        ShowMenuBuilder.AddMenuEntry(FSimpleFlipbookEditorCommands::Get().SetShowPivot);

        ShowMenuBuilder.AddMenuSeparator();

        ShowMenuBuilder.AddMenuEntry(FSimpleFlipbookEditorCommands::Get().SetShowGrid);
        ShowMenuBuilder.AddMenuEntry(FSimpleFlipbookEditorCommands::Get().SetShowBounds);

        ShowMenuBuilder.AddMenuSeparator();

        return ShowMenuBuilder.MakeWidget();
    }
} // namespace Simple2D