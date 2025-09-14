// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CommonRichTextBlock.h"

#include "DialogueTextBlock.generated.h"

class FRichTextLayoutMarshaller;
class FSlateTextLayout;
/**
 * A text block that exposes more information about text layout.
 */
UCLASS()
class RPGMENUS_API UDialogueTextBlock : public UCommonRichTextBlock
{
    GENERATED_BODY()

  public:
    FORCEINLINE TSharedPtr<FSlateTextLayout> GetTextLayout() const
    {
        return TextLayout;
    }

    FORCEINLINE TSharedPtr<FRichTextLayoutMarshaller> GetTextMarshaller() const
    {
        return TextMarshaller;
    }

  protected:
    TSharedRef<SWidget> RebuildWidget() override;

    virtual TSharedRef<FRichTextLayoutMarshaller> CreateMarshaller(
        TArray<TSharedRef<ITextDecorator>> CreatedDecorators);
    virtual TSharedRef<FSlateTextLayout> CreateLayout(SWidget *InOwner, const FTextBlockStyle &InDefaultTextStyle);

  private:
    TSharedPtr<FSlateTextLayout> TextLayout;
    TSharedPtr<FRichTextLayoutMarshaller> TextMarshaller;
};
