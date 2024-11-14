// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "Graphs/Nodes/PaperZDStateGraphNode.h"
#include "PaperZDStateGraphNode_Jump.generated.h"

/**
 * Specialized node that works as a secondary root and can be called from the AnimInstance to force change the flow of the state machine.
 * Will keep connected nodes to it for being cleaned up on the compile process, like the root node.
 */
UCLASS()
class UPaperZDStateGraphNode_Jump : public UPaperZDStateGraphNode
{
	GENERATED_BODY()

	/* Name for this redirector, will be indexed on the StateMachine. */
	UPROPERTY()
	FName Name;

public:
	UPaperZDStateGraphNode_Jump(const FObjectInitializer& ObjectInitializer);

	//~ Begin UEdGraphNode Interface
	virtual void AllocateDefaultPins() override;
	virtual TSharedPtr<SGraphNode> CreateVisualWidget() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void OnRenameNode(const FString & NewName) override;
	virtual void PostPlacedNewNode() override;
	virtual bool CanDuplicateNode() const override { return false; }
	//~ End UEdGraphNode Interface
	
	//~ Begin UPaperZDStateGraphNode Interface
	virtual UEdGraphPin* GetInputPin() const override { return nullptr; }
	virtual UEdGraphPin* GetOutputPin() const override { return Pins[0]; }
	virtual FString GetNodeName() const override;
	//~ End UPaperZDStateGraphNode Interface
};
