// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Components/SlateWrapperTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InputUtilities.generated.h"

class UUserWidget;
/**
 * Utilities for testing things that involve user input
 */
UCLASS()
class POKEMONTESTS_API UInputUtilities : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:
	/**
	 * Simulate a key press on a widget
	 * @param Widget The widget in question
	 * @param Key The key to press
	 * @return The reply from the key press
	 */
	UFUNCTION(BlueprintCallable, Category="Testing|Input")
	static FEventReply SimulateKeyPress(UUserWidget* Widget, const FKey &Key);
};
