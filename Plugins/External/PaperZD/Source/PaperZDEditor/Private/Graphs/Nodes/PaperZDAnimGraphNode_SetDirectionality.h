// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "Graphs/Nodes/PaperZDAnimGraphNode_Base.h"
#include "AnimNodes/PaperZDAnimNode_SetDirectionality.h"
#include "PaperZDAnimGraphNode_SetDirectionality.generated.h"

/**
 * Sets the directionality data for multi-directional sequence rendering.
 */
UCLASS()
class UPaperZDAnimGraphNode_SetDirectionality : public UPaperZDAnimGraphNode_Base
{
	GENERATED_BODY()

	/* Runtime node. */
	UPROPERTY(EditAnywhere, Category = "Settings")
	FPaperZDAnimNode_SetDirectionality AnimNode;

public:
	UPaperZDAnimGraphNode_SetDirectionality();

	//~Begin UPaperZDAnimGraphNode_Base Interface
	virtual FString GetNodeCategory() const override;
	virtual void ValidateAnimNodeDuringCompilation(const UPaperZDAnimBP* ForAnimBP, FCompilerResultsLog& MessageLog);
	//~End UPaperZDAnimGraphNode_Base Interface

	//~ Begin UEdGraphNode Interface.
 	virtual FLinearColor GetNodeTitleColor() const override;
 	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	//~ End UEdGraphNode Interface.
};
