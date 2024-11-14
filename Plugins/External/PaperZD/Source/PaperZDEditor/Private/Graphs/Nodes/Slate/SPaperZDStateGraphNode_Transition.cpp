// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Graphs/Nodes/Slate/SPaperZDStateGraphNode_Transition.h"
#include "Graphs/Nodes/PaperZDStateGraphNode_Transition.h"
#include "Graphs/Nodes/PaperZDTransitionGraphNode_Result.h"
#include "Graphs/PaperZDAnimTransitionGraph.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/SToolTip.h"
#include "EdGraphSchema_K2.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "SKismetLinearExpression.h"
#include "ConnectionDrawingPolicy.h"
#include "IDocumentation.h"
#include "Editors/Util/PaperZDEditorStyle.h"

#define LOCTEXT_NAMESPACE "ZD_TransitionNodes"

void SPaperZDStateGraphNode_Transition::Construct(const FArguments& InArgs, UPaperZDStateGraphNode_Transition* InNode)
{
	GraphNode = InNode;
	TransitionNode = CastChecked<UPaperZDStateGraphNode_Transition>(InNode);
	UpdateGraphNode();
	InitNodePosition();
}

void SPaperZDStateGraphNode_Transition::GetNodeInfoPopups(FNodeInfoContext* Context, TArray<FGraphInformationPopupInfo>& Popups) const
{
}

void SPaperZDStateGraphNode_Transition::MoveTo(const FVector2D& NewPosition, FNodeSet& NodeFilter, bool bMarkDirty /* = true */)
{
	// Ignored; position is set by the location of the attached state nodes
}

bool SPaperZDStateGraphNode_Transition::RequiresSecondPassLayout() const
{
	return true;
}

void SPaperZDStateGraphNode_Transition::PerformSecondPassLayout(const TMap<UObject*, TSharedRef<SNode>>& NodeToWidgetLookup) const
{
	UPaperZDStateGraphNode_Transition* TransNode = CastChecked<UPaperZDStateGraphNode_Transition>(GraphNode);

	// Find the geometry of the state nodes we're connecting
	FGeometry StartGeom;
	FGeometry EndGeom;

	int32 TransIndex = 0;
	int32 NumOfTrans = 1;

	const UPaperZDStateGraphNode* PrevState = TransNode->GetFromNode();
	const UPaperZDStateGraphNode* NextState = TransNode->GetToNode();
	if ((PrevState != NULL) && (NextState != NULL))
	{
		const TSharedRef<SNode>* pPrevNodeWidget = NodeToWidgetLookup.Find(PrevState);
		const TSharedRef<SNode>* pNextNodeWidget = NodeToWidgetLookup.Find(NextState);
		if ((pPrevNodeWidget != NULL) && (pNextNodeWidget != NULL))
		{
			const TSharedRef<SNode>& PrevNodeWidget = *pPrevNodeWidget;
			const TSharedRef<SNode>& NextNodeWidget = *pNextNodeWidget;

			StartGeom = FGeometry(FVector2D(PrevState->NodePosX, PrevState->NodePosY), FVector2D::ZeroVector, PrevNodeWidget->GetDesiredSize(), 1.0f);
			EndGeom = FGeometry(FVector2D(NextState->NodePosX, NextState->NodePosY), FVector2D::ZeroVector, NextNodeWidget->GetDesiredSize(), 1.0f);

			//TArray<UPaperZDAnimGraphNode_Transition*> Transitions = PrevState->GetTransitions();
			TArray<UEdGraphPin *> LinkedPins = PrevState->GetOutputPin()->LinkedTo;

			/*Transitions = Transitions.FilterByPredicate([NextState](UPaperZDAnimGraphNode_Transition* InTransition) -> bool
			{
				return InTransition->GetNextState() == NextState;
			});*/

			//Map this to a transition array
			TArray<UPaperZDStateGraphNode_Transition*> Transitions;
			for (UEdGraphPin *Pin : LinkedPins)
			{
				UPaperZDStateGraphNode_Transition* Trans = Cast<UPaperZDStateGraphNode_Transition>(Pin->GetOwningNode());
				if (Trans && Trans->GetToNode() == NextState)
				{
					Transitions.Add(Trans);
				}
			}

			TransIndex = Transitions.IndexOfByKey(TransNode);
			NumOfTrans = Transitions.Num();
		}
	}

	//Position Node
	PositionBetweenTwoNodesWithOffset(StartGeom, EndGeom, TransIndex, NumOfTrans);
}

