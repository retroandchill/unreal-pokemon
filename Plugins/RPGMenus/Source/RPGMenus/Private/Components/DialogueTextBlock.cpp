// "Unreal Pokémon" created by Retro & Chill.

#include "Components/DialogueTextBlock.h"
#include "Framework/Text/RichTextLayoutMarshaller.h"
#include "Widgets/Text/SRichTextBlock.h"

TSharedRef<SWidget> UDialogueTextBlock::RebuildWidget() {
    // Copied from URichTextBlock::RebuildWidget
    UpdateStyleData();

    TArray<TSharedRef<ITextDecorator>> CreatedDecorators;
    CreateDecorators(CreatedDecorators);
    TextMarshaller = CreateMarshaller(CreatedDecorators);

    MyRichTextBlock = SNew(SRichTextBlock)
                          .TextStyle(bOverrideDefaultStyle ? &GetDefaultTextStyleOverride() : &DefaultTextStyle)
                          .Marshaller(TextMarshaller)
                          .CreateSlateTextLayout(FCreateSlateTextLayout::CreateWeakLambda(
                              this, [this](SWidget *InOwner, const FTextBlockStyle &InDefaultTextStyle) mutable {
                                  TextLayout = CreateLayout(InOwner, InDefaultTextStyle);
                                  return StaticCastSharedPtr<FSlateTextLayout>(TextLayout).ToSharedRef();
                              }));

    return MyRichTextBlock.ToSharedRef();
}

TSharedRef<FRichTextLayoutMarshaller>
UDialogueTextBlock::CreateMarshaller(const TArray<TSharedRef<ITextDecorator>> &CreatedDecorators) {
    return FRichTextLayoutMarshaller::Create(CreateMarkupParser(), CreateMarkupWriter(), CreatedDecorators,
                                             StyleInstance.Get());
}

TSharedRef<FSlateTextLayout> UDialogueTextBlock::CreateLayout(SWidget *InOwner,
                                                              const FTextBlockStyle &InDefaultTextStyle) {
    return FSlateTextLayout::Create(InOwner, InDefaultTextStyle);
}