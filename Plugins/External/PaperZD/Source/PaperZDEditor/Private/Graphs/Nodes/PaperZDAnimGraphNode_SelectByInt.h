// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "Graphs/Nodes/PaperZDAnimGraphNode_Base.h"
#include "AnimNodes/PaperZDAnimNode_SelectByInt.h"
#include "PaperZDAnimGraphNode_SelectByInt.generated.h"

/**
 * Selects between two animations depending on a given boolean value.
 */
UCLASS()
class UPaperZDAnimGraphNode_SelectByInt : public UPaperZDAnimGraphNode_Base
{
	GENERATED_BODY()

	/* Runtime node. */
	UPROPERTY(EditAnywhere, Category = "Settings")
	FPaperZDAnimNode_SelectByInt AnimNode;

	/* When a pin has been removed, it holds the index that points to it, so that node reconstruction can correctly re-bind the links. */
	int32 RemovedPinIndex;

public:
	UPaperZDAnimGraphNode_SelectByInt();

	//~ Begin UEdGraphNode Interface.
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual void ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>& OldPins) override;
	//~ End UEdGraphNode Interface.	

	// UK2Node interface
	virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;
	// End of UK2Node interface

	//~Begin UPaperZDAnimGraphNode_Base Interface
	virtual FString GetNodeCategory() const;
	//~End UPaperZDAnimGraphNode_Base Interface

	/* Adds an animation pin to the entry list. */
	void AddAnimationPin();

	/* Removes the given animation pin. */
	void RemoveAnimationPin(UEdGraphPin* Pin);
};
