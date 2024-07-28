// "Unreal Pokémon" created by Retro & Chill.

#include "Text/ShadowedRichTextBlock.h"
#include "Framework/Text/RichTextLayoutMarshaller.h"
#include "RenderDeferredCleanup.h"
#include "Text/ShadowedRichTextLayoutMarshaller.h"
#include "Text/ShadowedTextLayout.h"
#include "Widgets/Text/SRichTextBlock.h"

TSharedRef<FRichTextLayoutMarshaller> UShadowedRichTextBlock::CreateMarshaller(
    const TArray<TSharedRef<ITextDecorator>> &CreatedDecorators) {
    return MakeShared<FShadowedRichTextLayoutMarshaller>(CreateMarkupParser(), CreateMarkupWriter(),  CreatedDecorators, StyleInstance.Get());
}

TSharedRef<FSlateTextLayout> UShadowedRichTextBlock::CreateLayout(SWidget *InOwner, const FTextBlockStyle &InDefaultTextStyle) {
    return MakeShared<FShadowedTextLayout>(InOwner, InDefaultTextStyle);
}