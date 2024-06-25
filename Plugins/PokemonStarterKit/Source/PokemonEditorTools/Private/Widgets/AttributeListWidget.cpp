// "Unreal Pokémon" created by Retro & Chill.


#include "Widgets/AttributeListWidget.h"

#include "SlateOptMacros.h"
#include "Battle/GameplayAbilities/Events/BattleEvent.h"
#include "Widgets/AttributeItem.h"
#include "Widgets/Input/SSearchBox.h"

using namespace Pokemon;

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

SAttributeListWidget::~SAttributeListWidget() {
	if (OnAttributePicked.IsBound()) {
		OnAttributePicked.Unbind();
	}
}

void SAttributeListWidget::Construct(const FArguments& InArgs) {
	auto AttributeCallback = [this](const FProperty& Property, OUT TArray< FString >& StringArray) {
	    if (auto Class = Property.GetOwnerClass(); Class->IsChildOf(StructType)
	        && !Class->ClassGeneratedBy) {
	        StringArray.Add(FString::Printf(TEXT("%s.%s"), *Class->GetName(), *Property.GetName()));
	    }
	};
    
	OnAttributePicked = InArgs._OnAttributePickedDelegate;
    StructType = InArgs._StructType;

	// Setup text filtering
	AttributeTextFilter = MakeShareable(new FAttributeTextFilter(FAttributeTextFilter::FItemToStringArray::CreateLambda(AttributeCallback)));

	UpdatePropertyOptions();

	TSharedPtr< SWidget > ClassViewerContent;

	SAssignNew(ClassViewerContent, SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SAssignNew(SearchBoxPtr, SSearchBox)
			.HintText(NSLOCTEXT("EventValues", "SearchBoxHint", "Search Attributes"))
			.OnTextChanged(this, &SAttributeListWidget::OnFilterTextChanged)
			.DelayChangeNotificationsWhileTyping(true)
		]

	+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SSeparator)
			.Visibility(EVisibility::Collapsed)
		]
	
	+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		[
			SAssignNew(AttributeList, SListView<TSharedPtr< FAttributeViewerNode > >)
			.Visibility(EVisibility::Visible)
			.SelectionMode(ESelectionMode::Single)
			.ListItemsSource(&PropertyOptions)

 			// Generates the actual widget for a tree item
			.OnGenerateRow(this, &SAttributeListWidget::OnGenerateRowForAttributeViewer)

 			// Find out when the user selects something in the tree
			.OnSelectionChanged(this, &SAttributeListWidget::OnAttributeSelectionChanged)
		];


	ChildSlot
		[
			ClassViewerContent.ToSharedRef()
		];
}

TSharedRef< ITableRow > SAttributeListWidget::OnGenerateRowForAttributeViewer(TSharedPtr<FAttributeViewerNode> Item, const TSharedRef< STableViewBase >& OwnerTable) {
	TSharedRef< SAttributeItem > ReturnRow = SNew(SAttributeItem, OwnerTable)
		.HighlightText(SearchBoxPtr->GetText())
		.TextColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.f))
		.AssociatedNode(Item);

	return ReturnRow;
}

TSharedPtr<FAttributeViewerNode> SAttributeListWidget::UpdatePropertyOptions() {
	PropertyOptions.Empty();
	TSharedPtr<FAttributeViewerNode> InitiallySelected = MakeShareable(new FAttributeViewerNode(nullptr, "None"));

	PropertyOptions.Add(InitiallySelected);

	// Gather all UAttribute classes
	for (TObjectIterator<UScriptStruct> ClassIt; ClassIt; ++ClassIt) {
	    auto Class = *ClassIt; 
        if (!Class->IsChildOf(StructType)) {
            continue;
        }
	    
        // Allow entire classes to be filtered globally
        if (Class->HasMetaData(TEXT("HideInDetailsView"))) {
            continue;
        }

        for (TFieldIterator<FProperty> PropertyIt(Class, EFieldIteratorFlags::ExcludeSuper); PropertyIt; ++PropertyIt) {
            FProperty *Property = *PropertyIt;

            // if we have a search string and this doesn't match, don't show it
            if (AttributeTextFilter.IsValid() && !AttributeTextFilter->PassesFilter(*Property)) {
                continue;
            }

            // Allow properties to be filtered globally (never show up)
            if (Property->HasMetaData(TEXT("HideInDetailsView"))) {
                continue;
            }

            TSharedPtr<FAttributeViewerNode> SelectableProperty = MakeShareable(new FAttributeViewerNode(Property, FString::Printf(TEXT("%s.%s"), *Class->GetName(), *Property->GetName())));
            PropertyOptions.Add(SelectableProperty);
        }
    }

	return InitiallySelected;
}

void SAttributeListWidget::OnFilterTextChanged(const FText& InFilterText)
{
	AttributeTextFilter->SetRawFilterText(InFilterText);
	SearchBoxPtr->SetError(AttributeTextFilter->GetFilterErrorText());

	UpdatePropertyOptions();
}

void SAttributeListWidget::OnAttributeSelectionChanged(TSharedPtr<FAttributeViewerNode> Item, ESelectInfo::Type SelectInfo)
{
	OnAttributePicked.ExecuteIfBound(Item->Attribute);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION