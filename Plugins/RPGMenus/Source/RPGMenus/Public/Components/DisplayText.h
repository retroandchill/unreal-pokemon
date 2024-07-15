// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/RPGMenusSettings.h"

#include "DisplayText.generated.h"

class UCommonTextStyle;
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
    void NativePreConstruct() override;
    
  public:
    /**
     * Get the display text as shown to the player
     * @return The display text
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Content)
    FText GetText() const;

    /**
     * Set the display text for this widget
     * @param NewText The new display text for the widget
     */
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = Content)
    void SetText(const FText &NewText);

    /**
     * Get the display text as shown to the player
     * @return The display text
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Style)
    TSubclassOf<UCommonTextStyle> GetTextStyle() const;

    /**
     * Set the display text for this widget
     * @param NewStyle The new display text for the widget
     */
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = Style)
    void SetTextStyle(TSubclassOf<UCommonTextStyle> NewStyle);

  protected:
    /**
     * Called when the text is set by the game to make sure all Blueprint Visuals are good
     */
    UFUNCTION(BlueprintImplementableEvent, Category = Text)
    void OnTextSet(const FText &SetText);

    /**
     * Called when the text is set by the game to make sure all Blueprint Visuals are good
     */
    UFUNCTION(BlueprintImplementableEvent, Category = Text)
    void OnTextStyleSet(TSubclassOf<UCommonTextStyle> SetStyle);

  private:
    UPROPERTY(EditAnywhere, BlueprintGetter = GetText, BlueprintSetter = SetText, Category = Content, meta = (MultiLine))
    FText Text = NSLOCTEXT("PokemonUI", "DisplayText_InitialText", "Text Block");

    UPROPERTY(EditAnywhere, BlueprintGetter = GetTextStyle, BlueprintSetter = SetTextStyle, Category = Style)
    TSubclassOf<UCommonTextStyle> TextStyle;
    
};
