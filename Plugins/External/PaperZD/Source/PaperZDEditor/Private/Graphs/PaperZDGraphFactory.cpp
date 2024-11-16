// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Graphs/PaperZDGraphFactory.h"
#include "Graphs/PaperZDAnimGraphSchema.h"
#include "Graphs/PaperZDAnimGraphConnectionDrawingPolicy.h"
#include "Graphs/Nodes/Slate/SPaperZDGraphPin_AnimDataLink.h"

#include "Graphs/Nodes/PaperZDAnimGraphNode_Sink.h"
#include "Graphs/Nodes/Slate/SPaperZDAnimGraphNode_Sink.h"
#include "Graphs/Nodes/PaperZDAnimGraphNode_StateMachine.h"
#include "Graphs/Nodes/Slate/SPaperZDAnimGraphNode_StateMachine.h"

//////////////////////////////////////////////////////////////////////////
//// Graph Factory
//////////////////////////////////////////////////////////////////////////
TSharedPtr<SGraphNode> FPaperZDGraphNodeFactory::CreateNode(UEdGraphNode* InNode) const
{
	if (UPaperZDAnimGraphNode_Sink* SinkNode = Cast<UPaperZDAnimGraphNode_Sink>(InNode))
	{
		return SNew(SPaperZDAnimGraphNode_Sink, SinkNode);
	}
	else if (UPaperZDAnimGraphNode_StateMachine* StateMachineNode = Cast<UPaperZDAnimGraphNode_StateMachine>(InNode))
	{
		return SNew(SPaperZDAnimGraphNode_StateMachine, StateMachineNode);
	}

	return nullptr;
}

//////////////////////////////////////////////////////////////////////////
//// Pin Factory
//////////////////////////////////////////////////////////////////////////
TSharedPtr<SGraphPin> FPaperZDGraphPinFactory::CreatePin(UEdGraphPin* Pin) const
{
	if (Pin->PinType.PinCategory == UEdGraphSchema_K2::PC_Struct)
	{
		if ((Pin->PinType.PinSubCategoryObject == FPaperZDAnimDataLink::StaticStruct()))
		{
			return SNew(SPaperZDGraphPin_AnimDataLink, Pin);
		}
	}

	return nullptr;
}

//////////////////////////////////////////////////////////////////////////
//// Pin Connection Factory
//////////////////////////////////////////////////////////////////////////
FConnectionDrawingPolicy* FPaperZDGraphPinConnectionFactory::CreateConnectionPolicy(const UEdGraphSchema* Schema, int32 InBackLayerID, int32 InFrontLayerID, float ZoomFactor, const class FSlateRect& InClippingRect, class FSlateWindowElementList& InDrawElements, class UEdGraph* InGraphObj) const
{
	if (Schema->IsA(UPaperZDAnimGraphSchema::StaticClass()))
	{
		return new FPaperZDAnimGraphConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, ZoomFactor, InClippingRect, InDrawElements, InGraphObj);
	}

	return nullptr;
}
