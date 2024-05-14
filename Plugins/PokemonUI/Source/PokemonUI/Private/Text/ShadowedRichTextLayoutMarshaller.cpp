// "Unreal Pokémon" created by Retro & Chill.

#include "Text/ShadowedRichTextLayoutMarshaller.h"
#include "Framework/Text/SlateTextUnderlineLineHighlighter.h"
#include "Text/ShadowedTextRun.h"

FShadowedRichTextLayoutMarshaller::FShadowedRichTextLayoutMarshaller(TArray<TSharedRef<ITextDecorator>> InDecorators,
                                                                     const ISlateStyle *const InDecoratorStyleSet)
    : FRichTextLayoutMarshaller(MoveTemp(InDecorators), InDecoratorStyleSet) {
}

FShadowedRichTextLayoutMarshaller::FShadowedRichTextLayoutMarshaller(TSharedPtr<IRichTextMarkupParser> InParser,
                                                                     TSharedPtr<IRichTextMarkupWriter> InWriter,
                                                                     TArray<TSharedRef<ITextDecorator>> InDecorators,
                                                                     const ISlateStyle *const InDecoratorStyleSet)
    : FRichTextLayoutMarshaller(MoveTemp(InParser), MoveTemp(InWriter), MoveTemp(InDecorators), InDecoratorStyleSet) {
}

void FShadowedRichTextLayoutMarshaller::AppendRunsForText(
    const int32 LineIndex, const FTextRunParseResults &TextRun, const FString &ProcessedString,
    const FTextBlockStyle &DefaultTextStyle, const TSharedRef<FString> &InOutModelText, FTextLayout &TargetTextLayout,
    TArray<TSharedRef<IRun>> &Runs, TArray<FTextLineHighlight> &LineHighlights,
    TMap<const FTextBlockStyle *, TSharedPtr<FSlateTextUnderlineLineHighlighter>> &CachedUnderlineHighlighters,
    TMap<const FTextBlockStyle *, TSharedPtr<FSlateTextStrikeLineHighlighter>> &CachedStrikeLineHighlighters) {
    TSharedPtr<ISlateRun> Run;

    if (TSharedPtr<ITextDecorator> Decorator = TryGetDecorator(ProcessedString, TextRun); Decorator.IsValid()) {
        // Create run and update model string.
        Run =
            Decorator->Create(TargetTextLayout.AsShared(), TextRun, ProcessedString, InOutModelText, DecoratorStyleSet);
    } else {
        FRunInfo RunInfo(TextRun.Name);
        for (const TPair<FString, FTextRange> &Pair : TextRun.MetaData) {
            int32 Length = FMath::Max(0, Pair.Value.EndIndex - Pair.Value.BeginIndex);
            RunInfo.MetaData.Add(Pair.Key, ProcessedString.Mid(Pair.Value.BeginIndex, Length));
        }

        const FTextBlockStyle *TextBlockStyle;
        FTextRange ModelRange;
        ModelRange.BeginIndex = InOutModelText->Len();
        if (!(TextRun.Name.IsEmpty()) && DecoratorStyleSet->HasWidgetStyle<FTextBlockStyle>(FName(*TextRun.Name))) {
            *InOutModelText += ProcessedString.Mid(TextRun.ContentRange.BeginIndex,
                                                   TextRun.ContentRange.EndIndex - TextRun.ContentRange.BeginIndex);
            TextBlockStyle = &(DecoratorStyleSet->GetWidgetStyle<FTextBlockStyle>(FName(*TextRun.Name)));
        } else {
            *InOutModelText += ProcessedString.Mid(TextRun.OriginalRange.BeginIndex,
                                                   TextRun.OriginalRange.EndIndex - TextRun.OriginalRange.BeginIndex);
            TextBlockStyle = &DefaultTextStyle;
        }
        ModelRange.EndIndex = InOutModelText->Len();

        // Create run.
        TSharedPtr<FShadowedTextRun> SlateTextRun =
            MakeShared<FShadowedTextRun>(RunInfo, InOutModelText, *TextBlockStyle, ModelRange);

        if (SlateTextRun) {
            // Apply the FontSizeMultiplier at the style use by the IRun
            SlateTextRun->ApplyFontSizeMultiplierOnTextStyle(FontSizeMultiplier);
        }
        Run = SlateTextRun;

        if (!TextBlockStyle->UnderlineBrush.GetResourceName().IsNone()) {
            TSharedPtr<FSlateTextUnderlineLineHighlighter> UnderlineLineHighlighter =
                CachedUnderlineHighlighters.FindRef(TextBlockStyle);
            if (!UnderlineLineHighlighter.IsValid()) {
                UnderlineLineHighlighter = FSlateTextUnderlineLineHighlighter::Create(
                    TextBlockStyle->UnderlineBrush, TextBlockStyle->Font, TextBlockStyle->ColorAndOpacity,
                    TextBlockStyle->ShadowOffset, TextBlockStyle->ShadowColorAndOpacity);
                CachedUnderlineHighlighters.Add(TextBlockStyle, UnderlineLineHighlighter);
            }

            LineHighlights.Add(FTextLineHighlight(LineIndex, ModelRange,
                                                  FSlateTextUnderlineLineHighlighter::DefaultZIndex,
                                                  UnderlineLineHighlighter.ToSharedRef()));
        }

        if (!TextBlockStyle->StrikeBrush.GetResourceName().IsNone()) {
            TSharedPtr<FSlateTextStrikeLineHighlighter> StrikeLineHighlighter =
                CachedStrikeLineHighlighters.FindRef(TextBlockStyle);
            if (!StrikeLineHighlighter.IsValid()) {
                StrikeLineHighlighter = FSlateTextStrikeLineHighlighter::Create(
                    TextBlockStyle->StrikeBrush, TextBlockStyle->Font, TextBlockStyle->ColorAndOpacity,
                    TextBlockStyle->ShadowOffset, TextBlockStyle->ShadowColorAndOpacity);
                CachedStrikeLineHighlighters.Add(TextBlockStyle, StrikeLineHighlighter);
            }

            LineHighlights.Add(FTextLineHighlight(LineIndex, ModelRange, FSlateTextStrikeLineHighlighter::DefaultZIndex,
                                                  StrikeLineHighlighter.ToSharedRef()));
        }
    }

    Runs.Add(Run.ToSharedRef());
}