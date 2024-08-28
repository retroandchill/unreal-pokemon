// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "GridMapTile.generated.h"

class UGridMapTileSet;

USTRUCT()
struct GRIDMAP_API FGridTileHandle {
    GENERATED_BODY()

    UPROPERTY()
    TObjectPtr<UGridMapTileSet> TileSet;

    UPROPERTY()
    int32 TileIndex;

    bool IsValidTile() const;

    FGameplayTagContainer GetTileTags() const;
};

// This class does not need to be modified.
UINTERFACE()
class UGridMapTile : public UInterface {
    GENERATED_BODY()
};

/**
 * 
 */
class GRIDMAP_API IGridMapTile {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    virtual const FGridTileHandle& GetGridTile() const = 0;

    virtual void SetGridTile(const FGridTileHandle& InGridTile) = 0;

    virtual int32 GetLayer() const = 0;

    virtual void SetLayer(int32 Layer) = 0;

};