// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GridMapEditorMode.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "GridMapEditorUtilities.generated.h"

/**
 *
 */
UCLASS()
class GRIDMAPEDITOR_API UGridMapEditorUtilities : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

  public:
    UFUNCTION(BlueprintCallable, Category = GridMap)
    static void CreateAutoTileLayout(UGridMapTileSet *TileSet, const TArray<UStaticMesh *> &Tiles);
};
