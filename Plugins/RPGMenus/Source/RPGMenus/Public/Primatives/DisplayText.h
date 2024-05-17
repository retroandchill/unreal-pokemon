// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/RPGMenusSettings.h"

#include "DisplayText.generated.h"

class URichTextBlock;
class USizeBox;
class UMenuCommand;
class UTextBlock;

/**
 * Basic text command widget used for handling an option in a menu
 */
UCLASS(Blueprintable, Abstract)
class RPGMENUS_API UDisplayText : public UUserWidget {
    GENERATED_BODY()

  protected:
    TSharedRef<SWidget> RebuildWidget() override;
    void SynchronizeProperties() override;

    /**
     * Set the information about the text based on the default settings of the widget
     */
    virtual void SetTextInfo();

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
    void SetText(const FText &NewText);

    /**
     * Set the color of the text to the given new color
     * @param Color The new color for the text
     */
    UFUNCTION(BlueprintCallable, Category = Display)
    void SetTextColor(const FSlateColor &Color);

    /**
     * Set the color of the text's shadow
     * @param Color The new shadow color
     */
    UFUNCTION(BlueprintCallable, Category = Display)
    void SetShadowColor(const FSlateColor &Color);

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
    FVector2D GetTextSize(const FString &Text) const;

    /**
     * Get the total size of the text drawing area
     * @return The total size of the area to draw the text in
     */
    FVector2D GetTotalTextAreaSize() const;

    /**
     * Get the padding around the display text for the player
     * @return The padding around the text
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Text|Size")
    FMargin GetDisplayTextPadding() const;

  protected:
    /**
     * Called when the text is set by the game to make sure all Blueprint Visuals are good
     */
    UFUNCTION(BlueprintNativeEvent, Category = Text)
    void OnTextSet(const FText &Text);

    /**
     * Get the primary display text widget
     * @return The displayed text widget to the player
     */
    UFUNCTION(BlueprintPure, Category = "Widgets|Text", meta = (BlueprintInternalUseOnly = "true"))
    URichTextBlock *GetDisplayTextWidget() const;

  private:
    UPROPERTY(EditAnywhere, Category = "Widgets|Text", meta = (MultiLine))
    FText InitialText = NSLOCTEXT("PokemonUI", "DisplayText_InitialText", "Text Block");

    /**
     * The displayed text widget to the player
     */
    UPROPERTY(meta = (BindWidget), BlueprintGetter = GetDisplayTextWidget, Category = "Widgets|Text")
    TObjectPtr<URichTextBlock> DisplayTextWidget;

    /**
     * The box used to constrain the size of the widget to a fixed amount
     */
    UPROPERTY(meta = (BindWidgetOptional))
    TObjectPtr<USizeBox> SizeBox;

    /**
     * The font to set for the window
     */
    UPROPERTY(EditAnywhere, Category = Display, meta = (UIMin = 1, ClampMin = 1))
    TOptional<FSlateFontInfo> DisplayFont;

    /**
     * The color of the text to display
     */
    UPROPERTY(EditAnywhere, Category = Display)
    TOptional<FSlateColor> TextColor;

    /**
     * The color to set for the shadow text
     */
    UPROPERTY(EditAnywhere, Category = Display)
    TOptional<FSlateColor> ShadowColor;

    /**
     * The table used to determine the text styles.
     */
    UPROPERTY(EditAnywhere, Category = Display)
    TObjectPtr<UDataTable> TextStyles = GetDefault<URPGMenusSettings>()->GetTextStyleDataTable();

    /**
     * The amount to scale each line's height by.
     */
    UPROPERTY(EditAnywhere, Category = Display)
    float LineHeightPercentage = 1.f;
};
