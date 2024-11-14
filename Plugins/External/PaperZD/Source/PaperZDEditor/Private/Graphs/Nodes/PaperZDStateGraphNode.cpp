// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Graphs/Nodes/PaperZDStateGraphNode.h"
#include "Graphs/PaperZDStateMachineSchema.h"
#include "Kismet2/Kismet2NameValidators.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Engine/Blueprint.h"

#if ZD_VERSION_INLINED_CPP_SUPPORT
#include UE_INLINE_GENERATED_CPP_BY_NAME(PaperZDStateGraphNode)
#endif

//////////////////////////////////////////////////////////////////////////
//// Name Validator
//////////////////////////////////////////////////////////////////////////
class FPaperZDStateNodeNameValidator : public FStringSetNameValidator
{
public:
	FPaperZDStateNodeNameValidator(const UPaperZDStateGraphNode* InNode)
		: FStringSetNameValidator(FString())
	{
		TArray<UPaperZDStateGraphNode*> Nodes;
		InNode->GetGraph()->GetNodesOfClass(Nodes);

		for (auto NodeIt = Nodes.CreateIterator(); NodeIt; ++NodeIt)
		{
			auto Node = *NodeIt;
			if (Node != InNode)
			{
				Names.Add(Node->GetNodeName());
			}
		}
	}
};

//////////////////////////////////////////////////////////////////////////
//// PaperZD State Graph Node
//////////////////////////////////////////////////////////////////////////

UPaperZDStateGraphNode::UPaperZDStateGraphNode(const FObjectInitializer& ObjectInitializer)
	: Super()
{
}

void UPaperZDStateGraphNode::PostPasteNode()
{
	Super::PostPasteNode();

	if (UEdGraph* BoundGraph = GetBoundGraph())
	{
		// Add the new graph as a child of our parent graph
		UEdGraph* ParentGraph = GetGraph();

		if (ParentGraph->SubGraphs.Find(BoundGraph) == INDEX_NONE)
		{
			ParentGraph->SubGraphs.Add(BoundGraph);
		}

		//Avoid any name clashing with an older node
 		TSharedPtr<INameValidatorInterface> NameValidator = FNameValidatorFactory::MakeValidator(this);
 		FBlueprintEditorUtils::RenameGraphWithSuggestion(BoundGraph, NameValidator, GetNodeName());

		// Restore transactional flag that is lost during copy/paste process
		BoundGraph->SetFlags(RF_Transactional);

		UBlueprint* Blueprint = FBlueprintEditorUtils::FindBlueprintForGraphChecked(ParentGraph);
		FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(Blueprint);
	}
}

UObject* UPaperZDStateGraphNode::GetJumpTargetForDoubleClick() const
{
	return GetBoundGraph();
}

bool UPaperZDStateGraphNode::CanJumpToDefinition() const
{
	return GetJumpTargetForDoubleClick() != nullptr;
}

void UPaperZDStateGraphNode::JumpToDefinition() const
{
	if (UObject* HyperlinkTarget = GetJumpTargetForDoubleClick())
	{
		FKismetEditorUtilities::BringKismetToFocusAttentionOnObject(HyperlinkTarget);
	}
}

bool UPaperZDStateGraphNode::CanCreateUnderSpecifiedSchema(const UEdGraphSchema* Schema) const
{
	return Schema->IsA(UPaperZDStateMachineSchema::StaticClass());
}

void UPaperZDStateGraphNode::OnRenameNode(const FString& NewName)
{
	FBlueprintEditorUtils::RenameGraph(GetBoundGraph(), NewName);
}

TSharedPtr<class INameValidatorInterface> UPaperZDStateGraphNode::MakeNameValidator() const
{
	return MakeShareable(new FPaperZDStateNodeNameValidator(this));
}
