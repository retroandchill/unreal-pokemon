// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "PokemonTextButtonBase.generated.h"

class UCommonTextBlock;

/**
 * Base for any button that displays some kind of text to the player.
 */
UCLASS(Abstract)
class POKEMONUI_API UPokemonTextButtonBase : public UCommonButtonBase {
    GENERATED_BODY()

protected:
    void NativePreConstruct() override;
    
public:
    /**
     * Get the text that is drawn in this widget
     * @return The text that this button will display to the player
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Content)
    const FText& GetText() const;

    /**
     * Set the text that is drawn in this widget
     * @param NewText The text that this button will display to the player
     */
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = Content, meta = (AutoCreateRefTerm = NewText))
    void SetText(const FText& NewText);

    /**
     * Get the text block widget
     * @return The text block that the text is rendered to
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Widgets)
    UCommonTextBlock* GetTextBlock() const;

protected:
    void NativeOnCurrentTextStyleChanged() override;
    
private:
    /**
     * The text block that the text is rendered to
     */
    UPROPERTY(BlueprintGetter = GetTextBlock, Category = Widgets, meta = (BindWidget))
    TObjectPtr<UCommonTextBlock> TextBlock;
    
    /**
     * The text that this button will display to the player
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetText, BlueprintSetter = SetText, Category = Content)
    FText Text;

};
