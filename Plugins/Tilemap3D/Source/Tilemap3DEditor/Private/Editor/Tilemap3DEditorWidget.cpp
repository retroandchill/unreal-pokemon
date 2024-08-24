// "Unreal Pokémon" created by Retro & Chill.


#include "Editor/Tilemap3DEditorWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Editor/Tilemap3DEditorViewportWidget.h"

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