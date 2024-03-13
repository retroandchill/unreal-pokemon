// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HelpWindow.generated.h"

class UWindow;
class UDisplayText;

/**
 * Window used for displaying some basic help text to the player.
 */
UCLASS(Abstract, Blueprintable)
class RPGMENUS_API UHelpWindow : public UUserWidget {
	GENERATED_BODY()

protected:
	TSharedRef<SWidget> RebuildWidget() override;
	void SynchronizeProperties() override;
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

public:
	/**
	 * Get the text displayed by this window
	 * @return The text to display
	 */
	UFUNCTION(BlueprintPure, Category = Display, BlueprintInternalUseOnly)
	const FText& GetText() const;

	/**
	 * Set the text of this window to the given value.
	 * @param NewText The text to display
	 */
	UFUNCTION(BlueprintCallable, Category = Display, BlueprintInternalUseOnly)
	void SetText(const FText& NewText);

private:
	/**
	 * The frame used for the background of the widget.
	 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWindow> WindowFrame;

	/**
	 * The text displayed to the player in the window.
	 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UDisplayText> HelpText;

	/**
	 * The text displayed to the player in the window.
	 */
	UPROPERTY(EditAnywhere, BlueprintGetter=GetText, BlueprintSetter=SetText, Category = Display)
	FText Text;
};
