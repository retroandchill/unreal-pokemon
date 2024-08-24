// "Unreal Pokémon" created by Retro & Chill.


#include "Tilemap/Tilemap3D.h"
#include "Algo/ForEach.h"


FTileRow::FTileRow(int32 Size) {
    Tiles.SetNum(Size);
}

ATilemap3D::ATilemap3D() {
    TilemapRoot = CreateDefaultSubobject<USceneComponent>(TEXT("TilemapRoot"));
    TilemapRoot->SetMobility(EComponentMobility::Type::Static);
    SetRootComponent(TilemapRoot);
}

void ATilemap3D::AddTile(UStaticMesh *Mesh, int32 X, int32 Y, int32 Layer) {
    auto &Tile = GetAdjustedTilePosition(X, Y);
    if (IsValid(Tile)) {
        RemoveOwnedComponent(Tile);
    }
    auto NewComponent = CastChecked<UStaticMeshComponent>(AddComponentByClass(UStaticMeshComponent::StaticClass(),
        false, FTransform(FVector(TileSize.X * X, TileSize.Y * Y, 0)), false));
    NewComponent->CreationMethod = EComponentCreationMethod::Instance;
    AddOwnedComponent(NewComponent);
    NewComponent->SetStaticMesh(Mesh);
    Tile = NewComponent;

    OnTileChanged.Broadcast(X, Y, Layer);
}

void ATilemap3D::RemoveTile(int32 X, int32 Y, int32 Layer) {
    auto &Tile = GetAdjustedTilePosition(X, Y);
    if (IsValid(Tile)) {
        Tile->DestroyComponent(false);
        RemoveOwnedComponent(Tile);
    }
    Tile = nullptr;
    OnTileChanged.Broadcast(X, Y, Layer);
}

void ATilemap3D::OnConstruction(const FTransform &Transform) {
    Super::OnConstruction(Transform);
    for (int32 i = 0; i < TileRows.Num(); i++) {
        if (i >= SizeY) {
            Algo::ForEach(TileRows[i].Tiles, [this](USceneComponent* Component) {
                if (Component == nullptr) {
                    return;
                }
                Component->DestroyComponent(false);
                RemoveOwnedComponent(Component);
            });
        }

        auto &TileArray = TileRows[i].Tiles;
        for (int32 j = SizeX; j < TileArray.Num(); j++) {
            USceneComponent *Tile = TileArray[j];
            if (Tile == nullptr) {
                continue;
            }
            
            Tile->DestroyComponent(false);
            RemoveOwnedComponent(Tile);
        }
    }
    
    TileRows.SetNum(SizeY);
    Algo::ForEach(TileRows, [this](FTileRow& Row) { Row.Tiles.SetNum(SizeX); });
    OnMapSizeChanged.Broadcast();
}

int32 ATilemap3D::GetSizeX() const {
    return SizeX;
}

int32 ATilemap3D::GetSizeY() const {
    return SizeY;
}

USceneComponent * ATilemap3D::GetTile(int32 X, int32 Y) const {
    check(TileRows.IsValidIndex(Y))
    check(TileRows[Y].Tiles.IsValidIndex(X))
    return TileRows[Y].Tiles[X];
}

FDelegateHandle ATilemap3D::BindToOnMapSizeChange(FSimpleMulticastDelegate::FDelegate &&Delegate) {
    return OnMapSizeChanged.Add(MoveTemp(Delegate));
}

void ATilemap3D::RemoveMapSizeChangedBinding(FDelegateHandle Handle) {
    OnMapSizeChanged.Remove(Handle);
}

FDelegateHandle ATilemap3D::BindToOnTileChange(FOnTileChanged::FDelegate &&Delegate) {
    return OnTileChanged.Add(MoveTemp(Delegate));
}

void ATilemap3D::RemoveTileChangedBinding(FDelegateHandle Handle) {
    OnTileChanged.Remove(Handle);
}

TObjectPtr<USceneComponent> & ATilemap3D::GetAdjustedTilePosition(int32 X, int32 Y) {
    check(TileRows.IsValidIndex(Y))
    check(TileRows[Y].Tiles.IsValidIndex(X))
    return TileRows[Y].Tiles[X];
}