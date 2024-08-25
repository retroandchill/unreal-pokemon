// "Unreal Pokémon" created by Retro & Chill.


#include "Editor/Tilemap3DEditorViewportWidget.h"
#include "Editor/Tilemap3DEditorViewport.h"

TSharedRef<SWidget> UTilemap3DEditorViewportWidget::RebuildWidget() {
    SAssignNew(TilemapWidget, STilemap3DEditorViewport);
    return TilemapWidget.ToSharedRef();
}

void UTilemap3DEditorViewportWidget::SynchronizeProperties() {
    Super::SynchronizeProperties();
    TilemapWidget->SetTilemap(TilemapActor);
}

void UTilemap3DEditorViewportWidget::ReleaseSlateResources(bool bReleaseChildren) {
    Super::ReleaseSlateResources(bReleaseChildren);
    TilemapWidget.Reset();
}

void UTilemap3DEditorViewportWidget::SetTilemap(ATilemap3D *Tilemap3D) {
    TilemapActor = Tilemap3D;
    if (TilemapWidget != nullptr) {
        TilemapWidget->SetTilemap(TilemapActor);
    }
}

void UTilemap3DEditorViewportWidget::SetTile(UStaticMesh *Mesh) {
    TileMesh = Mesh;
    if (TilemapWidget != nullptr) {
        TilemapWidget->SetMesh(TileMesh);
    }
}