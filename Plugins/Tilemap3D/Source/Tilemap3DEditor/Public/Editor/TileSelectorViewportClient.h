// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Tilemap3DBaseEditorViewportClient.h"

class UTileset3D;
/**
 *
 */
class TILEMAP3DEDITOR_API FTileSelectorViewportClient : public FTilemap3DBaseEditorViewportClient {

  public:
    explicit FTileSelectorViewportClient(UTileset3D *InTileSet);

    void AddReferencedObjects(FReferenceCollector &Collector) override;

    void SetTileSet(UTileset3D *Tileset3D);

  private:
    TWeakObjectPtr<UTileset3D> Tileset;
    FPreviewScene OwnedPreviewScene;
    TArray<TObjectPtr<UStaticMeshComponent>> TileMeshes;
};
