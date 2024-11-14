// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Graphs/Nodes/Slate/SPaperZDStateGraphNode_State.h"
#include "Graphs/Nodes/PaperZDStateGraphNode_State.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/SToolTip.h"
#include "SGraphPreviewer.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "IDocumentation.h"
#include "Widgets/Text/SInlineEditableTextBlock.h"

//#include "SGraphPreviewer.h"
//#include "IDocumentation.h"

/////////////////////////////////////////////////////
// SPaperZDStateNodeOutputPin

class SPaperZDStateNodeOutputPin : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SPaperZDStateNodeOutputPin) {}
	SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, UEdGraphPin* InPin);
protected:
	// Begin SGraphPin interface
	virtual TSharedRef<SWidget>	GetDefaultValueWidget() override;
	// End SGraphPin interface

	const FSlateBrush* GetPinBorder() const;
};

void SPaperZDStateNodeOutputPin::Construct(const FArguments& InArgs, UEdGraphPin* InPin)
{
	this->SetCursor(EMouseCursor::Default);

	bShowLabel = true;

	GraphPinObj = InPin;
	check(GraphPinObj != NULL);

	const UEdGraphSchema* Schema = GraphPinObj->GetSchema();
	check(Schema);

	// Set up a hover for pins that is tinted the color of the pin.
	SBorder::Construct(SBorder::FArguments()
		.BorderImage(this, &SPaperZDStateNodeOutputPin::GetPinBorder)
		.BorderBackgroundColor(this, &SPaperZDStateNodeOutputPin::GetPinColor)
		.OnMouseButtonDown(this, &SPaperZDStateNodeOutputPin::OnPinMouseDown)
		.Cursor(this, &SPaperZDStateNodeOutputPin::GetPinCursor)
	);
}

TSharedRef<SWidget>	SPaperZDStateNodeOutputPin::GetDefaultValueWidget()
{
	return SNew(STextBlock);
}

const FSlateBrush* SPaperZDStateNodeOutputPin::GetPinBorder() const
{
	return (IsHovered())
		? FAppStyle::GetBrush(TEXT("Graph.StateNode.Pin.BackgroundHovered"))
		: FAppStyle::GetBrush(TEXT("Graph.StateNode.Pin.Background"));
}

/////////////////////////////////////////////////////
// SPaperZDStateGraphNode_State

void SPaperZDStateGraphNode_State::Construct(const FArguments& InArgs, UPaperZDStateGraphNode_State* InNode)
{
	GraphNode = InNode;
	SetCursor(EMouseCursor::CardinalCross);
	UpdateGraphNode();
}


void SPaperZDStateGraphNode_State::GetStateInfoPopup(UEdGraphNode* GraphNode, TArray<FGraphInformationPopupInfo>& Popups)
{
	//@TODO: Add state info when Debug data is added
}

void SPaperZDStateGraphNode_State::GetNodeInfoPopups(FNodeInfoContext* Context, TArray<FGraphInformationPopupInfo>& Popups) const
{
	GetStateInfoPopup(GraphNode, Popups);
}

FSlateColor SPaperZDStateGraphNode_State::GetBorderBackgroundColor() const
{
	FLinearColor InactiveStateColor(0.08f, 0.08f, 0.08f);
	FLinearColor ActiveStateColorDim(0.4f, 0.3f, 0.15f);
	FLinearColor ActiveStateColorBright(1.f, 0.6f, 0.35f);

	//@TODO: Use the other colors when Debug Data is available

	return InactiveStateColor;
}

