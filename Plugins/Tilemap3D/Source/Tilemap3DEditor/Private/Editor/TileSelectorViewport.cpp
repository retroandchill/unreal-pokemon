// "Unreal Pokémon" created by Retro & Chill.

#include "Editor/TileSelectorViewport.h"
#include "Editor/TileSelectorViewportClient.h"

void STileSelectorViewport::Construct(const FArguments &InArgs,
                                      TSharedPtr<FTileSelectorViewportClient> InViewportClient) {
    TypedViewportClient = InViewportClient;

    SEditorViewport::Construct(SEditorViewport::FArguments());
}

TSharedRef<FEditorViewportClient> STileSelectorViewport::MakeEditorViewportClient() {
    return TypedViewportClient.ToSharedRef();
}

FReply STileSelectorViewport::OnMouseButtonDown(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent) {
    return SEditorViewport::OnMouseButtonDown(MyGeometry, MouseEvent);
}

FReply STileSelectorViewport::OnMouseButtonUp(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent) {
    return SEditorViewport::OnMouseButtonUp(MyGeometry, MouseEvent);
}

FReply STileSelectorViewport::OnMouseMove(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent) {
    return SEditorViewport::OnMouseMove(MyGeometry, MouseEvent);
}

FReply STileSelectorViewport::OnMouseWheel(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent) {
    return SEditorViewport::OnMouseWheel(MyGeometry, MouseEvent);
}

FCursorReply STileSelectorViewport::OnCursorQuery(const FGeometry &MyGeometry, const FPointerEvent &CursorEvent) const {
    return SEditorViewport::OnCursorQuery(MyGeometry, CursorEvent);
}