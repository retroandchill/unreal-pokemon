// "Unreal Pokémon" created by Retro & Chill.

#include "Editor/Tilemap3DEditorWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Editor/Tilemap3DEditorViewportWidget.h"
#include "Editor/TileSelectorWidget.h"
#include "Ranges/Optional/GetPtrOrNull.h"
#include "Ranges/Optional/Map.h"
#include "Ranges/Optional/OrElse.h"

using FOnSelectionChanged = FOnTileSelectionChanged::FDelegate;

void UTilemap3DEditorWidget::NativeConstruct() {
    Super::NativeConstruct();

    if (!IsValid(TileSelector)) {
        return;
    }

    TileSelector->BindToTileSelectionChanged(
        FOnSelectionChanged::CreateWeakLambda(this, [this](const FTileHandle &Tile) {
            if (IsValid(Editor)) {
                Editor->SetTile(Tile);
            }
        }));
}

ATilemap3D *UTilemap3DEditorWidget::GetTilemapActor() const {
    return TilemapActor;
}

void UTilemap3DEditorWidget::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) {
    Super::PostEditChangeProperty(PropertyChangedEvent);
    if (IsValid(Editor)) {
        Editor->SetTilemap(TilemapActor);
    }

    if (IsValid(TileSelector)) {
        TileSelector->SetTileset(Tileset);
    }
}