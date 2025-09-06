// "Unreal Pokémon" created by Retro & Chill.
#include "Map/TileMapGridBasedMap.h"
#include "Engine/DataTable.h"
#include "GridBased2DSettings.h"
#include "GridUtils.h"
#include "PaperTileMap.h"
#include "Replacement/TileReplacerComponent.h"

// Sets default values
ATileMapGridBasedMap::ATileMapGridBasedMap()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    RootComponent->SetMobility(EComponentMobility::Type::Static);

    TileMapComponent = CreateDefaultSubobject<UPaperTileMapComponent>(TEXT("Map"));
    TileMapComponent->SetRelativeRotation(FRotator(0, 0, -90));
    TileMapComponent->SetupAttachment(RootComponent);

#if WITH_EDITORONLY_DATA
    TileReplacer = CreateDefaultSubobject<UTileReplacerComponent>(TEXT("TileReplacer"));
#endif

    auto Settings = GetDefault<UGridBased2DSettings>();
    TerrainTagDataTable = Cast<UDataTable>(Settings->GetTerrainTagDataTable().TryLoad());
}

#if WITH_EDITOR
void ATileMapGridBasedMap::PostInitProperties()
{
    Super::PostInitProperties();
    SetUpMapLocation();
}

void ATileMapGridBasedMap::PostReinitProperties()
{
    Super::PostReinitProperties();
    SetUpMapLocation();
}

void ATileMapGridBasedMap::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
    SetUpMapLocation();
}

void ATileMapGridBasedMap::PostLoad()
{
    Super::PostLoad();
    SetUpMapLocation();
}

void ATileMapGridBasedMap::PostEditMove(bool bFinished)
{
    Super::PostEditMove(bFinished);
    SetUpMapLocation(bFinished);
}
#endif

void ATileMapGridBasedMap::RefreshTileData()
{
#if WITH_EDITORONLY_DATA
    TileReplacer->RestoreCachedTiles(TileMapComponent);
    TileReplacer->ReplaceTiles(TileMapComponent);
#endif
}

void ATileMapGridBasedMap::ClearTileReplacements()
{

#if WITH_EDITORONLY_DATA
    TileReplacer->RestoreCachedTiles(TileMapComponent);
#endif
}

FIntRect ATileMapGridBasedMap::GetBounds() const
{
    auto RealLocation = GetActorLocation();
    auto GridSize = UGridUtils::GetGridSize(this);
    int32 X = FMath::FloorToInt32(RealLocation.X / GridSize);
    int32 Y = FMath::FloorToInt32(RealLocation.Y / GridSize);
    return FIntRect(X, Y, X + TileMapComponent->TileMap->MapWidth, Y + TileMapComponent->TileMap->MapHeight);
}

void ATileMapGridBasedMap::SetUpMapLocation(bool bFinishedMoving)
{
    const UPaperTileMap *TileMap = TileMapComponent->TileMap;
    if (TileMap == nullptr)
    {
        return;
    }

    FVector MapLocation(0, 0, 0);
    int32 TotalLayers = TileMap->TileLayers.Num();
    PlayerLevelLayer = FMath::Min(PlayerLevelLayer, TotalLayers - 1);
    double LowestLayerZ = static_cast<double>(TotalLayers) * TileMap->SeparationPerLayer;

    MapLocation.Z =
        LowestLayerZ - static_cast<double>(TileMap->SeparationPerLayer) * (TotalLayers - PlayerLevelLayer) - 1;
    TileMapComponent->SetRelativeLocation(MapLocation);

    if (bFinishedMoving)
    {
        auto Position = GetActorLocation();
        Position.Z = 0.0;
        SetActorLocation(Position);
    }
}
