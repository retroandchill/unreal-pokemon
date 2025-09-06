// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Species/Stat.h"

#include "PersonalityValueUtils.generated.h"

class IPokemon;
struct FPokemonDTO;
/**
 * Utility class for personality value related operations
 */
UCLASS()
class POKEMONCORE_API UPersonalityValueUtils : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

  public:
    // ReSharper disable once CppUE4CodingStandardNamingViolationWarning
    /**
     * The lower 8 bits of the personality value
     */
    static constexpr int32 LOWER_8_BITS = 0xFF;

    // ReSharper disable once CppUE4CodingStandardNamingViolationWarning
    /**
     * The lower 16 bits of the personality value
     */
    static constexpr int32 LOWER_16_BITS = 0xFFFF;

    /**
     * Generate a unique personality value that can be used for a Pokémon
     * @return A unique 32-bit unsigned Personality value
     */
    static uint32 GeneratePersonalityValue();

    /**
     * Generate a unique personality value that can be used for a Pokémon if one is not already set
     * @param DTO The DTO to check for an override
     * @return A unique 32-bit unsigned Personality value
     */
    static uint32 GeneratePersonalityValue(const FPokemonDTO &DTO);

    /**
     * Determines the best stat of a Pokémon based on its personality value and a given order of stats.
     *
     * @param Pokemon The Pokémon whose best stat is being determined.
     * @param StatOrder The order in which stats should be considered.
     * @param BestStat The stat that will be determined to be the best.
     * @param BestIV The IV (Individual Value) of the best stat.
     */
    UFUNCTION(BlueprintCallable, Category = Summary, meta = (AutoCreateRefTerm = Pokemon, DefaultToSelf = Pokemon))
    static void GetBestStat(const TScriptInterface<IPokemon> &Pokemon, const TArray<FMainStatHandle> &StatOrder,
                            FMainStatHandle &BestStat, int32 &BestIV);
};
