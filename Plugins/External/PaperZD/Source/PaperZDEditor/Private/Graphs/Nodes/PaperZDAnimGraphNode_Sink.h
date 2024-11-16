// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "Graphs/Nodes/PaperZDAnimGraphNode_Base.h"
#include "AnimNodes/PaperZDAnimNode_Sink.h"
#include "PaperZDAnimGraphNode_Sink.generated.h"

/**
 * A Sink node that "collects" the animation data as the output of the AnimationGraph.
 */
UCLASS()
class UPaperZDAnimGraphNode_Sink : public UPaperZDAnimGraphNode_Base
{
	GENERATED_BODY()

	/* Runtime node. */
	UPROPERTY(EditAnywhere, Category = "Settings")
	FPaperZDAnimNode_Sink AnimNode;

public:
	UPaperZDAnimGraphNode_Sink();

	//~Begin UPaperZDAnimGraphNode_Base Interface
	virtual bool RequiresOutputPin() const override { return false; }
	//~End UPaperZDAnimGraphNode_Base Interface

	//~ Begin UEdGraphNode Interface.
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual bool CanUserDeleteNode() const override { return false; }
	virtual bool CanDuplicateNode() const override { return false; }
	//virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual bool IsNodeRootSet() const override { return true; }
	//~ End UEdGraphNode Interface.
};
