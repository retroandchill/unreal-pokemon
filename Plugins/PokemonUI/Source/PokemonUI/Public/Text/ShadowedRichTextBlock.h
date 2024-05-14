// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Components/RichTextBlock.h"
#include "Components/TextWidgetTypes.h"
#include "ShadowedRichTextBlock.generated.h"

USTRUCT()
struct FModifiedTextStyle {
    GENERATED_BODY()

    TSharedPtr<FSlateStyleSet> StyleSet;

    UPROPERTY()
    FTextBlockStyle DefaultTextStyle;

    UPROPERTY()
    FTextBlockStyle DefaultTextStyleOverride;

    UPROPERTY()
    TMap<FName, FTextBlockStyle> TaggedTextStyles;

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
    void GenerateStyleSets();
    void SetDefaultStyleOverride(FModifiedTextStyle &StyleSet, const FVector2D &ShadowMask);
    void ProcessStyleSet(FModifiedTextStyle& StyleSet, FName SubStyleName, const FRichTextStyleRow& Row, const FVector2D& ShadowMask);

    UPROPERTY()
    TArray<FModifiedTextStyle> ModifiedTextStyles;

    TSharedPtr<SOverlay> Overlay;
    
};
