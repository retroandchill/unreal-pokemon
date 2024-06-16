﻿// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "Data/Command.h"
#include "Handlers/MenuHandler.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Pokemon/Breeding/PokemonGender.h"
#include "RangeHelpers.h"
#include <range/v3/view/filter.hpp>
#include <range/v3/view/transform.hpp>

#include "PokemonUIUtils.generated.h"

class UCommand;
class UMenuHandler;
class IPokemon;
struct FTextColor;
class UProgressBar;
class UDisplayText;

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

    /**
     * Set the text for the item if it is not null
     * @param TextWidget The item to set the text of
     * @param Text The text to set everything to
     */
    static void SetItemText(TObjectPtr<UDisplayText> &TextWidget, const FText &Text);

    /**
     * Set the text for the item if it is not null
     * @param TextWidget The item to set the text of
     * @param Text The text to set everything to
     */
    static void SetItemText(TObjectPtr<UDisplayText> &TextWidget, FStringView Text);

    /**
     * Set the color of the text if it is not null
     * @param TextWidget The item to set the color of
     * @param Color The color to set everything to
     */
    static void SetItemTextColor(TObjectPtr<UDisplayText> &TextWidget, const FTextColor &Color);

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
    static void SetPokemonGenderText(EPokemonGender Gender, TObjectPtr<UDisplayText> &TextWidget);

    /**
     * Create the command list from the given list of provided handlers
     * @tparam T The type of the handler class
     * @tparam A The types of the arguments to the ShouldShow() method in the handlers
     * @param Handlers The array of handler to process
     * @param CancelText The text to display to the player (doesn't show if not set)
     * @param Args The arguments to the ShouldShow() method in the handlers
     * @return The list of commands that was created
     */
    template <typename T, typename... A>
    static TArray<TObjectPtr<UCommand>> CreateCommandListFromHandlers(const TArray<T> &Handlers,
                                                                      const TOptional<FText> &CancelText, A &&...Args) {
        auto Commands =
            RangeHelpers::CreateRange(Handlers) |
            ranges::views::filter([&Args...](T Handler) { return Handler->ShouldShow(Forward<A>(Args)...); }) |
            ranges::views::transform(&UMenuHandler::CreateCommand) | RangeHelpers::TToArray<TObjectPtr<UCommand>>();

        if (CancelText.IsSet()) {
            static FName CancelName = TEXT("Cancel");
            Commands.Add(UCommand::CreateBasicCommand(CancelName, *CancelText));
        }

        return Commands;
    }
};