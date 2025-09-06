// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "EggGroupType.h"
#include "IndexedTableRow.h"

#include "EggGroup.generated.h"

/**
 * Represents an Egg Group for the game
 */
USTRUCT(BlueprintType, meta = (DatabaseType = "Hardcoded"))
struct POKEMONDATA_API FEggGroup : public FIndexedTableRow
{
    GENERATED_BODY()

    /**
     * The name that is displayed to the player
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Basic")
    FText RealName;

    /**
     * What kind of Egg Group is this
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Breeding")
    EEggGroupType Type;
};

/**
 * Blueprint function library for getting egg group data out.
 */
UCLASS()
class POKEMONDATA_API UEggGroupHelper : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

  public:
    /**
     * Get the list of all possible egg group names.
     * @return The list of all possible egg group names.
     */
    UFUNCTION()
    static TArray<FName> GetEggGroupNames();
};