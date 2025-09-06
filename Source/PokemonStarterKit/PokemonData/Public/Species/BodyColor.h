// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "IndexedTableRow.h"

#include "BodyColor.generated.h"

/**
 * Represents the Body Color of a Pokémon. Used when searching in the Pokédex.
 */
USTRUCT(BlueprintType, meta = (DatabaseType = "Hardcoded"))
struct POKEMONDATA_API FBodyColor : public FIndexedTableRow
{
    GENERATED_BODY()

    /**
     * The name that is displayed to the player
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Basic")
    FText RealName;
};

/**
 * Blueprint function library for getting body color data out.
 */
UCLASS()
class POKEMONDATA_API UBodyColorHelper : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

  public:
    /**
     * Get the list of all possible body color names.
     * @return The list of all possible body color names.
     */
    UFUNCTION()
    static TArray<FName> GetBodyColorNames();
};