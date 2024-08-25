// "Unreal Pokémon" created by Retro & Chill.


#include "Editor/Tilemap3DEditorWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Editor/Tilemap3DEditorViewportWidget.h"
#include "Editor/TileSelectorWidget.h"
#include "Ranges/Optional/GetPtrOrNull.h"
#include "Ranges/Optional/Map.h"

using FOnSelectionChanged = FOnTileSelectionChanged::FDelegate;

void UTilemap3DEditorWidget::NativeConstruct() {
    Super::NativeConstruct();

    if (!IsValid(TileSelector)) {
        return;
    }
    
    TileSelector->BindToTileSelectionChanged(FOnSelectionChanged::CreateWeakLambda(this,
        [this](TOptional<const FTile3D&> Tile) {
            if (IsValid(Editor)) {
                auto TileMesh = Tile |
                    UE::Optionals::Map(&FTile3D::TargetMesh) |
                    UE::Optionals::Map(&TSoftObjectPtr<UStaticMesh>::LoadSynchronous) |
                    UE::Optionals::GetPtrOrNull;    
                Editor->SetTile(TileMesh);
            }
        }));
}

ATilemap3D * UTilemap3DEditorWidget::GetTilemapActor() const {
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