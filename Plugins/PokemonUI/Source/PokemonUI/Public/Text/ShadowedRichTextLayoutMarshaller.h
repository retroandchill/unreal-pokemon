// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Text/RichTextLayoutMarshaller.h"

/**
 *
 */
class POKEMONUI_API FShadowedRichTextLayoutMarshaller : public FRichTextLayoutMarshaller {
  public:
    FShadowedRichTextLayoutMarshaller(TArray<TSharedRef<ITextDecorator>> InDecorators,
                                      const ISlateStyle *const InDecoratorStyleSet);
    FShadowedRichTextLayoutMarshaller(TSharedPtr<IRichTextMarkupParser> InParser,
                                      TSharedPtr<IRichTextMarkupWriter> InWriter,
                                      TArray<TSharedRef<ITextDecorator>> InDecorators,
                                      const ISlateStyle *const InDecoratorStyleSet);

    void AppendRunsForText(
        const int32 LineIndex, const FTextRunParseResults &TextRun, const FString &ProcessedString,
        const FTextBlockStyle &DefaultTextStyle, const TSharedRef<FString> &InOutModelText,
        FTextLayout &TargetTextLayout, TArray<TSharedRef<IRun>> &Runs, TArray<FTextLineHighlight> &LineHighlights,
        TMap<const FTextBlockStyle *, TSharedPtr<FSlateTextUnderlineLineHighlighter>> &CachedUnderlineHighlighters,
        TMap<const FTextBlockStyle *, TSharedPtr<FSlateTextStrikeLineHighlighter>> &CachedStrikeLineHighlighters);
};
