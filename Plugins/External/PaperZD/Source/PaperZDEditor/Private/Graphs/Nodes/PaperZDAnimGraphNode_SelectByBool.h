// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "Graphs/Nodes/PaperZDAnimGraphNode_Base.h"
#include "AnimNodes/PaperZDAnimNode_SelectByBool.h"
#include "PaperZDAnimGraphNode_SelectByBool.generated.h"

/**
 * Selects between two animations depending on a given boolean value.
 */
UCLASS()
class UPaperZDAnimGraphNode_SelectByBool : public UPaperZDAnimGraphNode_Base
{
	GENERATED_BODY()

	/* Runtime node. */
	UPROPERTY(EditAnywhere, Category = "Settings")
	FPaperZDAnimNode_SelectByBool AnimNode;

public:
	UPaperZDAnimGraphNode_SelectByBool();

	//~ Begin UEdGraphNode Interface.
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	//~ End UEdGraphNode Interface.		

	//~Begin UPaperZDAnimGraphNode_Base Interface
	virtual FString GetNodeCategory() const;
	virtual void ValidateAnimNodeDuringCompilation(const UPaperZDAnimBP* ForAnimBP, FCompilerResultsLog& MessageLog) override;
	//~End UPaperZDAnimGraphNode_Base Interface
};
