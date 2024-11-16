// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "Graphs/Nodes/PaperZDStateGraphNode_TransBase.h"
#include "PaperZDStateGraphNode_Transition.generated.h"

/**
 * A normal transition node in the state machine.
 */
UCLASS()
class UPaperZDStateGraphNode_Transition : public UPaperZDStateGraphNode_TransBase
{
	GENERATED_UCLASS_BODY()

	/* Friendship for version fixup. */
	friend class FPaperZDRuntimeEditorProxy;

	/* When multiple transitions need to be evaluated, the ones with a higher priority will be checked first. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Transition")
	int32 Priority;

	/* Display color of the transition link. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Transition")
	FColor Color;

	/* If we should enable a transitional AnimGraph to play a temporal animation between the states. */
	UPROPERTY(EditAnywhere, Category = "Animation")
	bool bPlayTransitionalAnimations;

	/* Transitional AnimGraph for playing "in-between" state animations. */
	UPROPERTY()
	TObjectPtr<UEdGraph> AnimTransitionalGraph;

#pragma region DEPRECATED
	UPROPERTY()
	FName SequenceIdentifier_DEPRECATED;

	UPROPERTY(/*EditAnywhere, Category = _Animation*/)
	TObjectPtr<class UPaperZDAnimSequence> AnimSequence_DEPRECATED;
#pragma endregion

public:	

	//Getters
	FORCEINLINE bool HasTransitionalAnimations() const { return bPlayTransitionalAnimations; }
	FORCEINLINE UEdGraph* GetAnimTransitionalGraph() const { return AnimTransitionalGraph; }

	//~ Begin UObject Interface
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	//~ End UObject Interface

	//~ Begin UEdGraphNode Interface
	virtual void AllocateDefaultPins() override;
	virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;
	virtual TSharedPtr<SGraphNode> CreateVisualWidget() override;
	virtual void PostPasteNode() override;
	virtual void DestroyNode() override;
	//~ End UEdGraphNode Interface

	//~ Begin UPaperZDStateGraphNode Interface
	virtual FString GetDesiredNewNodeName() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	//~ End UPaperZDStateGraphNode Interface

	/* Connects two nodes using this transition as an intermediate node. */
	void CreateConnections(UPaperZDStateGraphNode* PreviousState, UPaperZDStateGraphNode* NextState);

	/* Obtain the node that this transition originates from. */
	UPaperZDStateGraphNode* GetFromNode() const;

	/* Obtain the node that this transition points to. */
	UPaperZDStateGraphNode* GetToNode() const;

	/* Brings Kismet attention to the AnimTransitional Graph. */
	void OpenAnimTransitionalGraph() const;

private:
	/* Creates the AnimTransitionalGraph. */
	void CreateAnimTransitionalGraph();

	/* Either adds or removes the AnimTransition graph from its parent graph, depending if its enabled*/
	void UpdateAnimTransitionalGraphState();
};