void SPaperZDStateGraphNode_State::UpdateGraphNode()
{
	InputPins.Empty();
	OutputPins.Empty();

	// Reset variables that are going to be exposed, in case we are refreshing an already setup node.
	RightNodeBox.Reset();
	LeftNodeBox.Reset();

	const FSlateBrush* NodeTypeIcon = GetNameIcon();

	FLinearColor TitleShadowColor(0.6f, 0.6f, 0.6f);
	TSharedPtr<SErrorText> ErrorText;
	TSharedPtr<SNodeTitle> NodeTitle = SNew(SNodeTitle, GraphNode);

	this->ContentScale.Bind(this, &SGraphNode::GetContentScale);
	this->GetOrAddSlot(ENodeZone::Center)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SBorder)
			.BorderImage(FAppStyle::GetBrush("Graph.StateNode.Body"))
			.Padding(0)
			.BorderBackgroundColor(this, &SPaperZDStateGraphNode_State::GetBorderBackgroundColor)
			[
				SNew(SOverlay)

				// PIN AREA
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SAssignNew(RightNodeBox, SVerticalBox)
				]

				// STATE NAME AREA
				+ SOverlay::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.Padding(10.0f)
				[
					SNew(SBorder)
					.BorderImage(FAppStyle::GetBrush("Graph.StateNode.ColorSpill"))
					.BorderBackgroundColor(TitleShadowColor)
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					.Visibility(EVisibility::SelfHitTestInvisible)
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							// POPUP ERROR MESSAGE
							SAssignNew(ErrorText, SErrorText)
							.BackgroundColor(this, &SPaperZDStateGraphNode_State::GetErrorColor)
							.ToolTipText(this, &SPaperZDStateGraphNode_State::GetErrorMsgToolTip)
						]

						+ SHorizontalBox::Slot()
						.AutoWidth()
						.VAlign(VAlign_Center)
						[
							SNew(SImage)
							.Image(NodeTypeIcon)
						]
					
						+ SHorizontalBox::Slot()
						.Padding(FMargin(4.0f, 0.0f, 4.0f, 0.0f))
						[
							SNew(SVerticalBox)
							+ SVerticalBox::Slot()
							.AutoHeight()
							[
								SAssignNew(InlineEditableText, SInlineEditableTextBlock)
								.Style(FAppStyle::Get(), "Graph.StateNode.NodeTitleInlineEditableText")
								.Text(NodeTitle.Get(), &SNodeTitle::GetHeadTitle)
								.OnVerifyTextChanged(this, &SPaperZDStateGraphNode_State::OnVerifyNameTextChanged)
								.OnTextCommitted(this, &SPaperZDStateGraphNode_State::OnNameTextCommited)
								.IsReadOnly(this, &SPaperZDStateGraphNode_State::IsNameReadOnly)
								.IsSelected(this, &SPaperZDStateGraphNode_State::IsSelectedExclusively)
							]
							
							+ SVerticalBox::Slot()
							.AutoHeight()
							[
								NodeTitle.ToSharedRef()
							]
						]
					]
				]
			]
		];

	ErrorReporting = ErrorText;
	ErrorReporting->SetError(ErrorMsg);
	CreatePinWidgets();
}

void SPaperZDStateGraphNode_State::CreatePinWidgets()
{
	UPaperZDStateGraphNode_State* StateNode = CastChecked<UPaperZDStateGraphNode_State>(GraphNode);

	UEdGraphPin* CurPin = StateNode->GetOutputPin();
	if (!CurPin->bHidden)
	{
		TSharedPtr<SGraphPin> NewPin = SNew(SPaperZDStateNodeOutputPin, CurPin);
		AddPin(NewPin.ToSharedRef());
	}
}

void SPaperZDStateGraphNode_State::AddPin(const TSharedRef<SGraphPin>& PinToAdd)
{
	PinToAdd->SetOwner(SharedThis(this));
	RightNodeBox->AddSlot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.FillHeight(1.0f)
		[
			PinToAdd
		];
	OutputPins.Add(PinToAdd);
}

TSharedPtr<SToolTip> SPaperZDStateGraphNode_State::GetComplexTooltip()
{
	UPaperZDStateGraphNode_State* StateNode = CastChecked<UPaperZDStateGraphNode_State>(GraphNode);

	return SNew(SToolTip)
		[
			SNew(SVerticalBox)
	
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				// Create the tooltip preview, ensure to disable state overlays to stop
				// PIE and read-only borders obscuring the graph
				SNew(SGraphPreviewer, StateNode->GetBoundGraph())
				.CornerOverlayText(this, &SPaperZDStateGraphNode_State::GetPreviewCornerText)
				.ShowGraphStateOverlay(false)
			]
			
			//@TODO: Documentation links
// 			+SVerticalBox::Slot()
// 			.AutoHeight()
// 			.Padding(FMargin(0.0f, 5.0f, 0.0f, 0.0f))
// 			[
// 				IDocumentation::Get()->CreateToolTip(FText::FromString("Documentation"), NULL, StateNode->GetDocumentationLink(), StateNode->GetDocumentationExcerptName())
// 			]

		];
}

FText SPaperZDStateGraphNode_State::GetPreviewCornerText() const
{
	UPaperZDStateGraphNode_State* StateNode = CastChecked<UPaperZDStateGraphNode_State>(GraphNode);
	return FText::Format(NSLOCTEXT("SGraphNodeAnimState", "PreviewCornerStateText", "{0} state"), FText::FromString(StateNode->GetNodeName()));
}

const FSlateBrush* SPaperZDStateGraphNode_State::GetNameIcon() const
{
	return FAppStyle::GetBrush(TEXT("Graph.StateNode.Icon"));
}