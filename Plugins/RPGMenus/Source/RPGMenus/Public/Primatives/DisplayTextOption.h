// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "SelectableOption.h"
#include "DisplayTextOption.generated.h"

class UDisplayText;
/**
 * Selectable option that contains a display text that can be updated
 */
UCLASS(Abstract)
class RPGMENUS_API UDisplayTextOption : public USelectableOption {
	GENERATED_BODY()
	
public:
	/**
	 * Get the display text as shown to the player
	 * @return The display text
	 */
	UFUNCTION(BlueprintPure, Category = Text)
	FText GetText() const;

	/**
	 * Set the display text for this widget
	 * @param NewText The new display text for the widget
	 */
	UFUNCTION(BlueprintCallable, Category = Display)
	void SetText(const FText& NewText);

	/**
	 * Get the font to be displayed to the player
	 * @return The font to set for the window
	 */
	UFUNCTION(BlueprintPure, Category = Display, meta = (BlueprintInternalUseOnly="true"))
	const FSlateFontInfo& GetDisplayFont() const;

	/**
	 * Set the color of the text to the given new color
	 * @param Color The new color for the text
	 */
	UFUNCTION(BlueprintCallable, Category = Display, meta = (BlueprintInternalUseOnly="true"))
	void SetTextColor(const FSlateColor& Color);

	/**
	 * Get the size of the current text contained within this widget
	 * @return The size of the current text
	 */
	FVector2D GetTextSize() const;

	/**
	 * Get the size of the given text when displayed to the player
	 * @param Text The text to display to the player
	 * @return The size of the given text
	 */
	FVector2D GetTextSize(const FString& Text) const;

	/**
	 * Get the total size of the text drawing area
	 * @return The total size of the area to draw the text in
	 */
	FVector2D GetTotalTextAreaSize() const;

	/**
	 * Get the padding around the display text for the player
	 * @return The padding around the text
	 */
	UFUNCTION(BlueprintCallable, Category = "Text|Size")
	FMargin GetDisplayTextPadding() const;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UDisplayText> DisplayText;
};
