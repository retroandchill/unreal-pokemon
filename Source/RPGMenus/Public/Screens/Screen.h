// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Screen.generated.h"

class USelectableWidget;
struct FInputActionInstance;
/**
 * Represents a basic screen used by the UI. They tend to be added in a stack format, and are displayed one on top
 * of the other.
 */
UCLASS(Blueprintable, Abstract)
class RPGMENUS_API UScreen : public UUserWidget {
	GENERATED_BODY()

public:
	/**
	 * Construct the default version of the screen
	 * @param ObjectInitializer The initializer used by Unreal Engine to build the object
	 */
	explicit UScreen(const FObjectInitializer& ObjectInitializer);

	TSharedRef<SWidget> RebuildWidget() override;

protected:
	/**
	 * Close the screen and return to the previous one
	 */
	UFUNCTION(BlueprintCallable, Category = Navigation)
	void CloseScreen();

private:
	/**
	 * Get list of selectable widgets in the window
	 */
	UPROPERTY()
	TArray<TObjectPtr<USelectableWidget>> SelectableWidgets;
};
