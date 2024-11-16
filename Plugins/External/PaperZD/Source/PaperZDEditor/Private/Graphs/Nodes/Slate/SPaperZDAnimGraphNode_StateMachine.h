// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "KismetNodes/SGraphNodeK2Composite.h"

class SPaperZDAnimGraphNode_StateMachine : public SGraphNodeK2Composite
{
public:
	SLATE_BEGIN_ARGS(SPaperZDAnimGraphNode_StateMachine) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, class UPaperZDAnimGraphNode_StateMachine* InNode);
protected:
	// SGraphNodeK2Composite interface
	virtual UEdGraph* GetInnerGraph() const override;
	// End of SGraphNodeK2Composite interface
};
