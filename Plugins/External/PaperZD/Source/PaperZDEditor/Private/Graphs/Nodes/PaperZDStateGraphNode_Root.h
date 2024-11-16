// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "Graphs/Nodes/PaperZDStateGraphNode.h"
#include "PaperZDStateGraphNode_Root.generated.h"

/**
 * Entry point of the state machine.
 */
UCLASS()
class UPaperZDStateGraphNode_Root : public UPaperZDStateGraphNode
{
	GENERATED_UCLASS_BODY()

	//~ Begin UEdGraphNode Interface
	virtual void AllocateDefaultPins() override;
	virtual bool CanDuplicateNode() const override { return false; }
	virtual bool CanUserDeleteNode() const override { return false; }
	virtual TSharedPtr<SGraphNode> CreateVisualWidget() override;
	//~ End UEdGraphNode Interface

	//~ Begin UPaperZDStateGraphNode Interface
	virtual UEdGraphPin* GetInputPin() const override { return nullptr; }
	virtual UEdGraphPin* GetOutputPin() const override { return Pins[0]; }
	virtual FString GetNodeName() const override;
	//~ End UPaperZDStateGraphNode Interface
};
