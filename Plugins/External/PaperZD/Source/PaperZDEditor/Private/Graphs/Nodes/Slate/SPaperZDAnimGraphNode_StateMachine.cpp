// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Graphs/Nodes/Slate/SPaperZDAnimGraphNode_StateMachine.h"
#include "Graphs/Nodes/PaperZDAnimGraphNode_StateMachine.h"
#include "Graphs/PaperZDStateMachineGraph.h"

void SPaperZDAnimGraphNode_StateMachine::Construct(const FArguments& InArgs, UPaperZDAnimGraphNode_StateMachine* InNode)
{
	GraphNode = InNode;
	SetCursor(EMouseCursor::CardinalCross);
	UpdateGraphNode();
}

UEdGraph* SPaperZDAnimGraphNode_StateMachine::GetInnerGraph() const
{
	UPaperZDAnimGraphNode_StateMachine* StateMachine= CastChecked<UPaperZDAnimGraphNode_StateMachine>(GraphNode);
	return StateMachine->GetStateMachineGraph();
}
