// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

namespace Pokemon {

struct FAttributeViewerNode {
    FAttributeViewerNode(FProperty* InAttribute, FString InAttributeName);

    /** The displayed name for this node. */
    TSharedPtr<FString> AttributeName;

    FProperty* Attribute;
};

/**
 * 
 */
class POKEMONEDITORTOOLS_API SAttributeItem : public SComboRow< TSharedPtr<FAttributeViewerNode> > {
public:
    SLATE_BEGIN_ARGS(SAttributeItem)
        : _TextColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f))
    {}

    /** The text this item should highlight, if any. */
    SLATE_ARGUMENT(FText, HighlightText)
    /** The color text this item will use. */
    SLATE_ARGUMENT(FSlateColor, TextColor)
    /** The node this item is associated with. */
    SLATE_ARGUMENT(TSharedPtr<FAttributeViewerNode>, AssociatedNode)

    SLATE_END_ARGS()

    /**
    * Construct the widget
    *
    * @param InArgs   A declaration from which to construct the widget
    */
    void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTableView);

    /** Returns the text color for the item based on if it is selected or not. */
    FSlateColor GetTextColor() const;

private:

    /** The text color for this item. */
    FSlateColor TextColor;

    /** The Attribute Viewer Node this item is associated with. */
    TSharedPtr< FAttributeViewerNode > AssociatedNode;
};
}