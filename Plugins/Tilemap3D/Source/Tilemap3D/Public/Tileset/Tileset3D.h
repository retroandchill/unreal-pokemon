// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Tile3D.h"
#include "UObject/Object.h"
#include "Tileset3D.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class TILEMAP3D_API UTileset3D : public UObject {
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Tileset)
    const TArray<FTile3D> &GetTiles() const;

    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Tileset)
    TArray<FName> GetTileNames() const;

private:
    UPROPERTY(EditAnywhere, BlueprintGetter = GetTiles, Category = Tileset)
    TArray<FTile3D> Tiles;
    
};