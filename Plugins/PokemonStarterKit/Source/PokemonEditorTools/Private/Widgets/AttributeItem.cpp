// "Unreal Pokémon" created by Retro & Chill.


#include "Widgets/AttributeItem.h"

#include "SlateOptMacros.h"

using namespace Pokemon;

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

FAttributeViewerNode::FAttributeViewerNode(FProperty *InAttribute, FString InAttributeName) :
    AttributeName(MakeShared<FString>(MoveTemp(InAttributeName))),
    Attribute(InAttribute) {
}

void SAttributeItem::Construct(const FArguments &InArgs, const TSharedRef<STableViewBase> &InOwnerTableView) {
    AssociatedNode = InArgs._AssociatedNode;

    this->ChildSlot
        [
            SNew(SHorizontalBox)

            + SHorizontalBox::Slot()
                .FillWidth(1.0f)
                .Padding(0.0f, 3.0f, 6.0f, 3.0f)
                .VAlign(VAlign_Center)
                [
                    SNew(STextBlock)
                    .Text(FText::FromString(*AssociatedNode->AttributeName.Get()))
                    .HighlightText(InArgs._HighlightText)
                    .ColorAndOpacity(this, &SAttributeItem::GetTextColor)
                    .IsEnabled(true)
                ]
        ];

    TextColor = InArgs._TextColor;

    ConstructInternal(STableRow::FArguments().ShowSelection(true), InOwnerTableView);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

FSlateColor SAttributeItem::GetTextColor() const {
    const TSharedPtr< ITypedTableView< TSharedPtr<FAttributeViewerNode> > > OwnerWidget = OwnerTablePtr.Pin();
    const TSharedPtr<FAttributeViewerNode>* MyItem = OwnerWidget->Private_ItemFromWidget(this);
    const bool bIsSelected = OwnerWidget->Private_IsItemSelected(*MyItem);

    if (bIsSelected) {
        return FSlateColor::UseForeground();
    }

    return TextColor;
}