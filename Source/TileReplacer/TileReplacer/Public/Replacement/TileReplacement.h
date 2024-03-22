// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "TileReplacement.generated.h"

class UPaperTileSet;

/**
 * Struct row for tile replacement data
 */
USTRUCT(BlueprintType)
struct TILEREPLACER_API FTileReplacement : public FTableRowBase {
 GENERATED_BODY()

 UPROPERTY(EditAnywhere, Category = "Tile Replacement")
 TObjectPtr<UPaperTileSet> SourceTileSet;

 UPROPERTY(EditAnywhere, Category = "Tile Replacement", meta = (MustImplement = "PaperTileReplacement"))
 TSubclassOf<AActor> TileReplacement;

 UPROPERTY(EditAnywhere, Category = "Tile Replacement")
 bool bCollisionEnabled = false;
 
};