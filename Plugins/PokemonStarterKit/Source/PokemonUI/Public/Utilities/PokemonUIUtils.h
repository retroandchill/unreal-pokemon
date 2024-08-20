// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Pokemon/Breeding/PokemonGender.h"

#include "PokemonUIUtils.generated.h"

class UPokemonMenuBase;
class UCommand;
class UMenuHandler;
class IPokemon;
class UProgressBar;
class UCommonTextBlock;
class UDisplayText;

/**
 * Blueprint function library for basic operations that the UI might require
 */
UCLASS()
class POKEMONUI_API UPokemonUIUtils : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
    /**
     * The default animation drain speed in generation 4. This is 1 HP every 2 frames of a 60 FPS drain.
     */
    static constexpr float AnimationDrainSpeed = 2.f / 60.f;

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

    /**
     * Set the values in the given progres bar to percentage given the provided values
     * @param ProgressBar The progress bar to set the percent of
     * @param CurrentValue The current value
     * @param MaxValue The maximum value
     */
    static void SetBarValues(TObjectPtr<UProgressBar> &ProgressBar, float CurrentValue, float MaxValue);

    /**
     * Set the text for a Pokémon's gender in the screen
     * @param Gender The Pokémon to process
     * @param TextWidget The widget to set the text for
     */
    UFUNCTION(BlueprintCallable, Category = Content)
    static void SetPokemonGenderText(EPokemonGender Gender, UDisplayText *TextWidget);
};