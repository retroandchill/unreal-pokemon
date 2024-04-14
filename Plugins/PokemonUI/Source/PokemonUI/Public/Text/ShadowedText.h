// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Primatives/DisplayText.h"

#include "ShadowedText.generated.h"

/**
 * Text widget that consists of three (3) additional text shadows that are displayed.
 */
UCLASS(Blueprintable, Abstract)
class POKEMONUI_API UShadowedText : public UDisplayText {
    GENERATED_BODY()

  protected:
    void SetTextInfo() override;

  public:
    void OnTextSet_Implementation(const FText &Text) override;

    /**
     * Get the first text shadow to display
     * @return The first text shadow to display
     */
    UFUNCTION(BlueprintPure, Category = "Widgets|Text", meta = (BlueprintInternalUseOnly = "true"))
    UTextBlock *GetShadow1() const;

    /**
     * Get the second text shadow to display
     * @return The second text shadow to display
     */
    UFUNCTION(BlueprintPure, Category = "Widgets|Text", meta = (BlueprintInternalUseOnly = "true"))
    UTextBlock *GetShadow2() const;

    /**
     * Get the third text shadow to display
     * @return The third text shadow to display
     */
    UFUNCTION(BlueprintPure, Category = "Widgets|Text", meta = (BlueprintInternalUseOnly = "true"))
    UTextBlock *GetShadow3() const;

    /**
     * Set the color of the text's shadow
     * @param Color The new shadow color
     */
    UFUNCTION(BlueprintCallable, Category = Display, meta = (BlueprintInternalUseOnly = "true"))
    void SetShadowColor(const FSlateColor &Color);

  private:
    /**
     * Internal method used to set the text and font of a shadow
     * @param Shadow A pointer to the shadow to set the information of
     */
    void SetShadowTextAndFont(UTextBlock *Shadow);

    /**
     * Set the text of the given shadow
     * @param Shadow A pointer to the shadow
     * @param Text The text to set
     */
    void SetShadowText(UTextBlock *Shadow, const FText &Text);

    /**
     * The first text shadow to display
     */
    UPROPERTY(meta = (BindWidget), BlueprintGetter = GetShadow1, Category = "Widgets|Text")
    TObjectPtr<UTextBlock> Shadow1;

    /**
     * The second text shadow to display
     */
    UPROPERTY(meta = (BindWidget), BlueprintGetter = GetShadow2, Category = "Widgets|Text")
    TObjectPtr<UTextBlock> Shadow2;

    /**
     * The third text shadow to display
     */
    UPROPERTY(meta = (BindWidget), BlueprintGetter = GetShadow3, Category = "Widgets|Text")
    TObjectPtr<UTextBlock> Shadow3;

    /**
     * The color to set for the shadow text
     */
    UPROPERTY(EditAnywhere, BlueprintSetter = SetShadowColor, Category = Display)
    FSlateColor ShadowColor;
};
