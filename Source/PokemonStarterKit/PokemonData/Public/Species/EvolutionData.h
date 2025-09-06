// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "IndexedTableRow.h"
#include "UObject/Object.h"

#include "EvolutionData.generated.h"

/**
 * The data related with how a Pokémon evolves
 */
USTRUCT(BlueprintType, meta = (DatabaseType = "Hardcoded"))
struct POKEMONDATA_API FEvolutionData : public FIndexedTableRow
{
    GENERATED_BODY()

    /**
     * The name that is displayed to the player
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Basic")
    FText RealName;
};

/**
 * Blueprint function library for getting evolution data out.
 */
UCLASS()
class POKEMONDATA_API UEvolutionHelper : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

  public:
    /**
     * Get the list of all possible evolution names.
     * @return The list of all possible evolution names.
     */
    UFUNCTION()
    static TArray<FName> GetEvolutionNames();
};