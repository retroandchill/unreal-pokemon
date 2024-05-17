// "Unreal Pokémon" created by Retro & Chill.

#include "Text/ShadowedTextRun.h"
#include "Framework/Text/ShapedTextCache.h"
#include <array>

FShadowedTextRun::FShadowedTextRun(const FRunInfo &InRunInfo, const TSharedRef<const FString> &InText,
                                   const FTextBlockStyle &Style)
    : FSlateTextRun(InRunInfo, InText, Style) {
}

FShadowedTextRun::FShadowedTextRun(const FRunInfo &InRunInfo, const TSharedRef<const FString> &InText,
                                   const FTextBlockStyle &Style, const FTextRange &InRange)
    : FSlateTextRun(InRunInfo, InText, Style, InRange) {
}

int32 FShadowedTextRun::OnPaint(const FPaintArgs &PaintArgs, const FTextArgs &TextArgs,
                                const FGeometry &AllottedGeometry, const FSlateRect &MyCullingRect,
                                FSlateWindowElementList &OutDrawElements, int32 LayerId,
                                const FWidgetStyle &InWidgetStyle, bool bParentEnabled) const {
#if !UE_BUILD_SHIPPING
    if (const auto CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("Slate.LogPaintedText")); CVar->GetBool()) {
        UE_LOG(LogSlate, Log, TEXT("FSlateTextRun: '%s'."), **Text)
    }
#endif
    const ESlateDrawEffect DrawEffects = bParentEnabled ? ESlateDrawEffect::None : ESlateDrawEffect::DisabledEffect;

    const auto &Block = TextArgs.Block;
    const auto &Line = TextArgs.Line;

    const bool ShouldDropShadow = Style.ShadowColorAndOpacity.A > 0.f && Style.ShadowOffset.SizeSquared() > 0.f;
    const auto BlockRange = Block->GetTextRange();
    const auto BlockTextContext = Block->GetTextContext();

    // The block size and offset values are pre-scaled, so we need to account for that when converting the block offsets
    // into paint geometry
    const float InverseScale = Inverse(AllottedGeometry.Scale);

    // A negative shadow offset should be applied as a positive offset to the text to avoid clipping issues
    const FVector2D DrawShadowOffset(
        (Style.ShadowOffset.X > 0.0f) ? Style.ShadowOffset.X * AllottedGeometry.Scale : 0.0f,
        (Style.ShadowOffset.Y > 0.0f) ? Style.ShadowOffset.Y * AllottedGeometry.Scale : 0.0f);
    const FVector2D DrawTextOffset(
        (Style.ShadowOffset.X < 0.0f) ? -Style.ShadowOffset.X * AllottedGeometry.Scale : 0.0f,
        (Style.ShadowOffset.Y < 0.0f) ? -Style.ShadowOffset.Y * AllottedGeometry.Scale : 0.0f);

    // Make sure we have up-to-date shaped text to work with
    // We use the full line view range (rather than the run range) so that text that spans runs will still be shaped
    // correctly
    auto ShapedText = ShapedTextCacheUtil::GetShapedTextSubSequence(
        BlockTextContext.ShapedTextCache,
        FCachedShapedTextKey(Line.Range, AllottedGeometry.GetAccumulatedLayoutTransform().GetScale(), BlockTextContext,
                             Style.Font),
        BlockRange, **Text, BlockTextContext.TextDirection);

    FTextOverflowArgs OverflowArgs;
    if ((TextArgs.OverflowPolicy == ETextOverflowPolicy::Ellipsis ||
         TextArgs.OverflowPolicy == ETextOverflowPolicy::MultilineEllipsis) &&
        TextArgs.OverflowDirection != ETextOverflowDirection::NoOverflow) {
        OverflowArgs.OverflowTextPtr = BlockTextContext.ShapedTextCache->FindOrAddOverflowEllipsisText(
            AllottedGeometry.GetAccumulatedLayoutTransform().GetScale(), BlockTextContext, Style.Font);
        OverflowArgs.OverflowDirection = TextArgs.OverflowDirection;
        OverflowArgs.bIsLastVisibleBlock = TextArgs.bIsLastVisibleBlock;
        OverflowArgs.bIsNextBlockClipped = TextArgs.bIsNextBlockClipped;
    }

    // Draw the optional shadow
    if (ShouldDropShadow) {
        auto ShadowShapedText = ShapedText;
        if (Style.ShadowColorAndOpacity != Style.Font.OutlineSettings.OutlineColor) {
            // Copy font info for shadow to replace the outline color
            auto ShadowFontInfo = Style.Font;
            ShadowFontInfo.OutlineSettings.OutlineColor = Style.ShadowColorAndOpacity;
            ShadowFontInfo.OutlineSettings.OutlineMaterial = nullptr;
            if (!ShadowFontInfo.OutlineSettings.bApplyOutlineToDropShadows) {
                ShadowFontInfo.OutlineSettings.OutlineSize = 0;
            }

            // Create new shaped text for drop shadow
            ShadowShapedText = ShapedTextCacheUtil::GetShapedTextSubSequence(
                BlockTextContext.ShapedTextCache,
                FCachedShapedTextKey(Line.Range, AllottedGeometry.GetAccumulatedLayoutTransform().GetScale(),
                                     BlockTextContext, ShadowFontInfo),
                BlockRange, **Text, BlockTextContext.TextDirection);
        }

        static const std::array ShadowMasks = {FIntVector2(1, 0), FIntVector2(0, 1), FIntVector2(1, 1)};
        for (const auto &Mask : ShadowMasks) {
            FVector2D MaskedOffset(DrawShadowOffset.X * Mask.X, DrawShadowOffset.Y * Mask.Y);
            FSlateDrawElement::MakeShapedText(
                OutDrawElements, LayerId,
                AllottedGeometry.ToPaintGeometry(
                    TransformVector(InverseScale, Block->GetSize()),
                    FSlateLayoutTransform(TransformPoint(InverseScale, Block->GetLocationOffset() + MaskedOffset))),
                ShadowShapedText, DrawEffects, InWidgetStyle.GetColorAndOpacityTint() * Style.ShadowColorAndOpacity,
                InWidgetStyle.GetColorAndOpacityTint() * Style.Font.OutlineSettings.OutlineColor, OverflowArgs);
            LayerId++;
        }
    }

    // Draw the text itself
    FSlateDrawElement::MakeShapedText(
        OutDrawElements, LayerId,
        AllottedGeometry.ToPaintGeometry(
            TransformVector(InverseScale, Block->GetSize()),
            FSlateLayoutTransform(TransformPoint(InverseScale, Block->GetLocationOffset() + DrawTextOffset))),
        ShapedText, DrawEffects, InWidgetStyle.GetColorAndOpacityTint() * Style.ColorAndOpacity.GetColor(InWidgetStyle),
        InWidgetStyle.GetColorAndOpacityTint() * Style.Font.OutlineSettings.OutlineColor, OverflowArgs);
    LayerId++;

    return LayerId;
}