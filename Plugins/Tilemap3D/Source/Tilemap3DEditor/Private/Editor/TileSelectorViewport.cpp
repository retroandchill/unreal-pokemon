// "Unreal Pokémon" created by Retro & Chill.


#include "Editor/TileSelectorViewport.h"
#include "Editor/TileSelectorViewportClient.h"


void STileSelectorViewport::Construct(const FArguments &InArgs,
    TSharedPtr<FTileSelectorViewportClient> InViewportClient) {
    TypedViewportClient = InViewportClient;

    SEditorViewport::Construct(SEditorViewport::FArguments());

    auto ParentContents = ChildSlot.GetWidget();
    ChildSlot
    [
        SNew(SOverlay)
        +SOverlay::Slot()
        .HAlign(HAlign_Fill)
        .VAlign(VAlign_Fill)
        [
            ParentContents
        ]
    ];
}

TSharedRef<FEditorViewportClient> STileSelectorViewport::MakeEditorViewportClient() {
    return TypedViewportClient.ToSharedRef();
}