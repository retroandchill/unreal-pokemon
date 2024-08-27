// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Tilemap3DBaseEditorViewportClient.h"

class UTileset3D;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnPreviewTileChanged, int32, int32);

/**
 * 
 */
class TILEMAP3DEDITOR_API FTile3DEditorViewportClient  : public FTilemap3DBaseEditorViewportClient {
public:
	explicit FTile3DEditorViewportClient(UTileset3D* InTileSet);

	// FViewportClient interface
	virtual void Tick(float DeltaSeconds) override;
	// End of FViewportClient interface

	virtual FLinearColor GetBackgroundColor() const override;
	virtual void DrawCanvas(FViewport& InViewport, FSceneView& View, FCanvas& Canvas) override;

    void SetTileSet(UTileset3D* Tileset3D);
	void SetTileIndex(int32 InTileIndex);
	int32 GetTileIndex() const;

	// Delegate for when the index of the single tile being edited changes
	FOnPreviewTileChanged& GetOnSingleTileIndexChanged() {
		return OnSingleTileIndexChanged;
	}

protected:
	FBox GetDesiredFocusBounds() const override;

private:
	// Tile set
	UTileset3D* TileSet;

	// The current tile being edited or INDEX_NONE
	int32 TileBeingEditedIndex = INDEX_NONE;

	// Should we show stats for the tile?
	bool bShowStats = false;

	// Pointer back to the sprite editor viewport control that owns us
	TWeakPtr<SEditorViewport> SpriteEditorViewportPtr;

	// The current transaction for undo/redo
	TUniquePtr<FScopedTransaction> ScopedTransaction;

	// The preview scene
	FPreviewScene OwnedPreviewScene;

	// The preview sprite in the scene
	UStaticMeshComponent* PreviewTileSpriteComponent;

	// Called when TileBeingEditedIndex changes
	FOnPreviewTileChanged OnSingleTileIndexChanged;
};
