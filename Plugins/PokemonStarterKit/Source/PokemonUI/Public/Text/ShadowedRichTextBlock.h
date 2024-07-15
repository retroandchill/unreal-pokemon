// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CommonRichTextBlock.h"

#include "ShadowedRichTextBlock.generated.h"

/**
 * Internal struct used for handling information about text styles.
 */
USTRUCT(NotBlueprintType)
struct FModifiedTextStyle {
    GENERATED_BODY()

    /**
     * The constructed style set pointer.
     */
    TSharedPtr<FSlateStyleSet> StyleSet;

    /**
     * The default text style that is referenced.
     */
    UPROPERTY()
    FTextBlockStyle DefaultTextStyle;

    /**
     * The override for the default text style if the override option is checked.
     */
    UPROPERTY()
    FTextBlockStyle DefaultTextStyleOverride;

    /**
     * The named text styles pulled from the table.
     */
    UPROPERTY()
    TMap<FName, FTextBlockStyle> TaggedTextStyles;

    /**
     * The rich text block that this style uses
     */
    TSharedPtr<SRichTextBlock> RichTextBlock;
};

/**
 * Clone of RichTextBlock that draws three shadows instead of just one.
 */
UCLASS()
class POKEMONUI_API UShadowedRichTextBlock : public UCommonRichTextBlock {
    GENERATED_BODY()

  protected:
    TSharedRef<SWidget> RebuildWidget() override;
};