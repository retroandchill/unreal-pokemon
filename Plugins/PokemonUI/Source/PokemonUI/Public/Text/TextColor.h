// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

#include "TextColor.generated.h"

/**
 * Struct that holds the information regarding a text color
 */
USTRUCT(BlueprintType)
struct POKEMONUI_API FTextColor {
    GENERATED_BODY()

    /**
     * The primary color of the text.
     */
    UPROPERTY(EditAnywhere, Category = "User Interface|Text")
    FSlateColor MainColor;

    /**
     * The color of the text's shadow.
     */
    UPROPERTY(EditAnywhere, Category = "User Interface|Text")
    FSlateColor ShadowColor;
};