TSharedRef<SWidget> SPaperZDStateGraphNode_Transition::GenerateRichTooltip()
{
	UPaperZDStateGraphNode_Transition* TransNode = CastChecked<UPaperZDStateGraphNode_Transition>(GraphNode);
	if (TransNode->GetBoundGraph() == NULL)
	{
		return SNew(STextBlock).Text(LOCTEXT("NoAnimGraphBoundToNodeMessage", "Error: No graph"));
	}

	// Find the expression hooked up to the can execute pin of the transition node
	const UEdGraphSchema_K2* Schema = GetDefault<UEdGraphSchema_K2>();

	UEdGraphPin* CanExecPin = NULL;
	if (UPaperZDAnimTransitionGraph* TransGraph = Cast<UPaperZDAnimTransitionGraph>(TransNode->GetBoundGraph()))
	{
		if (UPaperZDTransitionGraphNode_Result* ResultNode = TransGraph->GetResultNode())
		{
			CanExecPin = ResultNode->FindPin(TEXT("bCanEnterTransition"));
		}
	}

	TSharedRef<SVerticalBox> Widget = SNew(SVerticalBox);
	const FText TooltipDesc = GetPreviewCornerText(false);

	// Transition rule linearized
	Widget->AddSlot()
		.AutoHeight()
		.Padding(2.0f)
		[
			SNew(STextBlock)
			.TextStyle(FAppStyle::Get(), TEXT("Graph.TransitionNode.TooltipName"))
			.Text(TooltipDesc)
		];

	Widget->AddSlot()
		.AutoHeight()
		.Padding(2.0f)
		[
			SNew(STextBlock)
			.TextStyle(FAppStyle::Get(), TEXT("Graph.TransitionNode.TooltipRule"))
			.Text(LOCTEXT("AnimGraphNodeTransitionRule_ToolTip", "Transition Rule (in words)"))
		];

	Widget->AddSlot()
		.AutoHeight()
		.Padding(2.0f)
		[
			SNew(SKismetLinearExpression, CanExecPin)
		];

	//@TODO: add documentation link when developed
	/*Widget->AddSlot()
		.AutoHeight()
		.Padding(2.0f)
		[
			IDocumentation::Get()->CreateToolTip(FText::FromString("Documentation"), NULL, TransNode->GetDocumentationLink(), TransNode->GetDocumentationExcerptName())
		];*/

	return Widget;
}

TSharedPtr<SToolTip> SPaperZDStateGraphNode_Transition::GetComplexTooltip()
{
	return SNew(SToolTip)
		[
			GenerateRichTooltip()
		];
}

void SPaperZDStateGraphNode_Transition::UpdateGraphNode()
{
	InputPins.Empty();
	OutputPins.Empty();

	// Reset variables that are going to be exposed, in case we are refreshing an already setup node.
	RightNodeBox.Reset();
	LeftNodeBox.Reset();

	this->ContentScale.Bind(this, &SGraphNode::GetContentScale);
	this->GetOrAddSlot(ENodeZone::Center)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			[
				SNew(SImage)
				.Image(this, &SPaperZDStateGraphNode_Transition::GetBodyBrush)
				.ColorAndOpacity(this, &SPaperZDStateGraphNode_Transition::GetTransitionColor)
			]
			+ SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(SImage)
				.Image(FPaperZDEditorStyle::Get().GetBrush("PaperZDEditor.Graph.TransitionNode.Icon"))
			]
		];
}

const FSlateBrush* SPaperZDStateGraphNode_Transition::GetBodyBrush() const
{
	return TransitionNode->HasTransitionalAnimations() ? FPaperZDEditorStyle::Get().GetBrush("PaperZDEditor.Graph.TransitionNode.Body_Seq") 
	: FPaperZDEditorStyle::Get().GetBrush("PaperZDEditor.Graph.TransitionNode.Body");
}

FText SPaperZDStateGraphNode_Transition::GetPreviewCornerText(bool bReverse) const
{
	UPaperZDStateGraphNode_Transition* TransNode = CastChecked<UPaperZDStateGraphNode_Transition>(GraphNode);
	UPaperZDStateGraphNode* PrevState = (bReverse ? TransNode->GetToNode() : TransNode->GetFromNode());
	UPaperZDStateGraphNode* NextState = (bReverse ? TransNode->GetFromNode() : TransNode->GetToNode());

	return FText::Format(LOCTEXT("TransitionXToY", "{0} to {1}"), PrevState->GetNodeTitle(ENodeTitleType::FullTitle), NextState->GetNodeTitle(ENodeTitleType::FullTitle));
}

