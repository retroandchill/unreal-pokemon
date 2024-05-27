// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "IndexedTableRow.h"

#include "Ability.generated.h"

/**
 * Represents the data for an Ability type in the game
 */
USTRUCT(BlueprintType, meta = (DatabaseType = "PBS"))
struct POKEMONDATA_API FAbility : public FIndexedTableRow {
    GENERATED_BODY()

    /**
     * The name that is displayed to the player
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Basic")
    FText RealName;

    /**
     * Description as displayed to the player
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Basic")
    FText Description;

    /**
     * Words/phrases that can be used to group the data"
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Metadata")
    TArray<FName> Tags;
};

/**
 * Blueprint function library for getting ability data out.
 */
UCLASS()
class POKEMONDATA_API UAbilityHelper : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
    /**
     * Get the list of all possible ability names.
     * @return The list of all possible ability names.
     */
    UFUNCTION()
    static TArray<FName> GetAbilityNames();
};