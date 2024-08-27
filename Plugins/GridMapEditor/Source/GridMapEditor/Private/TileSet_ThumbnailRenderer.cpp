#include "TileSet_ThumbnailRenderer.h"
#include "Engine/StaticMesh.h"
#include "TileSet.h"
#include "ShowFlags.h"
#include "SceneView.h"
#include "Misc/App.h"

void UTileSet_ThumbnailRenderer::Draw(UObject* Object, int32 X, int32 Y, uint32 Width, uint32 Height, FRenderTarget* RenderTarget, FCanvas* Canvas, bool bAdditionalViewFamily)
{
	UGridMapTileSet* TileSet = Cast<UGridMapTileSet>(Object);

	// invalid or empty
	if (TileSet == nullptr || TileSet->Tiles.Num() == 0)
		return;

	FGridMapTileList* TileList = nullptr;
	for (FGridMapTileList& CheckTileList : TileSet->Tiles)
	{
		if (CheckTileList.Tiles.Num() > 0)
		{
			if (TileList == nullptr)
				TileList = &CheckTileList;

			if (CheckTileList.TileAdjacency.Bitset == 0)
			{
				TileList = &CheckTileList;
				break;
			}
		}
	}

	if (TileList == nullptr)
		return;

	TileSet->MeshForIcon = TileList->Tiles[0].LoadSynchronous();
	if (!TileSet->MeshForIcon)
		return;

	Super::Draw(TileSet->MeshForIcon, X, Y, Width, Height, RenderTarget, Canvas, bAdditionalViewFamily);
}
