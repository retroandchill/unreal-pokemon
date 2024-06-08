// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "IndexedTableRow.h"
#include "PaperSprite.h"

#include "BodyShape.generated.h"

/**
 * Represents the Body Shape of a Pokémon. Used when searching in the Pokédex.
 */
USTRUCT(BlueprintType, meta = (DatabaseType = "Hardcoded"))
struct POKEMONDATA_API FBodyShape : public FIndexedTableRow {
    GENERATED_BODY()

    /**
     * The name that is displayed to the player
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Basic")
    FText RealName;

    /**
     * The sprite used as the Icon for this graphic
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Visuals")
    UPaperSprite *Icon;
};

/**
 * Blueprint function library for getting body shape data out.
 */
UCLASS()
class POKEMONDATA_API UBodyShapeHelper : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
    /**
     * Get the list of all possible body shape names.
     * @return The list of all possible body shape names.
     */
    UFUNCTION()
    static TArray<FName> GetBodyShapeNames();
};