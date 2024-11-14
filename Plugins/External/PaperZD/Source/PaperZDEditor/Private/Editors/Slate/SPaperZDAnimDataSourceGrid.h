// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/SToolTip.h"
#include "Input/CursorReply.h"
#include "Input/Reply.h"

class STooltip;

DECLARE_DELEGATE_RetVal_OneParam(bool, FOnNodeHasDataSignature, int32);
DECLARE_DELEGATE_OneParam(FOnNodeSelectionChangeSignature, int32);

/**
 * Renders a directional grid that can be used to place Animation data on it.
 */
class SPaperZDAnimDataSourceGrid : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SPaperZDAnimDataSourceGrid)
		: _DrawRadius(0.85f)
		, _ShowTooltip(false)
		, _InitialSelection(INDEX_NONE)
	{}
	SLATE_ATTRIBUTE(int32, NumNodes)
	SLATE_ATTRIBUTE(float, AngleOffset)
	SLATE_ARGUMENT(float, DrawRadius)
	SLATE_ARGUMENT(bool, ShowTooltip)
	SLATE_ARGUMENT(float, InitialSelection)
	SLATE_EVENT(FOnNodeHasDataSignature, OnNodeHasData)
	SLATE_EVENT(FOnNodeSelectionChangeSignature, OnPreviewAreaChange)
	SLATE_EVENT(FOnNodeSelectionChangeSignature, OnNodeSelectionChange)
	SLATE_END_ARGS()

	/* Construct this widget. */
	void Construct(const FArguments& InArgs);

	//~ Begin SWidget Interface
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual FVector2D ComputeDesiredSize(float LayoutScaleMultiplier) const override;
	virtual bool SupportsKeyboardFocus() const override;
	//~ End SWidget Interface

private:
	//Draw functionality
	void PaintLines(const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32& DrawLayerId) const;
	void PaintAreas(const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32& DrawLayerId) const;
	void PaintNodes(const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32& DrawLayerId) const;

	/* Caches the node paint data. */
	void CacheNodeData(const FGeometry& AllottedGeometry);

	/* True if we have enough data for drawing the nodes. */
	bool CanPaintNodes() const;

	/* Checks if the given node data source has data on it. */
	bool NodeHasData(int32 NodeIdx) const;

	/* Translates normal space to slate's local space. */
	FVector2D FromNormalToSlateSpace(const FVector2D NormalSpace, const FGeometry& AllottedGeometry) const;

	/* Translates slate's local space to normal space. */
	FVector2D FromSlateToNormalSpace(const FVector2D LocalSpace, const FGeometry& AllottedGeometry) const;

	/* Obtains the area in which the current point sits. */
	int32 GetDirectionalAreaForPoint(const FVector2D& LocalPosition, const FGeometry& AllottedGeometry) const;

	/* Same as GertDirectionalAreaForPoint but returns an invalid index if the mouse is outside the polygon. */
	int32 GetDirectionalAreaForPointClamped(const FVector2D& LocalPosition, const FGeometry& AllottedGeometry) const;

	/* Modifies the given position to be clamped inside the polygon, returns whether it was clamped or not. */
	bool ClampPosition(FVector2D& InOutLocalPosition, const FGeometry& AllottedGeometry, bool bForceEdge = false) const;

	/* Called to set a new node and area as the selected one. */
	void SetSelectedNode(int32 NewSelection);

	/* Updates the preview area index to match the area under the mouse. */
	void UpdatePreviewAreaToMousePosition(const FGeometry& AllottedGeometry);

	/* Tooltip related. */
	FText GetTooltipNodeTitle() const;
	FText GetTooltipNodeDetail() const;
	void ShowToolTip();
	void ResetToolTip();

private:
	//Delegates
	/* Delegate to check if the given node has data or is empty. */
	FOnNodeHasDataSignature OnNodeHasData;

	/* Delegate called when the preview node area changes. */
	FOnNodeSelectionChangeSignature OnPreviewAreaChange;

	/* Delegate called when the node selection changes. */
	FOnNodeSelectionChangeSignature OnNodeSelectionChange;

	/* Drawing data. */
	const FSlateBrush* BackgroundImage;
	const FSlateBrush* NodeBrush;
	const FSlateBrush* DirectionalAreaBrush;
	FLinearColor GridLinesColor;
	FLinearColor NodeLinesColor;
	FLinearColor GraphOutlineColor;
	FColor HightlightAreaColor;
	FColor SelectedAreaColor;
	FVector2D NodeSize;
	float Margin;
	bool bDrawAngleOffset;
	bool bMovingPreviewNode;
	float PreviewCrosshairSize;

	/* State specific colors. */
	FSlateColor HighlightNodeColor;
	FSlateColor NodeColor;
	FSlateColor SelectNodeColor;
	FSlateColor PreviewNodeColor;

	/* Default values for attributes. */
	int32 DefaultNumNodes;
	float DefaultAngleOffset;

	/* Cached data for drawing. */
	TArray<FVector2D> CachedNodeLocations;
	TArray<FVector2D> CachedMidPointLocations;

	/* Number of directional nodes we're representing. */
	TAttribute<int32> NumNodesAttribute;

	/* Directional offset from the top onto the first node, in degrees. */
	TAttribute<float> AngleOffsetAttribute;

	/* The radius in local space in which to draw the nodes. */
	float DrawRadius;

	/* If true, we show the tooltip while hovering nodes or dragging the preview node. */
	bool bShowTooltip;

	/* Index of the selected node. */
	int32 SelectedNodeIndex;

	/* Index of the node being hovered (including its zone). */
	int32 HoveredNodeIndex;

	/* Coordinates for the preview node, in normal space. */
	FVector2D PreviewNodeNormalizedLocation;

	/* Area that the preview node is currently inhabiting. */
	int32 PreviewAreaIndex;

	/* Mouse coordinates in local-space. */
	FVector2D MousePosition;

	/** Tooltip ptr which is shown when hovering/dropping/dragging a sample*/
	TSharedPtr<SToolTip> ToolTip;
};
