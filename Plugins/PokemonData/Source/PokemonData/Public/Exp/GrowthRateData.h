// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "IndexedTableRow.h"
#include "UObject/Object.h"

#include "GrowthRateData.generated.h"

/**
 * Represents the data row used to retrieve the exp growth curves
 */
USTRUCT(BlueprintType, meta = (DatabaseType = "Hardcoded"))
struct POKEMONDATA_API FGrowthRateData : public FIndexedTableRow {
    GENERATED_BODY()

    /**
     * The name that is displayed to the player
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Basic")
    FText RealName;
};

/**
 * Blueprint function library for getting growth rate data out.
 */
UCLASS()
class POKEMONDATA_API UGrowthRateHelper : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
    /**
     * Get the list of all possible growth rate names.
     * @return The list of all possible growth rate names.
     */
    UFUNCTION()
    static TArray<FName> GetGrowthRateNames();
};
