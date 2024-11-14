// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "AnimSequenceDetailCustomization.h"
#include "AnimSequences/PaperZDAnimSequence.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "Editors/Slate/SPaperZDAnimDataSourceGrid.h"
#include "IPropertyUtilities.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SSpacer.h"
#include "Widgets/Input/SNumericEntryBox.h" 	
#include "ScopedTransaction.h"

#define MIN_NUM_NODES 3
#define MAX_NUM_NODES 16

#define LOCTEXT_NAMESPACE "ZDAnimSequenceDetailCustomization"

TSharedRef<IDetailCustomization> FPaperZDAnimSequenceDetailCustomization::MakeInstance()
{
	return MakeShareable(new FPaperZDAnimSequenceDetailCustomization);
}

void FPaperZDAnimSequenceDetailCustomization::CustomizeDetails(const TSharedPtr<IDetailLayoutBuilder>& DetailBuilder)
{
	DetailBuilderPtr = DetailBuilder;
	CustomizeDetails(*DetailBuilder);
}

void FPaperZDAnimSequenceDetailCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	TArray<TWeakObjectPtr<UObject>> SelectedObjectsList;
	DetailBuilder.GetObjectsBeingCustomized(SelectedObjectsList);

	// Multi selection is disabled for this
	UPaperZDAnimSequence* AnimSequence = Cast<UPaperZDAnimSequence>(SelectedObjectsList[0].Get());
	AnimSequencePtr = AnimSequence;
	if (AnimSequence == nullptr || SelectedObjectsList.Num() > 1)
	{
		return;
	}

	//Obtain the data source array, if it exists
	DataSourceProperty = AnimSequence->GetAnimDataSourceProperty();
	TSharedRef<IPropertyHandle> DirectionalSequencePropertyHandle = DetailBuilder.GetProperty("bDirectionalSequence");
	TSharedRef<IPropertyHandle> AngleOffsetPropertyHandle = DetailBuilder.GetProperty("DirectionalAngleOffset");
	if (DataSourceProperty == nullptr)
	{
		//We have no access to the data source and hence, cannot make use of the "Directional Sequence" widgets nor properties.
		DirectionalSequencePropertyHandle->MarkHiddenByCustomization();
		AngleOffsetPropertyHandle->MarkHiddenByCustomization();
		return;
	}

	//Initial modifications to the array itself, marking as fixed order to remove each item's "reordering" widget outline
	static const FName Name_DisableReordering("EditFixedOrder");
	DataSourceProperty->SetMetaData(Name_DisableReordering, "true");
	TSharedRef<IPropertyHandle> DataSourcePropertyHandle = DetailBuilder.GetProperty(DataSourceProperty->GetFName(), AnimSequence->GetClass());
	DataSourcePropertyHandle->MarkHiddenByCustomization();
	
	//We store the array handle now that its metadata is setup.
	ArrayHandle = DataSourcePropertyHandle->AsArray();

	//Make sure the AnimSequence has a valid number of nodes
	if (!InitDataSourceHandleForAnimSequence(AnimSequence))
	{
		return;
	}
	
	//Setup the directional sequence property... we want to force this to go first in the UI, but for that we need to manually add the property to the detail builder
	DirectionalSequencePropertyHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &FPaperZDAnimSequenceDetailCustomization::OnDirectionalPropertyChange));
	DirectionalSequencePropertyHandle->MarkHiddenByCustomization();
	DetailBuilder.EditCategory("AnimSequence").AddProperty(DirectionalSequencePropertyHandle);
	
	//Angle should be hidden, as it will be shown in-lined alongside the number of nodes on top of the Grid widget
	AngleOffsetPropertyHandle->MarkHiddenByCustomization();

	//Create the directional widget
	TSharedPtr<SWidget> DirectionalGrid = SNew(SPaperZDAnimDataSourceGrid)
									.InitialSelection(0)
									.ShowTooltip(true)
									.NumNodes(this, &FPaperZDAnimSequenceDetailCustomization::GetNumNodes)
									.AngleOffset(this, &FPaperZDAnimSequenceDetailCustomization::GetAngleOffset)
									.OnNodeHasData(this, &FPaperZDAnimSequenceDetailCustomization::IsDataSourceEntrySet)
									.OnPreviewAreaChange(this, &FPaperZDAnimSequenceDetailCustomization::OnPreviewAreaChange)
									.OnNodeSelectionChange(this, &FPaperZDAnimSequenceDetailCustomization::SelectDataSourceEntry);

	//Create a row for the grid to live in
	FDetailWidgetRow& DirectionalGridRow = DetailBuilder.EditCategory("AnimSequence").AddCustomRow(FText::FromString("DataSource"));
	DirectionalGridRow
				.Visibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateSP(this, &FPaperZDAnimSequenceDetailCustomization::GetDirectionalGridVisibility)))
				.NameContent()
				[
					
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					[
						SNew(STextBlock)
						.Text(FText::FromString("Animation Data"))
						.Margin(FMargin(0.0f, 0.0f, 0.0f, 2.0f))
					]
					+ SVerticalBox::Slot()
					[
						SNew(STextBlock)
						.Text(this, &FPaperZDAnimSequenceDetailCustomization::GetAnimationDataDetailText)
						.Font(FCoreStyle::Get().GetFontStyle("SmallFont"))
						.Margin(FMargin(2.0f, 0.0f, 0.0f, 2.0f))
					]
				]
				.ValueContent()
				.MinDesiredWidth(250.0f)
				[
					
					SNew(SBorder)
					.BorderImage(FAppStyle::GetBrush("Graph.Panel.SolidBackground"))
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							//Angle and node control
							SNew(SVerticalBox)
							+ SVerticalBox::Slot()
							.AutoHeight()
							.Padding(FMargin(8.0f, 5.0f, 8.0f, 0.0f))
							[
								//Node number setup
								SNew(SHorizontalBox)
								+ SHorizontalBox::Slot()
								.VAlign(VAlign_Center)
								.AutoWidth()
								[
									SNew(STextBlock)
									.Text(FText::FromString("N:"))
									.ColorAndOpacity(FLinearColor::White)
									.Margin(FMargin(0.0f, 0.0f, 5.0f, 0.0f))
								]

								+ SHorizontalBox::Slot()
								.VAlign(VAlign_Center)
								.AutoWidth()
								[
									SNew(SNumericEntryBox<uint32>)
									.MinValue(3)
									.MinSliderValue(3)
									.MaxValue(16)
									.MaxSliderValue(16)
									.MinDesiredValueWidth(25.0f)
									.LabelVAlign(VAlign_Center)
									.OnValueCommitted(this, &FPaperZDAnimSequenceDetailCustomization::OnCommitedNumNodes)
									.Value(this, &FPaperZDAnimSequenceDetailCustomization::GetValueForNumNodesNumericEntry)
								]

								//Spacer
								+ SHorizontalBox::Slot()
								.VAlign(VAlign_Center)
								.FillWidth(1.0f)
								[
									SNew(SSpacer)
								]

								//Angle offset setup
								+ SHorizontalBox::Slot()
								.VAlign(VAlign_Center)
								.AutoWidth()
								[
									SNew(STextBlock)
									.Text(FText::FromString("Angle:"))
									.ColorAndOpacity(FLinearColor::White)
									.Margin(FMargin(0.0f, 0.0f, 5.0f, 0.0f))
								]

								+ SHorizontalBox::Slot()
								.VAlign(VAlign_Center)
								.AutoWidth()
								[
									AngleOffsetPropertyHandle->CreatePropertyValueWidget(true)
								]
							]

							//Grid
							+ SVerticalBox::Slot()
							.AutoHeight()
							[
								DirectionalGrid.ToSharedRef()
							]

						]
					]
				];

	//Create in-lined entries for the array data source, these will serve as a "detail view" for the grid.
	DataSourceEntryCount = GetNumNodes();
	for (int32 i = 0; i < DataSourceEntryCount; i++)
	{
		TSharedPtr<IPropertyHandle> Entry = ArrayHandle->GetElement(i);
		IDetailPropertyRow& Row = DetailBuilder.EditCategory("AnimSequence").AddProperty(Entry);
		Row.ShowPropertyButtons(false);
		Row.ShouldAutoExpand(true);
		Row.DisplayName(GetAnimationDataNodeText(i));
		Row.Visibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateSP(this, &FPaperZDAnimSequenceDetailCustomization::GetDetailVisibility, i)));
	}

	//Init the first node
	SelectDataSourceEntry(0);
}

