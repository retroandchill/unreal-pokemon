// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "PokemonTextButtonBase.h"
#include "TextIconButtonBase.generated.h"

class UImage;
/**
 * 
 */
UCLASS(Abstract)
class POKEMONUI_API UTextIconButtonBase : public UPokemonTextButtonBase {
    GENERATED_BODY()

protected:
    void NativePreConstruct() override;
    
public:
    /**
     * Get the image that is drawn to the player
     * @return The brush that is applied to the icon
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Content)
    const FSlateBrush& GetIconImage() const;

    /**
     * Set the image that is drawn to the player
     * @param NewImage The brush that is applied to the icon
     */
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = Content, meta = (AutoCreateRefTerm = NewImage))
    void SetIconImage(const FSlateBrush& NewImage);

    /**
     * Get the image that is drawn to the player when the button is hovered
     * @return The brush that is applied to the icon when the button is hovered
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Content)
    const FSlateBrush& GetIconImageHovered() const;

    /**
     * Set the image that is drawn to the player when the button is hovered
     * @param NewImage The brush that is applied to the icon when the button is hovered
     */
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = Content, meta = (AutoCreateRefTerm = NewImage))
    void SetIconImageHovered(const FSlateBrush& NewImage);

    /**
     * Get the image widget that displays the icon
     * @return The image widget that displays the icon
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Widgets)
    UImage* GetIcon() const;

protected:
    void NativeOnHovered() override;
    void NativeOnUnhovered() override;
    
private:
    /**
     * The image widget that displays the icon
     */
    UPROPERTY(BlueprintGetter = GetIcon, Category = Widgets, meta = (BindWidget))
    TObjectPtr<UImage> Icon;
    
    /**
     * The brush that is applied to the icon
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetIconImage, BlueprintSetter = SetIconImage, Category = Content)
    FSlateBrush IconImage;

    /**
     * The brush that is applied to the icon when the button is hovered
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetIconImageHovered, BlueprintSetter = SetIconImageHovered, Category = Content)
    FSlateBrush IconImageHovered;

};