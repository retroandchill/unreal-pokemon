// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "IndexedTableRow.h"
#include "PokemonStatType.h"
#include "UObject/Object.h"

#include "Stat.generated.h"

/**
 * Represents one of the stats in the database.
 */
USTRUCT(BlueprintType, meta = (DatabaseType = "Hardcoded"))
struct POKEMONDATA_API FStat : public FIndexedTableRow {
    GENERATED_BODY()

    /**
     * The name that is displayed to the player
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Basic")
    FText RealName;

    /**
     * The name that is displayed to the player when there is a limited space
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Basic")
    FText RealNameBrief;

    /**
     * What kind of stat is this?
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Stats")
    EPokemonStatType Type;

    /**
     * The order this stat is processed when parsing the PBS files
     */
    UPROPERTY(DisplayName = "PBS Order", BlueprintReadOnly, EditAnywhere, Category = "Metadata",
              meta = (EditCondition = "Type != EPokemonStatType::Battle", UIMin = 0, ClampMin = 0))
    int32 PbsOrder;

    /**
     * The field used to initialize attributes during battle. This is also what updates the base values during a
     * level-up or a form change.
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Metadata", meta = (EditCondition = "Type != EPokemonStatType::Battle"))
    FGameplayAttribute BaseAttribute;

    /**
     * The field used to initialize attributes during battle. This is also what updates the base values during a
     * level-up or a form change.
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Metadata", meta = (EditCondition = "Type != EPokemonStatType::Main"))
    FGameplayAttribute StagesAttribute;
};

/**
 * Blueprint function library for getting stat data out.
 */
UCLASS()
class POKEMONDATA_API UStatHelper : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
    /**
     * Get the list of all possible stat names.
     * @return The list of all possible stat names.
     */
    UFUNCTION()
    static TArray<FName> GetStatNames();

    /**
     * Get the list of all possible main stat names.
     * @return The list of all possible main stat names.
     */
    UFUNCTION()
    static TArray<FName> GetMainStatNames();

    /**
     * Get the list of all possible main/battle stat names.
     * @return The list of all possible main/battle stat names.
     */
    UFUNCTION()
    static TArray<FName> GetMainBattleStatNames();

    /**
     * Get the list of all possible battle stat names.
     * @return The list of all possible battle stat names.
     */
    UFUNCTION()
    static TArray<FName> GetBattleStatNames();
};

/**
 * Thin wrapper around a battle stat name, that forces the user to select a battle stat.
 */
USTRUCT(BlueprintType)
struct POKEMONDATA_API FBattleStat {
    GENERATED_BODY()

    /**
     * The name of the battle stat.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data,
              meta = (GetOptions = "PokemonData.StatHelper.GetBattleStatNames"))
    FName Stat;
};

/**
 * Function used to get the type hash of the battle, making it identical to the wrapped property.
 * @param Key The key structure
 * @return The return type in question
 */
inline uint32 GetTypeHash(const FBattleStat &Key) {
    return GetTypeHash(Key.Stat);
}