bool FPaperZDAnimSequenceDetailCustomization::InitDataSourceHandleForAnimSequence(UPaperZDAnimSequence* AnimSequence)
{
	uint32 NumElements;
	if (ArrayHandle->GetNumElements(NumElements) == FPropertyAccess::Success)
	{
		if (AnimSequence->IsDirectionalSequence() && NumElements < MIN_NUM_NODES)
		{
			//Create at least 3 nodes
			static const uint32 DefaultNum = 4;
			SetDataSourceNum(DefaultNum);
		}
		else if (!AnimSequence->IsDirectionalSequence() && NumElements != 1)
		{
			//Make sure there's only 1 node here
			SetDataSourceNum(1);
		}

		return true;
	}

	return false;
}

void FPaperZDAnimSequenceDetailCustomization::SetDataSourceNum(uint32 Num)
{
	uint32 NumElements;
	if (ArrayHandle->GetNumElements(NumElements) == FPropertyAccess::Success)
	{
		if (Num > NumElements)
		{
			for (uint32 i = NumElements; i < Num; i++)
			{
				ArrayHandle->AddItem();
			}
		}
		else if (Num < NumElements)
		{
			while (Num < NumElements)
			{
				ArrayHandle->DeleteItem(--NumElements);
			}
		}
		
		//Should have the same amount of elements now
		ArrayHandle->GetNumElements(NumElements);
		check(NumElements == Num);
	}
}

