// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "Graphs/Nodes/PaperZDAnimGraphNode_Base.h"
#include "AnimNodes/PaperZDAnimNode_RandomPlayer.h"
#include "PaperZDAnimGraphNode_RandomPlayer.generated.h"

class UPaperZDAnimSequence;

/**
 * Plays sequences randomly by choosing from a provided list.
 */
UCLASS()
class UPaperZDAnimGraphNode_RandomPlayer : public UPaperZDAnimGraphNode_Base
{
	GENERATED_BODY()

	/* Runtime node. */
	UPROPERTY(EditAnywhere, Category = "Settings")
	FPaperZDAnimNode_RandomPlayer AnimNode;

public:
	UPaperZDAnimGraphNode_RandomPlayer();

	//~ Begin UEdGraphNode interface
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	//~ End UEdGraphNode interface

	//~ Begin UPaperZDAnimGraphNode_Base Interface
	virtual FString GetNodeCategory() const override;
	//~ End UPaperZDAnimGraphNode_Base Interface
};
