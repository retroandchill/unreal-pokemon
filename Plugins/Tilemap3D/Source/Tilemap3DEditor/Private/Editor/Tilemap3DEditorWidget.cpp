// "Unreal Pokémon" created by Retro & Chill.


#include "Editor/Tilemap3DEditorWidget.h"
#include "Algo/ForEach.h"
#include "Blueprint/WidgetTree.h"
#include "Editor/TileButtonBase.h"
#include "Editor/Tilemap3DEditorViewportWidget.h"
#include "Tilemap/Tilemap3D.h"

using FTileCallback = FOnTileChanged::FDelegate;

void UTilemap3DEditorWidget::NativePreConstruct() {
    Super::NativePreConstruct();
    if (IsValid(TilemapActor)) {
        TilemapActor->RemoveMapSizeChangedBinding(MapSizeChangedHandle);
        TilemapActor->BindToOnMapSizeChange(FSimpleDelegate::CreateUObject(this,
            &UTilemap3DEditorWidget::UpdateTileButtons));
        
        TilemapActor->RemoveTileChangedBinding(TileChangedHandle);
        TileChangedHandle = TilemapActor->BindToOnTileChange(FTileCallback::CreateUObject(this,
            &UTilemap3DEditorWidget::UpdateTileAppearance));
    }
    
    UpdateTileButtons();
}

ATilemap3D * UTilemap3DEditorWidget::GetTilemapActor() const {
    return TilemapActor;
}

void UTilemap3DEditorWidget::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) {
    Super::PostEditChangeProperty(PropertyChangedEvent);
    if (IsValid(Editor)) {
        Editor->SetTilemap(TilemapActor);
        Editor->SetTile(SelectedTile);
    }
}

void UTilemap3DEditorWidget::UpdateTileButtons() {
    Algo::ForEach(TileButtons, &UWidget::RemoveFromParent);
    TileButtons.Reset();
    if (!IsValid(TileButtonClass) || !IsValid(TilemapActor)) {
        return;
    }
    
    int32 SizeX = TilemapActor->GetSizeX();
    int32 SizeY = TilemapActor->GetSizeY();
    TileButtons.Reserve(SizeX * SizeY);
    for (int32 i = 0; i < SizeX; i++) {
        for (int32 j = 0; j < SizeY; j++) {
            auto Button = WidgetTree->ConstructWidget(TileButtonClass);
            SlotTileButton(Button, i, j);
            Button->OnClicked().AddUObject(this, &UTilemap3DEditorWidget::OnTileButtonClicked, i, j);
            Button->Refresh(TilemapActor, i, j);
            TileButtons.Emplace(Button);
        }
    }
}

void UTilemap3DEditorWidget::UpdateTileAppearance(int32 X, int32 Y, int32 Layer) {
    check(IsValid(TilemapActor))
    int32 TileIndex = X * TilemapActor->GetSizeX() + Y;
    check(TileButtons.IsValidIndex(TileIndex))
    TileButtons[TileIndex]->Refresh(TilemapActor, X, Y, Layer);
}