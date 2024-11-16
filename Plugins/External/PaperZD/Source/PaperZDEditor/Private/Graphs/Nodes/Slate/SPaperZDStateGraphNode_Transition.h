// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateColor.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SWidget.h"
#include "SNodePanel.h"
#include "SGraphNode.h"

//Fwd
class SToolTip;
class UPaperZDStateGraphNode_Transition;

class SPaperZDStateGraphNode_Transition : public SGraphNode
{
public:
	SLATE_BEGIN_ARGS(SPaperZDStateGraphNode_Transition) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UPaperZDStateGraphNode_Transition* InNode);

	//~ SNodePanel::SNode interface
	virtual void GetNodeInfoPopups(FNodeInfoContext* Context, TArray<FGraphInformationPopupInfo>& Popups) const override;
	virtual void MoveTo(const FVector2D& NewPosition, FNodeSet& NodeFilter, bool bMarkDirty = true) override;
	virtual bool RequiresSecondPassLayout() const override;
	virtual void PerformSecondPassLayout(const TMap< UObject*, TSharedRef<SNode> >& NodeToWidgetLookup) const override;
	//~ End of SNodePanel::SNode interface

	const FSlateBrush* GetBodyBrush() const;

	//~ SGraphNode interface
	virtual void UpdateGraphNode() override;
	virtual TSharedPtr<SToolTip> GetComplexTooltip() override;
	//~ End of SGraphNode interface

	// Calculate position for multiple nodes to be placed between a start and end point, by providing this nodes index and max expected nodes 
	void PositionBetweenTwoNodesWithOffset(const FGeometry& StartGeom, const FGeometry& EndGeom, int32 NodeIndex, int32 MaxNodes) const;

	//Get the transition color for the given node
	static FLinearColor StaticGetTransitionColor(UPaperZDStateGraphNode_Transition* TransNode, bool bIsHovered);

private:
	TSharedPtr<STextEntryPopup> TextEntryWidget;
	UPaperZDStateGraphNode_Transition* TransitionNode;

private:
	/* Gets the title of the transition to show on the tooltip. */
	FText GetPreviewCornerText(bool reverse) const;

	/* Getter for the color of the transition. */
	FSlateColor GetTransitionColor() const;

	/* Create the tooltip that shows the inlinined linear expression for the transition. */
	TSharedRef<SWidget> GenerateRichTooltip();

	/* Initializes the node position when created, so the GraphPanel doesn't mistakenly assume its not visible. */
	void InitNodePosition();
};
