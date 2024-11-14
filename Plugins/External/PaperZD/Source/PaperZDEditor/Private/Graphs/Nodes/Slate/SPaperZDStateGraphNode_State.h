// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateColor.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "SNodePanel.h"
#include "SGraphNode.h"
#include "SGraphPin.h"

//Fwd 
class UPaperZDStateGraphNode_State;
class SToolTip;

class SPaperZDStateGraphNode_State : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SPaperZDStateGraphNode_State) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UPaperZDStateGraphNode_State* InNode);
	
	//~ SNodePanel::SNode interface
	virtual void GetNodeInfoPopups(FNodeInfoContext* Context, TArray<FGraphInformationPopupInfo>& Popups) const override;
	//~ End of SNodePanel::SNode interface

	//~ SGraphNode interface
	virtual void UpdateGraphNode() override;
	virtual void CreatePinWidgets() override;
	virtual void AddPin(const TSharedRef<SGraphPin>& PinToAdd) override;
	virtual TSharedPtr<SToolTip> GetComplexTooltip() override;
	//~ End of SGraphNode interface

	static void GetStateInfoPopup(UEdGraphNode* GraphNode, TArray<FGraphInformationPopupInfo>& Popups);

protected:
	FSlateColor GetBorderBackgroundColor() const;
	virtual const FSlateBrush* GetNameIcon() const;
	virtual FText GetPreviewCornerText() const;
};