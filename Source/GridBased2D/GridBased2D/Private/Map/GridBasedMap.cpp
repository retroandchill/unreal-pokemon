// "Unreal Pokémon" created by Retro & Chill.
#include "Map/GridBasedMap.h"

#include "Asserts.h"
#include "GridUtils.h"
#include "PaperTileMap.h"
#include "Characters/GameCharacter.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Map/MapAudioUtilities.h"
#include "Map/MapSubsystem.h"
#include "Map/WithinMap.h"
#include "Replacement/TileReplacerComponent.h"

// Sets default values
AGridBasedMap::AGridBasedMap() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	TileMapComponent = CreateDefaultSubobject<UPaperTileMapComponent>(TEXT("Map"));
	TileMapComponent->SetRelativeRotation(FRotator(0, 0, -90));
	TileMapComponent->SetupAttachment(RootComponent);

#if WITH_EDITORONLY_DATA
	TileReplacer = CreateDefaultSubobject<UTileReplacerComponent>(TEXT("TileReplacer"));
#endif
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

void AGridBasedMap::BeginPlay() {
	Super::BeginPlay();
	
	if (auto Player = Cast<AGameCharacter>(
		UGameplayStatics::GetPlayerCharacter(this, 0));
		Player != nullptr && IsObjectInMap(Player)) {
		OnPlayerEnter();
	}

	auto InitialCharacters = UGridUtils::FindAllActors<AGameCharacter>(this);
	Characters.Empty();
	for (auto Char : InitialCharacters) {
		Characters.Emplace(Char);
	}
}

#if WITH_EDITORONLY_DATA
void AGridBasedMap::RefreshTileData() {
	TileReplacer->RestoreCachedTiles(TileMapComponent);
	TileReplacer->ReplaceTiles(TileMapComponent);
}

void AGridBasedMap::ClearTileReplacements() {
	TileReplacer->RestoreCachedTiles(TileMapComponent);
}
#endif

FIntRect AGridBasedMap::GetBounds() const {
	auto RealLocation = GetActorLocation();
	int32 X = FMath::FloorToInt32(RealLocation.X / UGridUtils::GRID_SIZE);
	int32 Y = FMath::FloorToInt32(RealLocation.Y / UGridUtils::GRID_SIZE);
	return FIntRect(X, Y, X + TileMapComponent->TileMap->MapWidth, Y + TileMapComponent->TileMap->MapHeight);
}

bool AGridBasedMap::IsObjectInMap(const IWithinMap* Object) const {
	return IsPositionInMap(Object->GetCurrentPosition());
}

bool AGridBasedMap::IsPositionInMap(const FIntVector2& Position) const {
	return GetBounds().Contains({Position.X, Position.Y});
}

bool AGridBasedMap::IsCharacterPartOfMap(const AGameCharacter* Character) const {
	return Characters.Contains(Character);
}

void AGridBasedMap::AddCharacter(AGameCharacter* Character) {
	Characters.Emplace(Character);
	Character->OnMapChanged(this);
}

void AGridBasedMap::RemoveCharacter(AGameCharacter* Character) {
	Characters.Remove(Character);
}

void AGridBasedMap::OnPlayerEnter() {
	UMapAudioUtilities::PlayBackgroundMusic(this, BackgroundMusic);
}

void AGridBasedMap::SetUpMapLocation(bool bFinishedMoving) {
	UPaperTileMap* TileMap = TileMapComponent->TileMap;
	GUARD(TileMap == nullptr, )

	FVector MapLocation(0, 0, 0);
	int32 TotalLayers = TileMap->TileLayers.Num();
	PlayerLevelLayer = FMath::Min(PlayerLevelLayer, TotalLayers - 1);
	double LowestLayerZ = static_cast<double>(TotalLayers) * TileMap->SeparationPerLayer;

	MapLocation.Z = LowestLayerZ - static_cast<double>(TileMap->SeparationPerLayer) * (TotalLayers - PlayerLevelLayer) -
		1;
	TileMapComponent->SetRelativeLocation(MapLocation);

	if (bFinishedMoving) {
		auto Position = GetActorLocation();
		Position.Z = 0.0;
		SetActorLocation(Position);
	}
}
