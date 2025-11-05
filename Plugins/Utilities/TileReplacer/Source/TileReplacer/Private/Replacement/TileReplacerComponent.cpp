// "Unreal Pokémon" created by Retro & Chill.

#include "Replacement/TileReplacerComponent.h"
#include "Engine/World.h"
#include "PaperTileLayer.h"
#include "PaperTileMap.h"
#include "PaperTileMapComponent.h"
#include "PaperTileSet.h"
#include "Replacement/PaperTileReplacement.h"
#include "Replacement/TileReplacement.h"
#include "TileReplacerSettings.h"

#define TRIPLE_LOOP(VarA, SizeA, VarB, SizeB, VarC, SizeC)                                                             \
    for (decltype(SizeA) VarA = 0; VarA < SizeA; VarA++)                                                               \
        for (decltype(SizeB) VarB = 0; VarB < SizeB; VarB++)                                                           \
            for (decltype(SizeC) VarC = 0; VarC < SizeC; VarC++)

// Sets default values for this component's properties
UTileReplacerComponent::UTileReplacerComponent() {
    auto Settings = GetDefault<UTileReplacerSettings>();
    TileReplacementTable = Cast<UDataTable>(Settings->GetTileReplacementsTable().TryLoad());
}

void UTileReplacerComponent::ReplaceTiles(UPaperTileMapComponent *TilemapComponent) {
    if (TileReplacementTable == nullptr) {
        UE_LOG(LogBlueprint, Warning, TEXT("No tile replacement table set for component: %s"), *GetName())
        return;
    }

    int32 SizeX;
    int32 SizeY;
    int32 SizeZ;
    TilemapComponent->GetMapSize(SizeX, SizeY, SizeZ);

    // Since we're already looping so much let's pre-cache all the replacements
    TMap<FPaperTileInfo, FTileReplacement *> TileReplacements;
    TArray<FTileReplacement *> Replacements;
    TileReplacementTable->GetAllRows<FTileReplacement>(TEXT("TileReplacements"), Replacements);
    for (auto Replacement : Replacements) {
        TileReplacements.Add(Replacement->SourceTile, Replacement);
    }

    TRIPLE_LOOP (i, SizeX, j, SizeY, k, SizeZ) {
        auto Tile = TilemapComponent->GetTile(i, j, k);
        if (!Tile.IsValid())
            continue;

        if (!TileReplacements.Contains(Tile))
            continue;

        auto ReplacementData = TileReplacements[Tile];
        auto Metadata = Tile.TileSet->GetTileMetadata(Tile.GetTileIndex());
        auto Actor = GetWorld()->SpawnActor<AActor>(ReplacementData->TileReplacement);
        Actor->AttachToActor(TilemapComponent->GetAttachParentActor(),
                             FAttachmentTransformRules::KeepRelativeTransform);
        IPaperTileReplacement::Execute_ConfigureReplacement(Actor, Tile, *Metadata, ReplacementData->bCollisionEnabled);
        Actor->SetActorRelativeLocation(
            TilemapComponent->GetRelativeLocation() +
            FVector(i * TilemapComponent->TileMap->TileWidth, j * TilemapComponent->TileMap->TileHeight,
                    -k * static_cast<double>(TilemapComponent->TileMap->SeparationPerLayer)));
        Actor->SetActorRelativeRotation(TilemapComponent->GetRelativeRotation());

        ReplacedTiles.Emplace(Tile, i, j, k, Actor);
        Tile.PackedTileIndex = INDEX_NONE;
        TilemapComponent->SetTile(i, j, k, Tile);
    }

    TilemapComponent->RebuildCollision();
}

void UTileReplacerComponent::RestoreCachedTiles(UPaperTileMapComponent *TileMapComponent) {
    if (TileMapComponent == nullptr) {
        UE_LOG(LogBlueprint, Warning, TEXT("No tilemap component found for: %s"), *GetName())
        return;
    }
    for (auto &Replacement : ReplacedTiles) {
        TileMapComponent->SetTile(Replacement.TileX, Replacement.TileY, Replacement.TileLayer,
                                  Replacement.OriginalTileInfo);
        Replacement.Replacement->Destroy();
    }

    ReplacedTiles.Empty();
}
