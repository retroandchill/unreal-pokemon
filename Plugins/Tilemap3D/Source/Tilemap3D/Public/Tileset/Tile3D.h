// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "Tile3D.generated.h"

USTRUCT(BlueprintType)
struct TILEMAP3D_API FTile3D {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Basic)
    FName TileName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
    TSoftObjectPtr<UStaticMesh> TargetMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Positioning)
    FTransform TileRelativeTransform;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Positioning, meta = (UIMin = 1, ClampMin = 1))
    int32 SizeX = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Positioning, meta = (UIMin = 1, ClampMin = 1))
    int32 SizeY = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Metadata)
    FGameplayTagContainer TerrainTags;
};
