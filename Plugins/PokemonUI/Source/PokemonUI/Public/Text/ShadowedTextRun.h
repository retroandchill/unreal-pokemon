// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Text/SlateTextRun.h"

/**
 * Modified version of FSlateTextRun to support drawing three drop shadows.
 */
class POKEMONUI_API FShadowedTextRun : public FSlateTextRun {
  public:
    /**
     * Create a new instance using the provided run info.
     * @param InRunInfo The information for the text run.
     * @param InText The text to draw.
     * @param Style The style to apply to the text.
     */
    FShadowedTextRun(const FRunInfo &InRunInfo, const TSharedRef<const FString> &InText, const FTextBlockStyle &Style);

    /**
     * Create a new instance using the provided run info.
     * @param InRunInfo The information for the text run.
     * @param InText The text to draw.
     * @param Style The style to apply to the text.
     * @param InRange The range of the text to render
     */
    FShadowedTextRun(const FRunInfo &InRunInfo, const TSharedRef<const FString> &InText, const FTextBlockStyle &Style,
                     const FTextRange &InRange);

    int32 OnPaint(const FPaintArgs &PaintArgs, const FTextArgs &TextArgs, const FGeometry &AllottedGeometry,
                  const FSlateRect &MyCullingRect, FSlateWindowElementList &OutDrawElements, int32 LayerId,
                  const FWidgetStyle &InWidgetStyle, bool bParentEnabled) const override;
};
