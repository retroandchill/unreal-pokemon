// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "AnimGraphNodeDetailCustomization.h"
#include "Graphs/Nodes/PaperZDAnimGraphNode_Base.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "ObjectEditorUtils.h"
#include "Widgets/Input/SCheckBox.h" 	
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "PaperZDAnimGraphNodeDetailCustomization"

/////////////////////////////////////////////////////
// FPaperZDAnimGraphNodeBindingExtension
void FPaperZDAnimGraphNodeBindingExtension::GetOptionalPinData(const IPropertyHandle& PropertyHandle, int32& OutOptionalPinIndex, UPaperZDAnimGraphNode_Base*& OutAnimGraphNode) const
{
	OutOptionalPinIndex = INDEX_NONE;

	TArray<UObject*> Objects;
	PropertyHandle.GetOuterObjects(Objects);

	FProperty* Property = PropertyHandle.GetProperty();
	if (Property)
	{
		OutAnimGraphNode = Cast<UPaperZDAnimGraphNode_Base>(Objects[0]);
		if (OutAnimGraphNode != nullptr)
		{
			OutOptionalPinIndex = OutAnimGraphNode->ShowPinForProperties.IndexOfByPredicate([Property](const FOptionalPinFromProperty& InOptionalPin)
				{
					return Property->GetFName() == InOptionalPin.PropertyName;
				});
		}
	}
}

bool FPaperZDAnimGraphNodeBindingExtension::IsPropertyExtendable(const UClass* InObjectClass, const IPropertyHandle& PropertyHandle) const
{
	int32 OptionalPinIndex;
	UPaperZDAnimGraphNode_Base* AnimGraphNode;
	GetOptionalPinData(PropertyHandle, OptionalPinIndex, AnimGraphNode);

	if (OptionalPinIndex != INDEX_NONE)
	{
		const FOptionalPinFromProperty& OptionalPin = AnimGraphNode->ShowPinForProperties[OptionalPinIndex];

		// Not optional
		if (!OptionalPin.bCanToggleVisibility && OptionalPin.bShowPin)
		{
			return false;
		}

		if (!PropertyHandle.GetProperty())
		{
			return false;
		}

		return OptionalPin.bCanToggleVisibility;
	}

	return false;
}

// Legacy binding widget (No Property Binding)
class SShowAsWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SShowAsWidget) {}

	SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, const TSharedRef<IPropertyHandle>& InPropertyHandle)
	{
		PropertyHandle = InPropertyHandle;

		TSharedRef<SHorizontalBox> HorizontalBox =
			SNew(SHorizontalBox)
			.ToolTipText(LOCTEXT("AsPinTooltip", "Show/hide this property as a pin on the node"));

		TWeakPtr<SWidget> WeakHorizontalBox = HorizontalBox;

		HorizontalBox->AddSlot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("ExposeAsPinLabel", "Expose"))
			.Font(IDetailLayoutBuilder::GetDetailFont())
			.Visibility_Lambda([WeakHorizontalBox]() { return WeakHorizontalBox.IsValid() && WeakHorizontalBox.Pin()->IsHovered() ? EVisibility::Visible : EVisibility::Collapsed; })
			];

		HorizontalBox->AddSlot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			.Padding(3.0f, 0.0f, 0.0f, 0.0f)
			[
				SNew(SCheckBox)
				.IsChecked(this, &SShowAsWidget::IsChecked)
			.OnCheckStateChanged(this, &SShowAsWidget::OnCheckStateChanged)
			];

		ChildSlot
			[
				HorizontalBox
			];
	}

	ECheckBoxState IsChecked() const
	{
		bool bValue;
		FPropertyAccess::Result Result = PropertyHandle->GetValue(bValue);
		if (Result == FPropertyAccess::MultipleValues)
		{
			return ECheckBoxState::Undetermined;
		}
		else
		{
			return bValue ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
		}
	}

	void OnCheckStateChanged(ECheckBoxState InCheckBoxState)
	{
		bool bValue = InCheckBoxState == ECheckBoxState::Checked;
		PropertyHandle->SetValue(bValue);
	}

	TSharedPtr<IPropertyHandle> PropertyHandle;
};


