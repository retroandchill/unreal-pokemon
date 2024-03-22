// "Unreal Pokémon" created by Retro & Chill.


#include "Replacement/TileReplacerComponent.h"

#include "PaperTileLayer.h"
#include "PaperTileMapComponent.h"
#include "Replacement/TileReplacement.h"
#include "Asserts.h"
#include "PaperTileMap.h"
#include "Replacement/TileReplacementActor.h"

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

void UTileReplacerComponent::ReplaceTiles(UPaperTileMapComponent* TilemapComponent) const {
	GUARD_WARN(TileReplacementClass == nullptr, , TEXT("No tile replacement class set for component: %s"), *GetName())
	
	int32 SizeX;
	int32 SizeY;
	int32 SizeZ;
	TilemapComponent->GetMapSize(SizeX, SizeY, SizeZ);

	// Since we're already looping so much let's pre-cache all the replacements
	TMap<UPaperTileSet*, FTileReplacement*> TileReplacements;
	TArray<FTileReplacement*> DataRows;
	TileReplacementTable->GetAllRows(TEXT("TileReplacement"), DataRows);
	for (auto Replacement : DataRows) {
		TileReplacements.Add(Replacement->SourceTileSet, Replacement);
	}
	
	TRIPLE_LOOP(i, SizeX, j, SizeY, k, SizeZ) {
		auto Tile = TilemapComponent->GetTile(i, j, k);
		if (!TileReplacements.Contains(Tile.TileSet))
			continue;

		Tile.PackedTileIndex = INDEX_NONE;
		TilemapComponent->SetTile(i, j, k, Tile);

		auto ReplacementData = TileReplacements[Tile.TileSet];
		auto Actor = GetWorld()->SpawnActor<ATileReplacementActor>(TileReplacementClass);
		Actor->AttachToActor(TilemapComponent->GetAttachParentActor(), FAttachmentTransformRules::KeepRelativeTransform);
		Actor->SetReplacementMaterial(ReplacementData->ReplacementMaterial, ReplacementData->bCollisionEnabled);
		Actor->SetActorRelativeLocation(FVector(
			i * TilemapComponent->TileMap->TileWidth,
			j * TilemapComponent->TileMap->TileHeight,
			TilemapComponent->GetRelativeLocation().Z - k * TilemapComponent->TileMap->SeparationPerLayer));
	}

	TilemapComponent->RebuildCollision();
}


// Called when the game starts
void UTileReplacerComponent::BeginPlay() {
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTileReplacerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

