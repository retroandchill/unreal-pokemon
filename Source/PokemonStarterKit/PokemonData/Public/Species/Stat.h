// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "DataRetrieval/DataStructHandle.h"
#include "IndexedTableRow.h"
#include "PokemonStatType.h"
#include "UObject/Object.h"

#include "Stat.generated.h"

/**
 * Represents one of the stats in the database.
 */
USTRUCT(BlueprintType, meta = (DatabaseType = "Hardcoded"))
struct POKEMONDATA_API FStat : public FIndexedTableRow
{
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
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Metadata",
              meta = (EditCondition = "Type != EPokemonStatType::Battle"))
    FGameplayAttribute BaseAttribute;

    /**
     * The field used to initialize attributes during battle. This is also what updates the base values during a
     * level-up or a form change.
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Metadata",
              meta = (EditCondition = "Type != EPokemonStatType::Main"))
    FGameplayAttribute StagesAttribute;
};

USTRUCT(BlueprintType, meta = (DisableSplitPin))
struct POKEMONDATA_API FStatHandle
{
    GENERATED_BODY()
    DECLARE_DATA_HANDLE(FStatHandle, FStat)

    /**
     * The name of the battle stat.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data,
              meta = (GetOptions = "PokemonData.StatHelper.GetStatNames"))
    FName RowID;
};

USTRUCT(BlueprintType, meta = (DisableSplitPin))
struct POKEMONDATA_API FMainStatHandle
{
    GENERATED_BODY()
    DECLARE_DATA_HANDLE(FMainStatHandle, FStat)

    /**
     * The name of the battle stat.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data,
              meta = (GetOptions = "PokemonData.StatHelper.GetMainStatNames"))
    FName RowID;
};

/**
 * Thin wrapper around a battle stat name, that forces the user to select a battle stat.
 */
USTRUCT(BlueprintType, meta = (DisableSplitPin))
struct POKEMONDATA_API FBattleStatHandle
{
    GENERATED_BODY()
    DECLARE_DATA_HANDLE(FBattleStatHandle, FStat)

    /**
     * The name of the battle stat.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data,
              meta = (GetOptions = "PokemonData.StatHelper.GetBattleStatNames"))
    FName RowID;
};

USTRUCT(BlueprintType, meta = (DisableSplitPin))
struct POKEMONDATA_API FMainBattleStatHandle
{
    GENERATED_BODY()
    DECLARE_DATA_HANDLE(FMainBattleStatHandle, FStat)

    /**
     * The name of the battle stat.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Data,
              meta = (GetOptions = "PokemonData.StatHelper.GetMainBattleStatNames"))
    FName RowID;
};

/**
 * Blueprint function library for getting stat data out.
 */
UCLASS()
class POKEMONDATA_API UStatHelper : public UBlueprintFunctionLibrary
{
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

    UFUNCTION(BlueprintPure, Category = StatHandle,
              meta = (DisplayName = "Convert To Name", CompactNodeTitle = "->", BlueprintAutocast,
                      AutoCreateRefTerm = Struct))
    static FName ConvertStatHandleToName(const FStatHandle &Struct);

    UFUNCTION(BlueprintPure, Category = Name,
              meta = (DisplayName = "Convert To Stat Handle", CompactNodeTitle = "->", BlueprintAutocast))
    static FStatHandle ConvertNameToItemHandle(FName Name);

    UFUNCTION(BlueprintPure, Category = StatHandle,
              meta = (DisplayName = "Convert To Name", CompactNodeTitle = "->", BlueprintAutocast,
                      AutoCreateRefTerm = Struct))
    static FName ConvertMainStatHandleToName(const FMainStatHandle &Struct);

    UFUNCTION(BlueprintPure, Category = Name,
              meta = (DisplayName = "Convert To Main Stat Handle", CompactNodeTitle = "->", BlueprintAutocast))
    static FMainStatHandle ConvertNameToMainStatHandle(FName Name);

    UFUNCTION(BlueprintPure, Category = StatHandle,
              meta = (DisplayName = "Convert To Name", CompactNodeTitle = "->", BlueprintAutocast,
                      AutoCreateRefTerm = Struct))
    static FName ConvertBattleStatHandleToName(const FBattleStatHandle &Struct);

    UFUNCTION(BlueprintPure, Category = Name,
              meta = (DisplayName = "Convert To Battle STat Handle", CompactNodeTitle = "->", BlueprintAutocast))
    static FBattleStatHandle ConvertNameToBattleStatHandle(FName Name);

    UFUNCTION(BlueprintPure, Category = StatHandle,
              meta = (DisplayName = "Convert To Name", CompactNodeTitle = "->", BlueprintAutocast,
                      AutoCreateRefTerm = Struct))
    static FName ConvertItemMainBattleStatHandleToName(const FMainBattleStatHandle &Struct);

    UFUNCTION(BlueprintPure, Category = Name,
              meta = (DisplayName = "Convert To Main/Battle Stat Handle", CompactNodeTitle = "->", BlueprintAutocast))
    static FMainBattleStatHandle ConverMainBattleStatHandle(FName Name);
};