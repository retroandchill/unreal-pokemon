// "Unreal Pokémon" created by Retro & Chill.

#include "Simple2D/Assets/SimpleFlipbook/Widgets/SimpleFlipbookEditorViewport.h"
#include "Simple2D/Assets/SimpleFlipbook/SimpleFlipbookEditorCommands.h"
#include "Simple2D/Assets/SimpleFlipbook/Widgets/SimpleFlipbookEditorViewportToolbar.h"
#include "SlateOptMacros.h"

namespace Simple2D {
    void SSimpleFlipbookEditorViewport::Construct(const FArguments &InArgs) {
        FlipbookBeingEdited = InArgs._FlipbookBeingEdited;
        SEditorViewport::Construct(SEditorViewport::FArguments());
    }

    void SSimpleFlipbookEditorViewport::BindCommands() {
        SEditorViewport::BindCommands();

        auto &Commands = FSimpleFlipbookEditorCommands::Get();

        TSharedRef<FSimpleFlipbookEditorViewportClient> EditorViewportClientRef = EditorViewportClient.ToSharedRef();

        CommandList->MapAction(
            Commands.SetShowGrid,
            FExecuteAction::CreateSP(EditorViewportClientRef, &FEditorViewportClient::SetShowGrid), FCanExecuteAction(),
            FIsActionChecked::CreateSP(EditorViewportClientRef, &FEditorViewportClient::IsSetShowGridChecked));

        CommandList->MapAction(
            Commands.SetShowBounds,
            FExecuteAction::CreateSP(EditorViewportClientRef, &FEditorViewportClient::ToggleShowBounds),
            FCanExecuteAction(),
            FIsActionChecked::CreateSP(EditorViewportClientRef, &FEditorViewportClient::IsSetShowBoundsChecked));

        CommandList->MapAction(
            Commands.SetShowPivot,
            FExecuteAction::CreateSP(EditorViewportClientRef, &FSimpleFlipbookEditorViewportClient::ToggleShowPivot),
            FCanExecuteAction(),
            FIsActionChecked::CreateSP(EditorViewportClientRef,
                                       &FSimpleFlipbookEditorViewportClient::IsShowPivotChecked));
    }

    TSharedRef<FEditorViewportClient> SSimpleFlipbookEditorViewport::MakeEditorViewportClient() {
        EditorViewportClient = MakeShared<FSimpleFlipbookEditorViewportClient>(FlipbookBeingEdited);
        return EditorViewportClient.ToSharedRef();
    }

    TSharedPtr<SWidget> SSimpleFlipbookEditorViewport::MakeViewportToolbar() {
        return SNew(SSimpleFlipbookEditorViewportToolbar, SharedThis(this));
    }

    EVisibility SSimpleFlipbookEditorViewport::GetTransformToolbarVisibility() const {
        return EVisibility::Visible;
    }

    void SSimpleFlipbookEditorViewport::OnFocusViewportToSelection() {
        EditorViewportClient->RequestFocusOnSelection(false);
    }

    TSharedRef<SEditorViewport> SSimpleFlipbookEditorViewport::GetViewportWidget() {
        return SharedThis(this);
    }

    TSharedPtr<FExtender> SSimpleFlipbookEditorViewport::GetExtenders() const {
        return MakeShared<FExtender>();
    }

    void SSimpleFlipbookEditorViewport::OnFloatingButtonClicked() {
        // No action
    }
} // namespace Simple2D