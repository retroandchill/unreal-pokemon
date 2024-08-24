// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tilemap3D.generated.h"

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnTileChanged, int32, int32, int32);

USTRUCT()
struct TILEMAP3D_API FTileRow {
    GENERATED_BODY()

    UPROPERTY()
    TArray<TObjectPtr<USceneComponent>> Tiles;

    FTileRow() = default;

    explicit FTileRow(int32 Size);
};

UCLASS()
class TILEMAP3D_API ATilemap3D : public AActor {
    GENERATED_BODY()

public:
    ATilemap3D();
    
    UFUNCTION(BlueprintCallable, Category = Tilemap)
    void AddTile(UStaticMesh* Mesh, int32 X, int32 Y, int32 Layer);

    UFUNCTION(BlueprintCallable, Category = Tilemap)
    void RemoveTile(int32 X, int32 Y, int32 Layer);

    void OnConstruction(const FTransform &Transform) override;

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Grid)
    int32 GetSizeX() const;

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Grid)
    int32 GetSizeY() const;

    UFUNCTION(BlueprintPure, Category = Grid)
    USceneComponent* GetTile(int32 X, int32 Y) const;

    FDelegateHandle BindToOnMapSizeChange(FSimpleMulticastDelegate::FDelegate&& Delegate);
    void RemoveMapSizeChangedBinding(FDelegateHandle Handle);

    FDelegateHandle BindToOnTileChange(FOnTileChanged::FDelegate&& Delegate);
    void RemoveTileChangedBinding(FDelegateHandle Handle);

private:
    TObjectPtr<USceneComponent>& GetAdjustedTilePosition(int32 X, int32 Y);
    
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