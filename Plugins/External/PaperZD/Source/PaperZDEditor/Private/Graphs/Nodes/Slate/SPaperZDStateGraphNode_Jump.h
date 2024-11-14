// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateColor.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "SNodePanel.h"
#include "SGraphNode.h"

class SGraphPin;
class UPaperZDStateGraphNode_Jump;

class SPaperZDStateGraphNode_Jump : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SPaperZDStateGraphNode_Jump) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UPaperZDStateGraphNode_Jump* InNode);
	
	// SGraphNode interface
	virtual void UpdateGraphNode() override;
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;
	// End of SGraphNode interface

protected:
	FSlateColor GetBorderBackgroundColor() const;
	FText GetPreviewCornerText() const;
};
