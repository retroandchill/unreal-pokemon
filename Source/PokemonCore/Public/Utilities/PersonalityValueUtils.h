// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PersonalityValueUtils.generated.h"

/**
 * Utility class for personality value related operations
 */
UCLASS()
class POKEMONCORE_API UPersonalityValueUtils : public UBlueprintFunctionLibrary {
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
};
