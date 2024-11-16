// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "PaperZDStateGraphNode_TransBase.h"
#include "PaperZDStateGraphNode_Conduit.generated.h"

class UPaperZDStateGraphNode_Transition;

/**
 * A conduit is a many-to-many "transition" node that can be used to group multiple connections
 * with the same transition rule.
 * A conduit has an inner "transition graph" which must evaluate to true for it to be considered, even if the
 * input/output transitions are valid.
 */
UCLASS()
class UPaperZDStateGraphNode_Conduit : public UPaperZDStateGraphNode_TransBase
{
	GENERATED_UCLASS_BODY()

private:
#pragma region DEPRECATED
	/* Display name of the conduit. */
	UPROPERTY()
	FText Name_DEPRECATED;
#pragma endregion

public:
	//~ Begin UEdGraphNode Interface
	virtual void AllocateDefaultPins() override;
	virtual void AutowireNewNode(UEdGraphPin* FromPin) override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual TSharedPtr<SGraphNode> CreateVisualWidget() override;
	//~ End UEdGraphNode Interface

	//~ Begin UPaperZDStateGraphNode Interface
	virtual UEdGraphPin* GetInputPin() const override { return Pins[0]; }
	virtual UEdGraphPin* GetOutputPin() const override { return Pins[1]; }
	virtual void GetTransitions(TArray<UPaperZDStateGraphNode_Transition *>& OutTransitions) const override;
	virtual FString GetDesiredNewNodeName() const override;
	//~ End UPaperZDStateGraphNode Interface
};
