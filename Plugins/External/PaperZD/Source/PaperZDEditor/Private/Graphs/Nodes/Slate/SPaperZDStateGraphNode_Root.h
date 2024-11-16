// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateColor.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "SNodePanel.h"
#include "SGraphNode.h"

class SGraphPin;
class UPaperZDStateGraphNode_Root;

class SPaperZDStateGraphNode_Root : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SPaperZDStateGraphNode_Root) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UPaperZDStateGraphNode_Root* InNode);

	//~ SNodePanel::SNode interface
	virtual void GetNodeInfoPopups(FNodeInfoContext* Context, TArray<FGraphInformationPopupInfo>& Popups) const override;
	//~ End of SNodePanel::SNode interface

	//~ SGraphNode interface
	virtual void UpdateGraphNode() override;
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;
	//~ End of SGraphNode interface

protected:
	FSlateColor GetBorderBackgroundColor() const;
	FText GetPreviewCornerText() const;
};
