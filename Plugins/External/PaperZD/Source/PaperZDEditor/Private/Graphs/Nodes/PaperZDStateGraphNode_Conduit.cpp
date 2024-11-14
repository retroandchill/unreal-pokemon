// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Graphs/Nodes/PaperZDStateGraphNode_Conduit.h"
#include "Graphs/Nodes/PaperZDStateGraphNode_Transition.h"
#include "Slate/SPaperZDStateGraphNode_Conduit.h"
#include "EdGraph/EdGraph.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDStateGraphNode_Conduit)
#endif

UPaperZDStateGraphNode_Conduit::UPaperZDStateGraphNode_Conduit(const FObjectInitializer& ObjectInitializer)
	: Super()
{
	Name_DEPRECATED = FText::FromString(TEXT("Conduit"));
	bCanRenameNode = true;
}

FText UPaperZDStateGraphNode_Conduit::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText::FromString(GetNodeName());
}

void UPaperZDStateGraphNode_Conduit::AllocateDefaultPins()
{
	FCreatePinParams PinParams;
	UEdGraphPin* Inputs = CreatePin(EGPD_Input, TEXT("Transition"), TEXT(""), NULL, TEXT("In"), PinParams);
	UEdGraphPin* Outputs = CreatePin(EGPD_Output, TEXT("Transition"), TEXT(""), NULL, TEXT("Out"), PinParams);
}

void UPaperZDStateGraphNode_Conduit::AutowireNewNode(UEdGraphPin* FromPin)
{
	Super::AutowireNewNode(FromPin);

	if (FromPin != NULL)
	{
		if (GetSchema()->TryCreateConnection(FromPin, GetInputPin()))
		{
			FromPin->GetOwningNode()->NodeConnectionListChanged();
		}
	}
}

TSharedPtr<SGraphNode> UPaperZDStateGraphNode_Conduit::CreateVisualWidget()
{
	return SNew(SPaperZDStateGraphNode_Conduit, this);
}

void UPaperZDStateGraphNode_Conduit::GetTransitions(TArray<UPaperZDStateGraphNode_Transition *>& OutTransitions) const
{
	//Iterate over every connection and check if its a transition
	for (auto Pin : GetOutputPin()->LinkedTo)
	{
		UPaperZDStateGraphNode_Transition* TransitionNode = Cast<UPaperZDStateGraphNode_Transition>(Pin->GetOwningNode());
		if (TransitionNode)
		{
			OutTransitions.Add(TransitionNode);
		}
	}
}

FString UPaperZDStateGraphNode_Conduit::GetDesiredNewNodeName() const
{
	return TEXT("Conduit");
}
