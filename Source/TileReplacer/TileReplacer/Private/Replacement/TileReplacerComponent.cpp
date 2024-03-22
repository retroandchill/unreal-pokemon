// "Unreal Pokémon" created by Retro & Chill.


#include "Replacement/TileReplacerComponent.h"

#include "PaperTileLayer.h"
#include "PaperTileMapComponent.h"
#include "Replacement/TileReplacement.h"
#include "Asserts.h"
#include "PaperTileMap.h"
#include "PaperTileSet.h"
#include "Replacement/PaperTileReplacement.h"

#define TRIPLE_LOOP(VarA, SizeA, VarB, SizeB, VarC, SizeC) \
	for (decltype(SizeA) VarA = 0; VarA < SizeA; VarA++) \
		for (decltype(SizeB) VarB = 0; VarB < SizeB; VarB++) \
			for (decltype(SizeC) VarC = 0; VarC < SizeC; VarC++)

// Sets default values for this component's properties
UTileReplacerComponent::UTileReplacerComponent() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTileReplacerComponent::ReplaceTiles(UPaperTileMapComponent* TilemapComponent) {
	GUARD_WARN(TileReplacementTable == nullptr, , TEXT("No tile replacement table set for component: %s"), *GetName())
	
	int32 SizeX;
	int32 SizeY;
	int32 SizeZ;
	TilemapComponent->GetMapSize(SizeX, SizeY, SizeZ);

	// Since we're already looping so much let's pre-cache all the replacements
	TMap<FName, FTileReplacement*> TileReplacements;
	for (auto RowNames = TileReplacementTable->GetRowNames(); auto Row : RowNames) {
		TileReplacements.Add(Row, TileReplacementTable->FindRow<FTileReplacement>(Row, TEXT("FindRow")));
	}
	
	TRIPLE_LOOP(i, SizeX, j, SizeY, k, SizeZ) {
		auto Tile = TilemapComponent->GetTile(i, j, k);
		if (!Tile.IsValid())
			continue;
		
		auto Metadata = Tile.TileSet->GetTileMetadata(Tile.GetTileIndex());
		if (!TileReplacements.Contains(Metadata->UserDataName))
			continue;
		
		auto ReplacementData = TileReplacements[Metadata->UserDataName];
		auto Actor = GetWorld()->SpawnActor<AActor>(ReplacementData->TileReplacement);
		Actor->AttachToActor(TilemapComponent->GetAttachParentActor(), FAttachmentTransformRules::KeepRelativeTransform);
		IPaperTileReplacement::Execute_ConfigureReplacement(Actor, Tile, *Metadata, ReplacementData->bCollisionEnabled);
		Actor->SetActorRelativeLocation(TilemapComponent->GetRelativeLocation() + FVector(
			i * TilemapComponent->TileMap->TileWidth,
			j * TilemapComponent->TileMap->TileHeight,
			-k * TilemapComponent->TileMap->SeparationPerLayer));
		Actor->SetActorRelativeRotation(TilemapComponent->GetRelativeRotation());

		ReplacedTiles.Emplace(Tile, i, j, k, Actor);
		Tile.PackedTileIndex = INDEX_NONE;
		TilemapComponent->SetTile(i, j, k, Tile);
	}

	TilemapComponent->RebuildCollision();
}

void UTileReplacerComponent::RestoreCachedTiles(UPaperTileMapComponent* TileMapComponent) {
	GUARD_WARN(TileMapComponent == nullptr, , TEXT("No tilemap component found for: %s"), *GetName())
	for (auto &Replacement : ReplacedTiles) {
		TileMapComponent->SetTile(Replacement.TileX, Replacement.TileY, Replacement.TileLayer, Replacement.OriginalTileInfo);
		Replacement.Replacement->Destroy();
	}

	ReplacedTiles.Empty();
}