FLinearColor SPaperZDStateGraphNode_Transition::StaticGetTransitionColor(UPaperZDStateGraphNode_Transition* TransNode, bool bIsHovered)
{
	const FLinearColor ActiveColor(1.0f, 0.4f, 0.3f, 1.0f);
	const FLinearColor HoverColor(0.724f, 0.256f, 0.0f, 1.0f);
	//FLinearColor BaseColor(0.9f, 0.9f, 0.9f, 1.0f);

	//@TODO: Add debug data colors and active / inactive status
	return bIsHovered ? HoverColor : TransNode->Color;
}

FSlateColor SPaperZDStateGraphNode_Transition::GetTransitionColor() const
{
	UPaperZDStateGraphNode_Transition* TransNode = CastChecked<UPaperZDStateGraphNode_Transition>(GraphNode);
	return StaticGetTransitionColor(TransNode, IsHovered());
}

void SPaperZDStateGraphNode_Transition::PositionBetweenTwoNodesWithOffset(const FGeometry& StartGeom, const FGeometry& EndGeom, int32 NodeIndex, int32 MaxNodes) const
{
	// Get a reasonable seed point (halfway between the boxes)
	const FVector2D StartCenter = FGeometryHelper::CenterOf(StartGeom);
	const FVector2D EndCenter = FGeometryHelper::CenterOf(EndGeom);
	const FVector2D SeedPoint = (StartCenter + EndCenter) * 0.5f;

	// Find the (approximate) closest points between the two boxes
	const FVector2D StartAnchorPoint = FGeometryHelper::FindClosestPointOnGeom(StartGeom, SeedPoint);
	const FVector2D EndAnchorPoint = FGeometryHelper::FindClosestPointOnGeom(EndGeom, SeedPoint);

	// Position ourselves halfway along the connecting line between the nodes, elevated away perpendicular to the direction of the line
	const float Height = 30.0f;

	const FVector2D DesiredNodeSize = GetDesiredSize();

	FVector2D DeltaPos(EndAnchorPoint - StartAnchorPoint);

	if (DeltaPos.IsNearlyZero())
	{
		DeltaPos = FVector2D(10.0f, 0.0f);
	}

	const FVector2D Normal = FVector2D(DeltaPos.Y, -DeltaPos.X).GetSafeNormal();

	const FVector2D NewCenter = StartAnchorPoint + (0.5f * DeltaPos) + (Height * Normal);

	FVector2D DeltaNormal = DeltaPos.GetSafeNormal();

	// Calculate node offset in the case of multiple transitions between the same two nodes
	// MultiNodeOffset: the offset where 0 is the centre of the transition, -1 is 1 <size of node>
	// towards the PrevStateNode and +1 is 1 <size of node> towards the NextStateNode.

	const float MutliNodeSpace = 0.2f; // Space between multiple transition nodes (in units of <size of node> )
	const float MultiNodeStep = (1.f + MutliNodeSpace); //Step between node centers (Size of node + size of node spacer)

	const float MultiNodeStart = -((MaxNodes - 1) * MultiNodeStep) / 2.f;
	const float MultiNodeOffset = MultiNodeStart + (NodeIndex * MultiNodeStep);

	// Now we need to adjust the new center by the node size, zoom factor and multi node offset
	const FVector2D NewCorner = NewCenter - (0.5f * DesiredNodeSize) + (DeltaNormal * MultiNodeOffset * DesiredNodeSize.Size());

	GraphNode->NodePosX = NewCorner.X;
	GraphNode->NodePosY = NewCorner.Y;
}

void SPaperZDStateGraphNode_Transition::InitNodePosition()
{
	UPaperZDStateGraphNode_Transition* TransNode = Cast<UPaperZDStateGraphNode_Transition>(GraphNode);
	const UPaperZDStateGraphNode* PrevState = TransNode->GetFromNode();
	const UPaperZDStateGraphNode* NextState = TransNode->GetToNode();

	if (PrevState && NextState)
	{
		//We just need to approximate the node position so it triggers a render on the graph panel, the second pass will accommodate this
		GraphNode->NodePosX = (PrevState->NodePosX + NextState->NodePosX) / 2;
		GraphNode->NodePosY = (PrevState->NodePosY + NextState->NodePosY) / 2;
	}
}

#undef LOCTEXT_NAMESPACE
