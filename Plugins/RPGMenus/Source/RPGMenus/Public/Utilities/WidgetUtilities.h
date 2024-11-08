// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "WidgetUtilities.generated.h"

class UCommonButtonBase;
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
    UFUNCTION(BlueprintCallable, Category = "Slate|Images")
    static void SetBrushFromAsset(UImage *ImageWidget, UObject *Asset, bool MatchSize = false);

    UFUNCTION(BlueprintCallable, Category = "Text|Styles")
    static void SetTextStyleOverride(URichTextBlock *TextBlock, TSubclassOf<UCommonTextStyle> Style);

};
