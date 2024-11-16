// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "EdGraph/EdGraphNode.h"
#include "PaperZDStateGraphNode.generated.h"

class UPaperZDStateGraphNode_Transition;

/**
 * Base class for all the StateMachine nodes for PaperZD.
 */
UCLASS()
class UPaperZDStateGraphNode : public UEdGraphNode
{
	GENERATED_UCLASS_BODY()

public:
	//~Begin UEdGraphNode Interface
	virtual void PostPasteNode() override;
	virtual UObject* GetJumpTargetForDoubleClick() const override;
	virtual bool CanJumpToDefinition() const override;
	virtual void JumpToDefinition() const override;
	virtual bool CanCreateUnderSpecifiedSchema(const UEdGraphSchema* Schema) const override;
	virtual void OnRenameNode(const FString& NewName) override;
	virtual TSharedPtr<class INameValidatorInterface> MakeNameValidator() const override;
	//~End UEdGraphNode Interface

	/* Obtain a list of all the transitions connected to the output of this node. */
	virtual void GetTransitions(TArray<UPaperZDStateGraphNode_Transition*>& OutTransitions) const {}

	/* Returns the name of this node. */
	virtual FString GetNodeName() const { return TEXT("BaseNode"); }

	/* Returns the name the nodes wishes to have when created. */
	virtual FString GetDesiredNewNodeName() const { return TEXT("BaseNode"); }

	/* Returns the input pin for this node. */
	virtual UEdGraphPin* GetInputPin() const { return nullptr; }

	/* Returns the output pin for this node. */
	virtual UEdGraphPin* GetOutputPin() const { return nullptr; }

	/* Returns the bound graph for this node, if one exists. */
	virtual UEdGraph* GetBoundGraph() const { return nullptr; }
};
