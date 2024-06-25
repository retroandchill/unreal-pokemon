// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Misc/TextFilter.h"
#include "Widgets/SCompoundWidget.h"


DECLARE_DELEGATE_OneParam(FOnAttributePicked, FProperty*);

namespace Pokemon {
struct FAttributeViewerNode;

/**
 * 
 */
class POKEMONEDITORTOOLS_API SAttributeListWidget : public SCompoundWidget {
public:
    SLATE_BEGIN_ARGS(SAttributeListWidget) {
    }

    SLATE_ARGUMENT(FOnAttributePicked, OnAttributePickedDelegate)
        SLATE_ARGUMENT(UScriptStruct*, StructType)

    SLATE_END_ARGS()

    /**
    * Construct the widget
    *
    * @param	InArgs			A declaration from which to construct the widget
    */
    void Construct(const FArguments& InArgs);

    virtual ~SAttributeListWidget();

private:
    typedef TTextFilter< const FProperty& > FAttributeTextFilter;

    /** Called by Slate when the filter box changes text. */
    void OnFilterTextChanged(const FText& InFilterText);

    /** Creates the row widget when called by Slate when an item appears on the list. */
    TSharedRef< ITableRow > OnGenerateRowForAttributeViewer(TSharedPtr<FAttributeViewerNode> Item, const TSharedRef< STableViewBase >& OwnerTable);

    /** Called by Slate when an item is selected from the tree/list. */
    void OnAttributeSelectionChanged(TSharedPtr<FAttributeViewerNode> Item, ESelectInfo::Type SelectInfo);

    /** Updates the list of items in the dropdown menu */
    TSharedPtr<FAttributeViewerNode> UpdatePropertyOptions();

    /** Delegate to be called when an attribute is picked from the list */
    FOnAttributePicked OnAttributePicked;

    /** The search box */
    TSharedPtr<SSearchBox> SearchBoxPtr;

    /** Holds the Slate List widget which holds the attributes for the Attribute Viewer. */
    TSharedPtr<SListView<TSharedPtr< FAttributeViewerNode > >> AttributeList;

    /** Array of items that can be selected in the dropdown menu */
    TArray<TSharedPtr<FAttributeViewerNode>> PropertyOptions;

    /** Filters needed for filtering the assets */
    TSharedPtr<FAttributeTextFilter> AttributeTextFilter;

    TObjectPtr<UScriptStruct> StructType;
};
}