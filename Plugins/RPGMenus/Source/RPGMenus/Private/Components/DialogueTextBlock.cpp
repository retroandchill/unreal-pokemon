// "Unreal Pokémon" created by Retro & Chill.


#include "Components/DialogueTextBlock.h"
#include "Framework/Text/RichTextLayoutMarshaller.h"
#include "Widgets/Text/SRichTextBlock.h"

TSharedRef<SWidget> UDialogueTextBlock::RebuildWidget() {
    // Copied from URichTextBlock::RebuildWidget
    UpdateStyleData();

    TArray<TSharedRef<ITextDecorator>> CreatedDecorators;
    CreateDecorators(CreatedDecorators);

    TextMarshaller = FRichTextLayoutMarshaller::Create(CreateMarkupParser(), CreateMarkupWriter(), CreatedDecorators, StyleInstance.Get());

    MyRichTextBlock =
        SNew(SRichTextBlock)
        .TextStyle(bOverrideDefaultStyle ? &GetDefaultTextStyleOverride() : &DefaultTextStyle)
        .Marshaller(TextMarshaller)
        .CreateSlateTextLayout(
            FCreateSlateTextLayout::CreateWeakLambda(this, [this] (SWidget* InOwner, const FTextBlockStyle& InDefaultTextStyle) mutable {
                TextLayout = FSlateTextLayout::Create(InOwner, InDefaultTextStyle);
                return StaticCastSharedPtr<FSlateTextLayout>(TextLayout).ToSharedRef();
            }));

    return MyRichTextBlock.ToSharedRef();
}