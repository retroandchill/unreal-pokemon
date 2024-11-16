// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SWidget.h"
#include "KismetNodes/SGraphNodeK2Base.h"

class UPaperZDAnimGraphNode_Base;

/**
 * Custom visual representation for the result node.
 */
class SPaperZDAnimGraphNode_Sink : public SGraphNodeK2Base
{
public:
	SLATE_BEGIN_ARGS(SPaperZDAnimGraphNode_Sink) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, class UPaperZDAnimGraphNode_Base* InNode);

protected:
	// SGraphNode interface
	virtual TSharedRef<SWidget> CreateNodeContentArea() override;
	// End of SGraphNode interface
};
