// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "IndexedTableRow.generated.h"

/**
 * 
 */
USTRUCT(BlueprintInternalUseOnly)
struct POKEMONDATA_API FIndexedTableRow : public FTableRowBase {
    GENERATED_BODY()

    /**
     * The internal ID used for lookup by the game
     */
    UPROPERTY(BlueprintReadOnly, Category = "Metadata")
    FName ID;

    /**
     * The number of this row in the data table
     */
    UPROPERTY(BlueprintReadOnly, Category = "Metadata")
    int32 RowNumber;

    void OnDataTableChanged(const UDataTable *InDataTable, const FName InRowName) override;
};
