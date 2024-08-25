// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ranges/Optional/OptionalRef.h"
#include "Tilemap3D.generated.h"

struct FTile3D;
class UTileset3D;

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnTileChanged, int32, int32, int32);

USTRUCT()
struct TILEMAP3D_API FTileHandle {
    GENERATED_BODY()

    FTileHandle() = default;

    FTileHandle(UTileset3D& InTileset, int32 InTileID);

    TOptional<const FTile3D&> GetTile() const;

    bool IsValidTile() const;
    
    bool operator==(const FTileHandle& Other) const;
    
private:
    UPROPERTY()
    TObjectPtr<UTileset3D> Tileset;

    UPROPERTY()
    int32 TileID = INDEX_NONE;
};

USTRUCT()
struct TILEMAP3D_API FTileInfo {
    GENERATED_BODY()

    FTileInfo() = default;

    FTileInfo(UStaticMeshComponent& InTileMesh, const FTileHandle& InTile, const FIntVector2 &InTileOrigin);
    
    bool IsValidTile() const;

    TOptional<const FTile3D&> GetTile() const;

    const FTileHandle& GetTileHandle() const;

    UStaticMeshComponent* GetMeshComponent() const;

    const FIntVector2& GetTileOrigin() const;

private:
    UPROPERTY()
    TObjectPtr<UStaticMeshComponent> TileMesh;

    UPROPERTY()
    FTileHandle Tile;

    UPROPERTY()
    FIntVector2 TileOrigin;
};

USTRUCT()
struct TILEMAP3D_API FTileRow {
    GENERATED_BODY()

    UPROPERTY()
    TArray<FTileInfo> Tiles;

    FTileRow() = default;

    explicit FTileRow(int32 Size);
};

UCLASS()
class TILEMAP3D_API ATilemap3D : public AActor {
    GENERATED_BODY()

public:
    ATilemap3D();
    
    void AddTile(const FTileHandle& Tile, int32 X, int32 Y, int32 Layer);
    void RemoveTile(int32 X, int32 Y, int32 Layer);

    void OnConstruction(const FTransform &Transform) override;

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Grid)
    int32 GetSizeX() const;

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Grid)
    int32 GetSizeY() const;

    const FTileInfo &GetTile(int32 X, int32 Y) const;

    FDelegateHandle BindToOnMapSizeChange(FSimpleMulticastDelegate::FDelegate&& Delegate);
    void RemoveMapSizeChangedBinding(FDelegateHandle Handle);

    FDelegateHandle BindToOnTileChange(FOnTileChanged::FDelegate&& Delegate);
    void RemoveTileChangedBinding(FDelegateHandle Handle);

private:
    FTileInfo& GetAdjustedTilePosition(int32 X, int32 Y);

    void FillInTile(const FTile3D& Tile, const FTileInfo& OriginTile, int32 X, int32 Y, int32 Layer);
    void ClearOutTile(const FTile3D& Tile, int32 X, int32 Y, int32 Layer);
    
    UPROPERTY(EditAnywhere, BlueprintGetter = GetSizeX, Category = Grid, meta = (UIMin = 1, ClampMin = 1))
    int32 SizeX = 10;

    UPROPERTY(EditAnywhere, BlueprintGetter = GetSizeY, Category = Grid, meta = (UIMin = 1, ClampMin = 1))
    int32 SizeY = 10;

    UPROPERTY(EditAnywhere, Category = Grid, meta = (UIMin = 1, ClampMin = 1))
    FVector TileSize = FVector(16);

    UPROPERTY()
    TObjectPtr<USceneComponent> TilemapRoot;
    
    UPROPERTY()
    TArray<FTileRow> TileRows;

    FSimpleMulticastDelegate OnMapSizeChanged;
    FOnTileChanged OnTileChanged;

};