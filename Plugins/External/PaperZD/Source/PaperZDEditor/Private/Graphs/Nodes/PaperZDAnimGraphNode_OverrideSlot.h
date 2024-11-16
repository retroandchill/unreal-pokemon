// Copyright 2017 ~ 2023 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "Graphs/Nodes/PaperZDAnimGraphNode_Base.h"
#include "AnimNodes/PaperZDAnimNode_OverrideSlot.h"
#include "EdGraph/EdGraphNodeUtils.h"
#include "PaperZDAnimGraphNode_OverrideSlot.generated.h"

/**
 * Creates an entry point for animations to be played through code that will override the AnimationGraph.
 */
UCLASS()
class UPaperZDAnimGraphNode_OverrideSlot : public UPaperZDAnimGraphNode_Base
{
	GENERATED_BODY()

	/* Runtime node. */
	UPROPERTY(EditAnywhere, Category = "Settings")
	FPaperZDAnimNode_OverrideSlot AnimNode;

	/* Constructing FText strings can be costly, so we cache the node's title */
	FNodeTextCache CachedNodeTitle;

public:
	UPaperZDAnimGraphNode_OverrideSlot();

	//~Begin UPaperZDAnimGraphNode_Base Interface
	virtual FString GetNodeCategory() const override;
	virtual void ValidateAnimNodeDuringCompilation(const UPaperZDAnimBP* ForAnimBP, FCompilerResultsLog& MessageLog) override;
	virtual void OnProcessDuringCompilation(FPaperZDAnimBPCompilerAccess& InCompilationContext, FPaperZDAnimBPGeneratedClassAccess& OutCompiledData);
	//~End UPaperZDAnimGraphNode_Base Interface

	// UEdGraphNode interface
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	// End of UEdGraphNode interface
};
