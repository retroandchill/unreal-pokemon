// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PokemonUIUtils.generated.h"

/**
 * Blueprint function library for basic operations that the UI might require
 */
UCLASS()
class POKEMONUI_API UPokemonUIUtils : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:
	/**
	 * Take a number and pad it with the required number of leading 0s
	 * @param Value The number to pad with 0s
	 * @param DesiredLength The desired length of the string
	 * @return The formatted number string
	 */
	UFUNCTION(BlueprintPure, Category = "Text|Formatting")
	static FString ZeroPad(int32 Value, int32 DesiredLength);

	/**
	 * Take a number and pad it with the required number of leading spaces
	 * @param Value The number to pad with spaces
	 * @param DesiredLength The desired length of the string
	 * @return The formatted number string
	 */
	UFUNCTION(BlueprintPure, Category = "Text|Formatting")
	static FString SpacePad(int32 Value, int32 DesiredLength);

	/**
	 * Take a number and pad it with the required number of leading characters
	 * @param Value The number to pad with characters
	 * @param DesiredLength The desired length of the string
	 * @param PaddingCharacter The character to insert in front of the number
	 * @return The formatted number string
	 */
	static FString PadInt(int32 Value, int32 DesiredLength, TCHAR PaddingCharacter = TEXT('0'));
};