TSharedRef<SWidget> FPaperZDAnimGraphNodeBindingExtension::GenerateExtensionWidget(const IDetailLayoutBuilder& InDetailBuilder, const UClass* InObjectClass, TSharedPtr<IPropertyHandle> InPropertyHandle)
{
	int32 OptionalPinIndex;
	UPaperZDAnimGraphNode_Base* AnimGraphNode;
	GetOptionalPinData(*InPropertyHandle.Get(), OptionalPinIndex, AnimGraphNode);
	check(OptionalPinIndex != INDEX_NONE);

	FProperty* AnimNodeProperty = InPropertyHandle->GetProperty();
	const FName PropertyName = AnimNodeProperty->GetFName();

	const FName OptionalPinArrayEntryName(*FString::Printf(TEXT("ShowPinForProperties[%d].bShowPin"), OptionalPinIndex));
	TSharedRef<IPropertyHandle> ShowPinPropertyHandle = InDetailBuilder.GetProperty(OptionalPinArrayEntryName, UPaperZDAnimGraphNode_Base::StaticClass());
	ShowPinPropertyHandle->MarkHiddenByCustomization();

	//No property access available, so we can just show the simple widget
	return SNew(SShowAsWidget, ShowPinPropertyHandle);
}

/////////////////////////////////////////////////////
// FPaperZDAnimGraphNodeDetailCustomization 
TSharedRef<IDetailCustomization> FPaperZDAnimGraphNodeDetailCustomization::MakeInstance()
{
	return MakeShareable(new FPaperZDAnimGraphNodeDetailCustomization);
}

void FPaperZDAnimGraphNodeDetailCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	TArray<TWeakObjectPtr<UObject>> SelectedObjectsList;
	DetailBuilder.GetObjectsBeingCustomized(SelectedObjectsList);

	// Hide the pin options property; it's represented inline per-property instead
	IDetailCategoryBuilder& PinOptionsCategory = DetailBuilder.EditCategory("PinOptions");
	TSharedRef<IPropertyHandle> AvailablePins = DetailBuilder.GetProperty("ShowPinForProperties");
	DetailBuilder.HideProperty(AvailablePins);
	TSharedRef<IPropertyHandle> PropertyBindings = DetailBuilder.GetProperty("PropertyBindings");
	DetailBuilder.HideProperty(PropertyBindings);

	// get first animgraph nodes
	UPaperZDAnimGraphNode_Base* AnimGraphNode = Cast<UPaperZDAnimGraphNode_Base>(SelectedObjectsList[0].Get());
	if (AnimGraphNode == nullptr)
	{
		return;
	}

	// make sure type matches with all the nodes. 
	const UPaperZDAnimGraphNode_Base* FirstNodeType = AnimGraphNode;
	for (int32 Index = 1; Index < SelectedObjectsList.Num(); ++Index)
	{
		UPaperZDAnimGraphNode_Base* CurrentNode = Cast<UPaperZDAnimGraphNode_Base>(SelectedObjectsList[Index].Get());
		if (!CurrentNode || CurrentNode->GetClass() != FirstNodeType->GetClass())
		{
			// if type mismatches, multi selection doesn't work, just return
			return;
		}
	}

	// Get the node property
	const FStructProperty* NodeProperty = AnimGraphNode->GetFNodeProperty();
	if (NodeProperty == nullptr)
	{
		return;
	}

	// customize anim graph node's own details if needed
	AnimGraphNode->CustomizeDetails(DetailBuilder);

	// Hide the Node property as we are going to be adding its inner properties below
	TSharedRef<IPropertyHandle> NodePropertyHandle = DetailBuilder.GetProperty(NodeProperty->GetFName(), AnimGraphNode->GetClass());
	DetailBuilder.HideProperty(NodePropertyHandle);

	uint32 NumChildHandles = 0;
	FPropertyAccess::Result Result = NodePropertyHandle->GetNumChildren(NumChildHandles);
	if (Result != FPropertyAccess::Fail)
	{
		for (uint32 ChildHandleIndex = 0; ChildHandleIndex < NumChildHandles; ++ChildHandleIndex)
		{
			TSharedPtr<IPropertyHandle> TargetPropertyHandle = NodePropertyHandle->GetChildHandle(ChildHandleIndex);
			if (TargetPropertyHandle.IsValid())
			{
				FProperty* TargetProperty = TargetPropertyHandle->GetProperty();
				IDetailCategoryBuilder& CurrentCategory = DetailBuilder.EditCategory(FObjectEditorUtils::GetCategoryFName(TargetProperty));

				int32 CustomPinIndex = AnimGraphNode->ShowPinForProperties.IndexOfByPredicate([TargetProperty](const FOptionalPinFromProperty& InOptionalPin)
					{
						return TargetProperty->GetFName() == InOptionalPin.PropertyName;
					});

				if (CustomPinIndex != INDEX_NONE)
				{
					const FOptionalPinFromProperty& OptionalPin = AnimGraphNode->ShowPinForProperties[CustomPinIndex];

					// Not optional
					if (!OptionalPin.bCanToggleVisibility && OptionalPin.bShowPin)
					{
						// Always displayed as a pin, so hide the property
						DetailBuilder.HideProperty(TargetPropertyHandle);
						continue;
					}

					if (!TargetPropertyHandle->GetProperty())
					{
						continue;
					}

					// if customized, do not do anything
					if (TargetPropertyHandle->IsCustomized())
					{
						continue;
					}

					// sometimes because of order of customization
					// this gets called first for the node you'd like to customize
					// then the above statement won't work
					// so you can mark certain property to have meta data "CustomizeProperty"
					// which will trigger below statement
					if (OptionalPin.bPropertyIsCustomized)
					{
						continue;
					}

					//Check if this property is "feature dependent"
					if (TargetProperty->HasMetaData("PropertyFeature") && !AnimGraphNode->IsPropertyFeatureActive(*TargetProperty->GetMetaData("PropertyFeature")))
					{
						continue;
					}

					TSharedRef<SWidget> InternalCustomWidget = CreatePropertyWidget(TargetProperty, TargetPropertyHandle.ToSharedRef(), AnimGraphNode->GetClass());

					if (OptionalPin.bCanToggleVisibility)
					{
						IDetailPropertyRow& PropertyRow = CurrentCategory.AddProperty(TargetPropertyHandle);

						TSharedPtr<SWidget> NameWidget;
						TSharedPtr<SWidget> ValueWidget;
						FDetailWidgetRow Row;
						PropertyRow.GetDefaultWidgets(NameWidget, ValueWidget, Row);

						ValueWidget = (InternalCustomWidget == SNullWidget::NullWidget) ? ValueWidget : InternalCustomWidget;

						const FName OptionalPinArrayEntryName(*FString::Printf(TEXT("ShowPinForProperties[%d].bShowPin"), CustomPinIndex));
						TSharedRef<IPropertyHandle> ShowHidePropertyHandle = DetailBuilder.GetProperty(OptionalPinArrayEntryName);

						ShowHidePropertyHandle->MarkHiddenByCustomization();

						ValueWidget->SetVisibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateSP(this, &FPaperZDAnimGraphNodeDetailCustomization::GetVisibilityOfProperty, ShowHidePropertyHandle)));

						// If we have an edit condition, that comes as part of the default name widget, so just use a text block to avoid duplicate checkboxes
						TSharedPtr<SWidget> PropertyNameWidget;
						if (TargetProperty->HasMetaData(TEXT("EditCondition")))
						{
							PropertyNameWidget = SNew(STextBlock)
								.Text(TargetProperty->GetDisplayNameText())
								.Font(IDetailLayoutBuilder::GetDetailFont())
								.ToolTipText(TargetProperty->GetToolTipText());
						}
						else
						{
							PropertyNameWidget = NameWidget;
						}

						NameWidget = PropertyNameWidget;

						// we only show children if visibility is one
						// whenever toggles, this gets called, so it will be refreshed
						const bool bShowChildren = GetVisibilityOfProperty(ShowHidePropertyHandle) == EVisibility::Visible;
						PropertyRow.CustomWidget(bShowChildren)
							.NameContent()
							.MinDesiredWidth(Row.NameWidget.MinWidth)
							.MaxDesiredWidth(Row.NameWidget.MaxWidth)
							[
								NameWidget.ToSharedRef()
							]
						.ValueContent()
							.MinDesiredWidth(Row.ValueWidget.MinWidth)
							.MaxDesiredWidth(Row.ValueWidget.MaxWidth)
							[
								ValueWidget.ToSharedRef()
							];
					}
					else if (InternalCustomWidget != SNullWidget::NullWidget)
					{
						// A few properties are internally customized within this customization. Here we
						// catch instances of these that don't have an optional pin flag.
						IDetailPropertyRow& PropertyRow = CurrentCategory.AddProperty(TargetPropertyHandle);
						PropertyRow.CustomWidget()
							.NameContent()
							[
								TargetPropertyHandle->CreatePropertyNameWidget()
							]
						.ValueContent()
							[
								InternalCustomWidget
							];
					}
					else
					{
						CurrentCategory.AddProperty(TargetPropertyHandle);
					}
				}
			}
		}
	}
}

TSharedRef<SWidget> FPaperZDAnimGraphNodeDetailCustomization::CreatePropertyWidget(FProperty* TargetProperty, TSharedRef<IPropertyHandle> TargetPropertyHandle, UClass* NodeClass)
{
	//@TODO: Add custom selector for AnimSequences (filtered by this AnimBP)
	return SNullWidget::NullWidget;
}

EVisibility FPaperZDAnimGraphNodeDetailCustomization::GetVisibilityOfProperty(TSharedRef<IPropertyHandle> Handle) const
{
	bool bShowAsPin;
	if (FPropertyAccess::Success == Handle->GetValue(/*out*/ bShowAsPin))
	{
		return bShowAsPin ? EVisibility::Hidden : EVisibility::Visible;
	}
	else
	{
		return EVisibility::Visible;
	}
}
#undef LOCTEXT_NAMESPACE
