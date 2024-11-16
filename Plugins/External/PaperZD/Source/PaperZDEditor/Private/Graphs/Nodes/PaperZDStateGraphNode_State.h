// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "Graphs/Nodes/PaperZDStateGraphNode.h"
#include "PaperZDStateGraphNode_State.generated.h"

class UPaperZDStateGraphNode_Transition;

/**
 * The state node is the main type of node for the state machine. In general, every other type of node is just transitory, while the StateNode is the only one
 * that can be active for more than one frame.
 */
UCLASS()
class UPaperZDStateGraphNode_State : public UPaperZDStateGraphNode
{
	GENERATED_UCLASS_BODY()

private:
	/* The internal animation graph for this state. */
	UPROPERTY()
	TObjectPtr<UEdGraph> BoundGraph;

public:

	/* Name of a custom event to call when the state machine enters this state. */
	UPROPERTY(EditAnywhere, Category = "Callbacks")
	FName OnStateEnterEventName;

	/* Name of a custom event to call when the state machine exits this state. */
	UPROPERTY(EditAnywhere, Category = "Callbacks")
	FName OnStateExitEventName;

#pragma region DEPRECATED
	/* Display name of the state. */
	UPROPERTY()
	FText Name_DEPRECATED;

	UPROPERTY()
	class UPaperFlipbook* Flipbook_DEPRECATED;

	UPROPERTY()
	FName SequenceIdentifier_DEPRECATED;

	UPROPERTY()
	class UPaperZDAnimSequence* AnimSequence_DEPRECATED;

	UPROPERTY()
	bool bShouldLoop_DEPRECATED;
#pragma endregion

public:

	//~ Begin UEdGraphNode Interface
	virtual void AllocateDefaultPins() override;
	virtual void PostPlacedNewNode() override;
	virtual void DestroyNode() override;
	virtual void PostPasteNode() override;
	virtual void AutowireNewNode(UEdGraphPin* FromPin) override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual TSharedPtr<SGraphNode> CreateVisualWidget() override;
	//~ End UEdGraphNode Interface

	//~ Begin UPaperZDStateGraphNode Interface
	virtual UEdGraphPin* GetInputPin() const override { return Pins[0]; }
	virtual UEdGraphPin* GetOutputPin() const override { return Pins[1]; }
	virtual UEdGraph* GetBoundGraph() const override { return BoundGraph; }
	virtual FString GetNodeName() const override;
	virtual FString GetDesiredNewNodeName() const override;
	virtual void GetTransitions(TArray<UPaperZDStateGraphNode_Transition*>& OutTransitions) const override;
	//~ End UPaperZDStateGraphNode Interface

private:
	/* Destroys the bound graphs. */
	void ClearBoundGraph();
};
