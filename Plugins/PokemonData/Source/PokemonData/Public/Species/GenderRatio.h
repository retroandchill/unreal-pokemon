// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"

#include "GenderRatio.generated.h"

// ReSharper disable once CppUE4CodingStandardNamingViolationWarning
/**
 * The maximum chance of a Pokémon being female
 */
constexpr int32 FEMALE_CHANCE_MAX = 255;

/**
 * Represents the Gender Ratio for a Specific Pokémon
 */
USTRUCT(BlueprintType, meta = (DatabaseType = "Hardcoded"))
struct POKEMONDATA_API FGenderRatio : public FTableRowBase {
    GENERATED_BODY()

    /**
     * The internal ID used for lookup by the game
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Basic")
    FName ID;

    /**
     * The name that is displayed to the player
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Basic")
    FText RealName;

    /**
     * If true this species is genderless.
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gender Determination")
    bool IsGenderless;

    /**
     * The chance this species has of being female out of 255. (Ignored if "Is Genderless" is set to true)
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gender Determination")
    uint8 FemaleChance;
};

/**
 * Blueprint function library for getting gender ratio data out.
 */
UCLASS()
class POKEMONDATA_API UGenderRatioHelper : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
    /**
     * Get the list of all possible gender ratio names.
     * @return The list of all possible gender ratio names.
     */
    UFUNCTION()
    static TArray<FName> GetGenderRatioNames();
};
