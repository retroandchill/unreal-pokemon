// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Components/RichTextBlock.h"
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
class POKEMONUI_API UShadowedRichTextBlock : public URichTextBlock {
    GENERATED_BODY()

protected:
    TSharedRef<SWidget> RebuildWidget() override;
    void SynchronizeProperties() override;
    void UpdateStyleData() override;

public:
    void ReleaseSlateResources(bool bReleaseChildren) override;
    
private:
    /**
     * Generate the style sets used for the additional shadow fills.
     */
    void GenerateStyleSets();

    /**
     * Set the default style override information for the struct.
     * @param StyleSet The style set to modify
     * @param ShadowMask The mask used to mask out the X and Y values of the shadow
     */
    void SetDefaultStyleOverride(FModifiedTextStyle &StyleSet, const FVector2D &ShadowMask) const;

    /**
     * Process a style set and construct the new information for the widgets to use.
     * @param StyleSet The style set to process
     * @param SubStyleName The name of the substyle in question
     * @param Row The row to process
     * @param ShadowMask The mask used to mask out the X and Y values of the shadow
     */
    void ProcessStyleSet(FModifiedTextStyle& StyleSet, FName SubStyleName, const FRichTextStyleRow& Row, const FVector2D& ShadowMask);

    /**
     * The list of modified text styles used by the widget.
     */
    UPROPERTY(Transient)
    TArray<FModifiedTextStyle> ModifiedTextStyles;

    /**
     * The overlay widget that contains all three text blocks.
     */
    TSharedPtr<SOverlay> Overlay;
    
};
