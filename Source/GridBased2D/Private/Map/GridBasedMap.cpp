//====================================================================================================================
// ** Unreal Pokémon created by Retro & Chill
//--------------------------------------------------------------------------------------------------------------------
// This project is intended as a means of learning more about how a game like Pokémon works by creating a framework
// from the ground up, and for non-commercial applications. While this code is original, Pokémon is the intellectual
// property of Game Freak and Nintendo, as such it is highly discouraged to use this kit to make a commercial product.
//--------------------------------------------------------------------------------------------------------------------
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//====================================================================================================================
#include "Map/GridBasedMap.h"

#include "PaperTileMap.h"

// Sets default values
AGridBasedMap::AGridBasedMap() : PlayerLevelLayer(1) {
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

// Called when the game starts or when spawned
void AGridBasedMap::BeginPlay() {
	Super::BeginPlay();
	
}

// Called every frame
void AGridBasedMap::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AGridBasedMap::SetUpMapLocation(bool bFinishedMoving) {
	if (TileMap == nullptr)
		return;
	
	TileMapComponent->SetTileMap(TileMap);
	
	FVector MapLocation(0, 0, 0);
	int32 TotalLayers = TileMap->TileLayers.Num();
	PlayerLevelLayer = FMath::Min(PlayerLevelLayer, TotalLayers - 1);
	float LowestLayerZ = TotalLayers * TileMap->SeparationPerLayer;

	MapLocation.Z = LowestLayerZ - TileMap->SeparationPerLayer * (TotalLayers - PlayerLevelLayer) - 1;
	TileMapComponent->SetRelativeLocation(MapLocation);
	SetBoundsPositions();

	if (bFinishedMoving) {
		auto Position = GetActorLocation();
		Position.Z = 0.0;
		SetActorLocation(Position);
	}
}

void AGridBasedMap::SetBoundsPositions(bool bIsInitializing) {
	int32 TileWidth = TileMapComponent->TileMap->TileWidth;
	int32 TileHeight = TileMapComponent->TileMap->TileHeight;
	int32 TotalMapWidth = TileWidth * TileMapComponent->TileMap->MapWidth;
	int32 TotalMapHeight = TileHeight * TileMapComponent->TileMap->MapHeight;
	int32 BoxHeight = FMath::Max(TileWidth, TileHeight) / 2;
	
	FVector TopLeft(-TileWidth / 2, 0,TileHeight / 2);

	auto TopBoundsPosition = TopLeft + FVector(TotalMapWidth / 2, 0,TileHeight / 2);
	TopBounds->SetRelativeLocation(TopBoundsPosition);
	TopBounds->SetBoxExtent(FVector(TotalMapWidth / 2, BoxHeight, TileHeight / 2));

	auto BottomBoundsPosition = TopLeft + FVector(TotalMapWidth / 2, 0, -TotalMapHeight - TileHeight / 2);
	BottomBounds->SetRelativeLocation(BottomBoundsPosition);
	BottomBounds->SetBoxExtent(FVector(TotalMapWidth / 2, BoxHeight, TileHeight / 2));

	auto LeftBoundPosition = TopLeft + FVector(-TileWidth / 2, 0,-TotalMapHeight / 2);
	LeftBounds->SetRelativeLocation(LeftBoundPosition);
	LeftBounds->SetBoxExtent(FVector(TileWidth / 2, BoxHeight, TotalMapHeight / 2));

	auto RightBoundPosition = TopLeft + FVector(TotalMapWidth + TileWidth / 2, 0, -TotalMapHeight / 2);
	RightBounds->SetRelativeLocation(RightBoundPosition);
	RightBounds->SetBoxExtent(FVector(TileWidth / 2, BoxHeight, TotalMapHeight / 2));
}

