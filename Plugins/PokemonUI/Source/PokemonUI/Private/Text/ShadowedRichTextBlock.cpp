// "Unreal Pokémon" created by Retro & Chill.

#include "Text/ShadowedRichTextBlock.h"
#include "Framework/Text/RichTextLayoutMarshaller.h"
#include "RenderDeferredCleanup.h"
#include "Text/ShadowedRichTextLayoutMarshaller.h"
#include "Text/ShadowedTextLayout.h"
#include "Widgets/Text/SRichTextBlock.h"

/**
 * Templated deleter for handling deferred deletion of memory.
 * @tparam ObjectType The type of object that is being deleted
 */
template <class ObjectType>
struct TDeferredDeleter : FDeferredCleanupInterface {
    /**
     * Construct the deleter object from the memory
     * @param InInnerObjectToDelete The memory to delete after processing is done
     */
    explicit TDeferredDeleter(ObjectType *InInnerObjectToDelete) : InnerObjectToDelete(InInnerObjectToDelete) {
    }

  private:
    /**
     * Unique pointer around the memory. Used to automatically handle deletion.
     */
    TUniquePtr<ObjectType> InnerObjectToDelete;
};

/**
 * Create a shared pointer with a custom deleter for deferred deleting.
 * @tparam ObjectType The object type to delete.
 * @tparam ArgsType The constructor arguments used when initializing the object.
 * @param Args The constructor arguments
 * @return The created pointer.
 */
template <class ObjectType, typename... ArgsType>
static TSharedPtr<ObjectType> MakeShareableDeferredCleanup(ArgsType... Args) {
    return MakeShareable(new ObjectType(Args...), [](ObjectType *ObjectToDelete) {
        BeginCleanup(new TDeferredDeleter<ObjectType>(ObjectToDelete));
    });
}

TSharedRef<SWidget> UShadowedRichTextBlock::RebuildWidget() {
    UpdateStyleData();

    TArray<TSharedRef<class ITextDecorator>> CreatedDecorators;
    CreateDecorators(CreatedDecorators);

    auto Marshaller = MakeShared<FShadowedRichTextLayoutMarshaller>(CreateMarkupParser(), CreateMarkupWriter(),
                                                                    CreatedDecorators, StyleInstance.Get());
    MyRichTextBlock = SNew(SRichTextBlock)
                          .CreateSlateTextLayout_Lambda([](SWidget *Owner, const FTextBlockStyle &TextBlockStyle) {
                              return MakeShared<FShadowedTextLayout>(Owner, TextBlockStyle);
                          })
                          .TextStyle(bOverrideDefaultStyle ? &GetDefaultTextStyleOverride() : &DefaultTextStyle)
                          .Marshaller(Marshaller);
    return MyRichTextBlock.ToSharedRef();
}