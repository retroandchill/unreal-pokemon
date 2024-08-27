// "Unreal Pokémon" created by Retro & Chill.


#include "Editor/TileSelector.h"
#include "IStructureDetailsView.h"

#include "SlateOptMacros.h"
#include "Editor/Tile3DEditorViewport.h"
#include "Editor/Tile3DEditorViewportClient.h"
#include "Editor/TileSelectorViewport.h"
#include "Editor/TileSelectorViewportClient.h"
#include "Tileset/Tileset3D.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void STileSelector::Construct(const FArguments & InArgs) {
    OnSelectedTileChanged_Handler = InArgs._OnSelectedTileChanged;

    auto ComboBoxGenerateWidget = [this](FName Item) {
        return SNew(STextBlock)
            .Text(FText::FromString(*Item.ToString()));
    };

    auto PlatformComboBoxContentText = [this]() -> FText {
        FName SelectedTile = TileComboBox->GetSelectedItem();
        return SelectedTile.IsValid()
                   ? FText::FromString(*SelectedTile.ToString())
                   : NSLOCTEXT("Tilemap3D", "SelectATile", "Select a Tile");
    };

    SelectorClient = MakeShared<FTileSelectorViewportClient>(Tileset.Get());
    PreviewClient = MakeShared<FTile3DEditorViewportClient>(Tileset.Get());

    ChildSlot
    [
        SNew(SVerticalBox)
        + SVerticalBox::Slot()
		.VAlign(VAlign_Top)
        .HAlign(HAlign_Fill)
        .AutoHeight()
        [
            SAssignNew(TileComboBox, SComboBox<FName>)
            .ContentPadding(FMargin(6.0f, 2.0f))
            .OptionsSource(&TileOptions)
            .OnGenerateWidget_Lambda(ComboBoxGenerateWidget)
            .OnSelectionChanged_Raw(this, &STileSelector::OnTileSelectionChanged)
            [
                SNew(STextBlock)
                    .Text_Lambda(PlatformComboBoxContentText)
            ]
        ]
        + SVerticalBox::Slot()
        .VAlign(VAlign_Top)
        .HAlign(HAlign_Fill)
        .FillHeight(1.f)
        [
            SAssignNew(PreviewWidget, STile3DEditorViewport, PreviewClient)
        ]
        + SVerticalBox::Slot()
        .VAlign(VAlign_Top)
        .HAlign(HAlign_Fill)
        .FillHeight(1.f)
        [
            SAssignNew(SelectorViewport, STileSelectorViewport, SelectorClient)
        ]
    ];
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

void STileSelector::SetTileset(UTileset3D *Tileset3D) {
    Tileset = Tileset3D;
    TileOptions = Tileset3D != nullptr ? Tileset3D->GetTileNames() : TArray<FName>();
    TileComboBox->RefreshOptions();
    if (!TileOptions.Contains(TileComboBox->GetSelectedItem())) {
        TileComboBox->SetSelectedItem(NAME_None);
    }
    SelectorClient->SetTileSet(Tileset3D);
    PreviewClient->SetTileSet(Tileset3D);

    if (Tileset3D != nullptr) {
        auto Tiles = Tileset3D->GetTileNames();
        auto TileIndex = Tiles.Find(TileComboBox->GetSelectedItem());
        PreviewClient->SetTileIndex(TileIndex);
    }
}

void STileSelector::OnTileSelectionChanged(FName Item, ESelectInfo::Type) const {
    if (!Tileset.IsValid()) {
        OnSelectedTileChanged_Handler.ExecuteIfBound(FTileHandle());
        return;
    }
    
    int32 Index = TileOptions.Find(Item);
    check(TileOptions.IsValidIndex(Index))
    PreviewClient->SetTileIndex(Index);
    OnSelectedTileChanged_Handler.ExecuteIfBound(FTileHandle(*Tileset, Index));
}