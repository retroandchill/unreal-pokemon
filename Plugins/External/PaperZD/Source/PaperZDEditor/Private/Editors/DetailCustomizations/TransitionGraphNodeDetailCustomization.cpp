// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "TransitionGraphNodeDetailCustomization.h"
#include "Graphs/Nodes/PaperZDStateGraphNode_Transition.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "Widgets/Input/SButton.h" 	
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Text/STextBlock.h" 	

#define LOCTEXT_NAMESPACE "PaperZDTransitionGraphNodeDetailCustomization"

//Widget class for the inlined TransitionalAnimGraph button
class STransitionalAnimGraphWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(STransitionalAnimGraphWidget){}
	SLATE_EVENT(FSimpleDelegate, OnOpenAnimGraphButtonClicked)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, const TSharedRef<IPropertyHandle>& InPropertyHandle)
	{
		PropertyHandle = InPropertyHandle;

		TSharedRef<SHorizontalBox> HorizontalBox =
			SNew(SHorizontalBox)
			.ToolTipText(LOCTEXT("EnableTransitionalAnimationsTooltip", "Enables a transitional animation graph to play a non-looping animation between states."));

		TWeakPtr<SWidget> WeakHorizontalBox = HorizontalBox;
		HorizontalBox->AddSlot()
 			.AutoWidth()
 			.VAlign(VAlign_Center)
 			.Padding(3.0f, 0.0f, 0.0f, 0.0f)
 			[
 				SNew(SCheckBox)
 				.IsChecked(this, &STransitionalAnimGraphWidget::IsChecked)
 				.OnCheckStateChanged(this, &STransitionalAnimGraphWidget::OnCheckboxStateChange)
 			];

		HorizontalBox->AddSlot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			.Padding(5.0f, 0.0f, 5.0f, 0.0f)
			[
				SNew(SButton)
				.Text(LOCTEXT("OpenAnimTransitionalGraph", "Open Animation Graph"))
				.OnPressed(InArgs._OnOpenAnimGraphButtonClicked)
				.IsEnabled(this, &STransitionalAnimGraphWidget::IsAnimGraphButtonEnabled)
			];

		//Add the widget to the child
		ChildSlot
		[
			HorizontalBox
		];
	}

	/* Returns the checkbox for enabling transitional animations and the graph access button. */
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

	/* Returns if the OpenAnimGraph button should be interactable. */
	bool IsAnimGraphButtonEnabled() const
	{
		bool bValue;
		PropertyHandle->GetValue(bValue);
		return bValue;
	}

	/* Updates the checkbox state when the value changes. */
	void OnCheckboxStateChange(ECheckBoxState InCheckBoxState)
	{
		bool bValue = InCheckBoxState == ECheckBoxState::Checked;
		PropertyHandle->SetValue(bValue);
	}

	//Property handle pointing to the "bPlayTransitionalAnimations" property of the transition
	TSharedPtr<IPropertyHandle> PropertyHandle;
};

/////////////////////////////////////////////////////
// FPaperZDTransitionGraphNodeDetailCustomization 
TSharedRef<IDetailCustomization> FPaperZDTransitionGraphNodeDetailCustomization::MakeInstance()
{
	return MakeShareable(new FPaperZDTransitionGraphNodeDetailCustomization);
}

void FPaperZDTransitionGraphNodeDetailCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	TArray<TWeakObjectPtr<UObject>> SelectedObjectsList;
	DetailBuilder.GetObjectsBeingCustomized(SelectedObjectsList);

	// get first animgraph nodes
	UPaperZDStateGraphNode_Transition* TransitionGraphNode = Cast<UPaperZDStateGraphNode_Transition>(SelectedObjectsList[0].Get());
	if (TransitionGraphNode == nullptr)
	{
		return;
	}

	// make sure type matches with all the nodes. 
	const UPaperZDStateGraphNode_Transition* FirstNodeType = TransitionGraphNode;
	for (int32 Index = 1; Index < SelectedObjectsList.Num(); ++Index)
	{
		UPaperZDStateGraphNode_Transition* CurrentNode = Cast<UPaperZDStateGraphNode_Transition>(SelectedObjectsList[Index].Get());
		if (!CurrentNode || CurrentNode->GetClass() != FirstNodeType->GetClass())
		{
			// if type mismatches, multi selection doesn't work, just return
			return;
		}
	}

	//Hide the normal bool property, as we're going to create an in-line property
	IDetailCategoryBuilder& AnimationCategory = DetailBuilder.EditCategory("Animation");
	TSharedRef<IPropertyHandle> TransitionalAnimationsProperty = DetailBuilder.GetProperty("bPlayTransitionalAnimations");
	DetailBuilder.HideProperty(TransitionalAnimationsProperty);
	
	//Disallow multi-selection for the transitional anim graphs
	if (SelectedObjectsList.Num() == 1)
	{
		TSharedRef<SWidget> CustomWidget = SNew(STransitionalAnimGraphWidget, TransitionalAnimationsProperty)
												.OnOpenAnimGraphButtonClicked(FSimpleDelegate::CreateUObject(TransitionGraphNode, &UPaperZDStateGraphNode_Transition::OpenAnimTransitionalGraph));

		FDetailWidgetRow& CustomRow = AnimationCategory.AddCustomRow(FText::FromString(TEXT("Animations")));
		CustomRow.NameContent()
			[
				SNew(STextBlock)
				.Text(LOCTEXT("EnableTransitionalAnimationsLabel", "Use Transitional Animations"))
				.ToolTipText(LOCTEXT("EnableTransitionalAnimationsTooltip", "Enables a transitional animation graph to play a non-looping animation between states."))
				.Font(DetailBuilder.GetDetailFont())
			];

		CustomRow.ValueContent()
			[
				CustomWidget
			];
	}
}
#undef LOCTEXT_NAMESPACE
