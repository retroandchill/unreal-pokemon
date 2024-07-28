// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "WidgetUtilities.generated.h"

class URichTextBlock;
class UCommonTextStyle;
class UImage;
/**
 * Utility class for Widget related operations
 */
UCLASS()
class RPGMENUS_API UWidgetUtilities : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
    /**
     * Get the DPI scale of the UMG widgets
     * @return The current DPI scale
     */
    UFUNCTION(BlueprintPure, DisplayName = "Get Widget DPI Scale", Category = "UMG|Scaling")
    static float GetWidgetDPIScale();

    /**
     * Get the desired size of a brush based on its input image. Defaults to the user-defined size if another size
     * cannot be found.
     * @param Brush The brush in question to get the size of
     * @return The desired size.
     */
    UFUNCTION(BlueprintPure, Category = "Slate|Images")
    static FVector2D GetDesiredBrushSize(const FSlateBrush &Brush);

    /**
     * Get the desired size of a texture asset based on the width and height of the source image.
     * @param Texture The source texture
     * @return The desired size.
     */
    UFUNCTION(BlueprintPure, Category = "Slate|Images")
    static FVector2D GetDesiredTextureSize(const UTexture2D *const Texture);

    /**
     * Set the size of the brush to the desired size, keeping all other elements the same
     * @param Brush The size of the brush in question
     * @param NewSize The new size of the brush to set
     */
    UFUNCTION(BlueprintCallable, Category = "Slate|Images")
    static void ChangeBrushSize(UPARAM(Ref) FSlateBrush &Brush, FVector2D NewSize);

    UFUNCTION(BlueprintCallable, Category = "Slate|Images")
    static void SetBrushFromAsset(UImage *ImageWidget, UObject *Asset, bool MatchSize = false);

    UFUNCTION(BlueprintCallable, Category = "Text|Styles")
    static void SetTextStyleOverride(URichTextBlock* TextBlock, TSubclassOf<UCommonTextStyle> Style);
};
