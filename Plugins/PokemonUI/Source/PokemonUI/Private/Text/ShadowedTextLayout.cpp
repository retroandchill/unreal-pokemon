// "Unreal Pokémon" created by Retro & Chill.

#include "Text/ShadowedTextLayout.h"
#include "Text/ShadowedTextRun.h"

FShadowedTextLayout::FShadowedTextLayout(SWidget *InOwner, const FTextBlockStyle &InDefaultTextStyle)
    : FSlateTextLayout(InOwner, InDefaultTextStyle) {
}

TSharedRef<IRun> FShadowedTextLayout::CreateDefaultTextRun(const TSharedRef<FString> &NewText,
                                                           const FTextRange &NewRange) const {
    return MakeShared<FShadowedTextRun>(FRunInfo(), NewText, DefaultTextStyle, NewRange);
}