bool FPaperZDAnimSequenceDetailCustomization::IsDataSourceEntrySet(int32 Index) const
{
	if (AnimSequencePtr.IsValid())
	{
		return AnimSequencePtr->IsDataSourceEntrySet(Index);
	}

	return false;
}

EVisibility FPaperZDAnimSequenceDetailCustomization::GetDirectionalGridVisibility() const
{
	return AnimSequencePtr.IsValid() && AnimSequencePtr->IsDirectionalSequence() ? EVisibility::Visible : EVisibility::Hidden;
}

EVisibility FPaperZDAnimSequenceDetailCustomization::GetDetailVisibility(int32 Index) const
{
	return Index == SelectedNodeIndex ? EVisibility::Visible : EVisibility::Hidden;
}

int32 FPaperZDAnimSequenceDetailCustomization::GetNumNodes() const
{
	uint32 NumNodes = 0;
	ArrayHandle->GetNumElements(NumNodes);
	return static_cast<int32>(NumNodes);
}

uint32 FPaperZDAnimSequenceDetailCustomization::GetNumNodesUnsigned() const
{
	uint32 NumNodes = 0;
	ArrayHandle->GetNumElements(NumNodes);
	return NumNodes;
}

TOptional<uint32> FPaperZDAnimSequenceDetailCustomization::GetValueForNumNodesNumericEntry() const
{
	return TOptional<uint32>(GetNumNodesUnsigned());
}

float FPaperZDAnimSequenceDetailCustomization::GetAngleOffset() const
{
	return AnimSequencePtr.IsValid() ? AnimSequencePtr->GetDirectionalAngleOffset() : 0.0f;
}

void FPaperZDAnimSequenceDetailCustomization::SelectDataSourceEntry(int32 Index)
{
	SelectedNodeIndex = Index;
}

void FPaperZDAnimSequenceDetailCustomization::OnPreviewAreaChange(int32 Index)
{
	if (AnimSequencePtr.IsValid())
	{
		AnimSequencePtr->DirectionalPreviewIndex = Index;
	}
}

void FPaperZDAnimSequenceDetailCustomization::OnDirectionalPropertyChange()
{
	ForceRefresh();
}

void FPaperZDAnimSequenceDetailCustomization::OnCommitedNumNodes(uint32 NewValue, ETextCommit::Type CommitType)
{
	if (NewValue != GetNumNodesUnsigned() && NewValue >= MIN_NUM_NODES && NewValue <= MAX_NUM_NODES)
	{
		FScopedTransaction Transaction(NSLOCTEXT("PaperZD", "AnimSequenceSetNumNodes", "Set Number of Nodes"));
		AnimSequencePtr->Modify();
		SetDataSourceNum(NewValue);
		ForceRefresh();
	}
}

void FPaperZDAnimSequenceDetailCustomization::ForceRefresh()
{
	//Need the detail layout builder to not hold a reference, otherwise the details view crashes due to the DetailLayout pointer not being unique
	if (DetailBuilderPtr.IsValid())
	{
		IDetailLayoutBuilder* LayoutBuilder = DetailBuilderPtr.Pin().Get();
		LayoutBuilder->ForceRefreshDetails();
	}
}

void FPaperZDAnimSequenceDetailCustomization::PostUndo(bool bSuccess)
{
	//Possibility that the post operation data source requires more nodes than we have available for use
	if (!CheckPostUndoRedoSupport())
	{
		ForceRefresh();
	}
}

void FPaperZDAnimSequenceDetailCustomization::PostRedo(bool bSuccess)
{
	//Possibility that the post operation data source requires more nodes than we have available for use
	if (!CheckPostUndoRedoSupport())
	{
		ForceRefresh();
	}
}

bool FPaperZDAnimSequenceDetailCustomization::CheckPostUndoRedoSupport() const
{
	//After an UNDO/REDO the property handle doesn't return a valid entry yet, hence we need to query the property directly
	bool bSupported = false;
	if (AnimSequencePtr.IsValid())
	{
		FScriptArrayHelper ArrayHelper(DataSourceProperty, DataSourceProperty->ContainerPtrToValuePtr<uint8>(AnimSequencePtr.Get()));
		bSupported = ArrayHelper.Num() <= DataSourceEntryCount;
	}

	return bSupported;
}

FText FPaperZDAnimSequenceDetailCustomization::GetAnimationDataDetailText() const
{
	FFormatNamedArguments Args;
	Args.Add(TEXT("N"), GetNumNodes());
	return FText::Format(LOCTEXT("AnimDataSourceDetailText", "- {N} Directions -"), Args);
}

FText FPaperZDAnimSequenceDetailCustomization::GetAnimationDataNodeText(int32 Index) const
{
	if (AnimSequencePtr.IsValid() && AnimSequencePtr->IsDirectionalSequence())
	{
		FFormatNamedArguments Args;
		Args.Add(TEXT("Node"), Index);
		return FText::Format(LOCTEXT("AnimDataSourceNodeText", "Animation Node {Node}"), Args);
	}
	else
	{
		return FText::FromString(TEXT("Animation Data"));
	}
}

#undef LOCTEXT_NAMESPACE
