// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "Graphs/Nodes/PaperZDStateGraphNode.h"
#include "PaperZDStateGraphNode_TransBase.generated.h"

/**
 * Base class for all the State Machine nodes that work as a transition and require an internal bound graph
 * for transition rules evaluation.
 */
UCLASS()
class UPaperZDStateGraphNode_TransBase : public UPaperZDStateGraphNode
{
	GENERATED_UCLASS_BODY()
		
private:
	/* Friendship for version fixup. */
	friend class FPaperZDRuntimeEditorProxy;

	/* The graph that contains the transition rules to evaluate. */
	UPROPERTY()
	TObjectPtr<UEdGraph> BoundGraph;

public:
	//~ Begin UEdGraphNode Interface
	virtual void PostPlacedNewNode() override;
	virtual void DestroyNode() override;
	//~ End UEdGraphNode Interface

	//~ Begin UPaperZDStateGraphNode Interface
	virtual UEdGraphPin* GetInputPin() const override { return Pins[0]; }
	virtual UEdGraphPin* GetOutputPin() const override { return Pins[1]; }
	virtual UEdGraph* GetBoundGraph() const override { return BoundGraph; }
	virtual FString GetNodeName() const override;
	//~ End UPaperZDStateGraphNode Interface
	
private:
	/* Creates the transition evaluation graph. */
	void CreateBoundGraph();

	/* Removes the bound graph. */
	void ClearBoundGraph();
};
