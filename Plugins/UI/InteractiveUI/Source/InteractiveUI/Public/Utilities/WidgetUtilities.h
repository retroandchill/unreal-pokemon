// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "WidgetUtilities.generated.h"

class USizeBox;
class UCommonButtonBase;
class URichTextBlock;
class UCommonTextStyle;
class UImage;

/**
 * Utility class for Widget related operations
 */
UCLASS()
class INTERACTIVEUI_API UWidgetUtilities : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

  public:
    /**
     * Sets the brush of a UImage widget based on the provided asset.
     * The asset can be a Texture2D, MaterialInterface, or an object that implements the USlateTextureAtlasInterface.
     * If the asset type is not supported or null, the brush will be set to nullptr.
     *
     * @param ImageWidget Pointer to the UImage widget whose brush is to be configured.
     * @param Asset Object used to create the brush, such as a Texture2D, MaterialInterface, or other compatible asset.
     * @param MatchSize If true, the image widget will adjust its size to match the dimensions of the provided asset.
     */
    UFUNCTION(BlueprintCallable, Category = "Slate|Images")
    static void SetBrushFromAsset(UImage *ImageWidget, UObject *Asset, bool MatchSize = false);

    /**
     * Overrides the text style of a specified Rich Text Block using a given text style class.
     * If the provided style is nullptr, all default style overrides will be cleared from the text block.
     *
     * @param TextBlock Pointer to the Rich Text Block to have its text style overridden.
     * @param Style The text style class to override the default text style. Passing nullptr clears the overrides.
     */
    UFUNCTION(BlueprintCallable, Category = "Text|Styles")
    static void SetTextStyleOverride(URichTextBlock *TextBlock, TSubclassOf<UCommonTextStyle> Style);
};
