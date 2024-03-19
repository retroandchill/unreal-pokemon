// "Unreal Pokémon" created by Retro & Chill.
#include "Map/GridBasedMap.h"

#include "GridUtils.h"
#include "PaperTileMap.h"
#include "Components/AudioComponent.h"
#include "Map/WithinMap.h"

// Sets default values
AGridBasedMap::AGridBasedMap() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	TileMapComponent = CreateDefaultSubobject<UPaperTileMapComponent>(TEXT("Map"));
	TileMapComponent->SetRelativeRotation(FRotator(0, 0, -90));
	TileMapComponent->SetupAttachment(RootComponent);

	TopBounds = CreateDefaultSubobject<UBoxComponent>(TEXT("TopBounds"));
	TopBounds->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	TopBounds->SetupAttachment(TileMapComponent);

	BottomBounds = CreateDefaultSubobject<UBoxComponent>(TEXT("BottomBounds"));
	BottomBounds->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	BottomBounds->SetupAttachment(TileMapComponent);

	LeftBounds = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftBounds"));
	LeftBounds->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	LeftBounds->SetupAttachment(TileMapComponent);

	RightBounds = CreateDefaultSubobject<UBoxComponent>(TEXT("RightBounds"));
	RightBounds->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	RightBounds->SetupAttachment(TileMapComponent);
	SetBoundsPositions(true);
	
}

void AGridBasedMap::PostInitProperties() {
	Super::PostInitProperties();
	SetUpMapLocation();
}

void AGridBasedMap::PostReinitProperties() {
	Super::PostReinitProperties();
	SetUpMapLocation();
}

void AGridBasedMap::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);
	SetUpMapLocation();
}

void AGridBasedMap::PostLoad() {
	Super::PostLoad();
	SetUpMapLocation();
}

void AGridBasedMap::PostEditMove(bool bFinished) {
	Super::PostEditMove(bFinished);
	SetUpMapLocation(bFinished);
}

FIntRect AGridBasedMap::GetBounds() const {
	auto RealLocation = GetActorLocation();
	int32 X = FMath::FloorToInt32(RealLocation.X / GridBased2D::GRID_SIZE);
	int32 Y = FMath::FloorToInt32(RealLocation.Y / GridBased2D::GRID_SIZE);
	return FIntRect(X, Y, X + TileMap->MapWidth, Y + TileMap->MapHeight);
}

bool AGridBasedMap::IsObjectInMap(const IWithinMap& Object) const {
	auto Position = Object.GetCurrentPosition();
	return GetBounds().Contains({Position.X, Position.Y});
}

void AGridBasedMap::SetUpMapLocation(bool bFinishedMoving) {
	if (TileMap == nullptr)
		return;

	TileMapComponent->SetTileMap(TileMap);

	FVector MapLocation(0, 0, 0);
	int32 TotalLayers = TileMap->TileLayers.Num();
	PlayerLevelLayer = FMath::Min(PlayerLevelLayer, TotalLayers - 1);
	double LowestLayerZ = static_cast<double>(TotalLayers) * TileMap->SeparationPerLayer;

	MapLocation.Z = LowestLayerZ - static_cast<double>(TileMap->SeparationPerLayer) * (TotalLayers - PlayerLevelLayer) -
		1;
	TileMapComponent->SetRelativeLocation(MapLocation);
	SetBoundsPositions();

	if (bFinishedMoving) {
		auto Position = GetActorLocation();
		Position.Z = 0.0;
		SetActorLocation(Position);
	}
}

void AGridBasedMap::SetBoundsPositions(bool) {
	int32 TileWidth = TileMapComponent->TileMap->TileWidth;
	int32 TileHeight = TileMapComponent->TileMap->TileHeight;
	int32 TotalMapWidth = TileWidth * TileMapComponent->TileMap->MapWidth;
	int32 TotalMapHeight = TileHeight * TileMapComponent->TileMap->MapHeight;
	int32 BoxHeight = FMath::Max(TileWidth, TileHeight) / 2;

	FVector TopLeft(-TileWidth / 2, 0, TileHeight / 2);

	auto TopBoundsPosition = TopLeft + FVector(TotalMapWidth / 2, 0, TileHeight / 2);
	TopBounds->SetRelativeLocation(TopBoundsPosition);
	TopBounds->SetBoxExtent(FVector(TotalMapWidth / 2, BoxHeight, TileHeight / 2));

	auto BottomBoundsPosition = TopLeft + FVector(TotalMapWidth / 2, 0, -TotalMapHeight - TileHeight / 2);
	BottomBounds->SetRelativeLocation(BottomBoundsPosition);
	BottomBounds->SetBoxExtent(FVector(TotalMapWidth / 2, BoxHeight, TileHeight / 2));

	auto LeftBoundPosition = TopLeft + FVector(-TileWidth / 2, 0, -TotalMapHeight / 2);
	LeftBounds->SetRelativeLocation(LeftBoundPosition);
	LeftBounds->SetBoxExtent(FVector(TileWidth / 2, BoxHeight, TotalMapHeight / 2));

	auto RightBoundPosition = TopLeft + FVector(TotalMapWidth + TileWidth / 2, 0, -TotalMapHeight / 2);
	RightBounds->SetRelativeLocation(RightBoundPosition);
	RightBounds->SetBoxExtent(FVector(TileWidth / 2, BoxHeight, TotalMapHeight / 2));
}
