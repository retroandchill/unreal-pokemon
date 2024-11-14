// Copyright 2017 ~ 2022 Critical Failure Studio Ltd. All rights reserved.

#include "Editors/Slate/SPaperZDAnimDataSourceGrid.h"
#include "Editors/Util/PaperZDEditorStyle.h"
#include "Rendering/DrawElements.h"
#include "Styling/AppStyle.h"
#include "Settings/EditorStyleSettings.h" 	
#include "Widgets/SBoxPanel.h" 	
#include "Widgets/Text/STextBlock.h" 	
#include "Framework/Application/SlateApplication.h"

#define MIN_NUM_NODES 3
#define MAX_NUM_NODES 16

#define LOCTEXT_NAMESPACE "ZDAnimDataSourceGrid"

void SPaperZDAnimDataSourceGrid::Construct(const FArguments& InArgs)
{
	DefaultNumNodes = 4;
	DefaultAngleOffset = 0.0f;
	SelectedNodeIndex = InArgs._InitialSelection;
	HoveredNodeIndex = INDEX_NONE;
	NodeSize = FVector2D(12.0f, 12.0f);
	PreviewNodeNormalizedLocation = FVector2D::ZeroVector;
	Margin = 10.0f;
	bDrawAngleOffset = false;
	bMovingPreviewNode = false;
	PreviewCrosshairSize = 3.5f;
	SetCursor(EMouseCursor::Crosshairs);

	//Obtain the data
	NumNodesAttribute = InArgs._NumNodes;
	AngleOffsetAttribute = InArgs._AngleOffset;
	DrawRadius = FMath::Clamp(InArgs._DrawRadius, 0.25f, 0.9f);
	bShowTooltip = InArgs._ShowTooltip;
	OnNodeHasData = InArgs._OnNodeHasData;
	OnPreviewAreaChange = InArgs._OnPreviewAreaChange;
	OnNodeSelectionChange = InArgs._OnNodeSelectionChange;

	//Get brushes and required images
	BackgroundImage = FAppStyle::GetBrush("Graph.Panel.SolidBackground");
	NodeBrush = FAppStyle::GetBrush("CurveEd.CurveKey");
	DirectionalAreaBrush = FPaperZDEditorStyle::Get().GetBrush("PaperZDEditor.WhiteBrush");

	// Retrieve UI color values
	NodeColor = FAppStyle::GetSlateColor("BlendSpaceKey.Regular");
	HighlightNodeColor = FAppStyle::GetSlateColor("BlendSpaceKey.Highlight");
	SelectNodeColor = FAppStyle::GetSlateColor("BlendSpaceKey.Pressed");
	PreviewNodeColor = FAppStyle::GetSlateColor("BlendSpaceKey.Preview");
	GridLinesColor = GetDefault<UEditorStyleSettings>()->RegularColor;
	GraphOutlineColor = GetDefault<UEditorStyleSettings>()->RuleColor;
	NodeLinesColor = FLinearColor(0.25f, 0.25f, 0.25f);
	HightlightAreaColor = FColor(75, 75, 75);
	SelectedAreaColor = SelectNodeColor.GetSpecifiedColor().ToFColor(true).WithAlpha(75);
	
	SAssignNew(ToolTip, SToolTip)
	.BorderImage(FCoreStyle::Get().GetBrush("ToolTip.BrightBackground"))
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		[
			SNew(STextBlock)
			.Text(this, &SPaperZDAnimDataSourceGrid::GetTooltipNodeTitle)
			.Font(FCoreStyle::Get().GetFontStyle("ToolTip.LargerFont"))
			.ColorAndOpacity(FLinearColor::Black)
		]

		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(STextBlock)
			.Text(this, &SPaperZDAnimDataSourceGrid::GetTooltipNodeDetail)
			.Font(FCoreStyle::Get().GetFontStyle("ToolTip.LargerFont"))
			.ColorAndOpacity(FLinearColor::Black)
		]
	];

// 	ChildSlot
// 	[
// 			
// 	];
}

void SPaperZDAnimDataSourceGrid::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);
	CacheNodeData(AllottedGeometry);
	HoveredNodeIndex = GetDirectionalAreaForPointClamped(MousePosition, AllottedGeometry);

	//Update preview node if needed
	if (bMovingPreviewNode || !CachedNodeLocations.IsValidIndex(PreviewAreaIndex))
	{
		UpdatePreviewAreaToMousePosition(AllottedGeometry);
	}
}

FReply SPaperZDAnimDataSourceGrid::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::LeftShift)
	{
		bMovingPreviewNode = true; 
		ShowToolTip();
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

FReply SPaperZDAnimDataSourceGrid::OnKeyUp(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::LeftShift)
	{
		bMovingPreviewNode = false; 
		ResetToolTip();
		return FReply::Handled();
	}

	return FReply::Unhandled();
}

FReply SPaperZDAnimDataSourceGrid::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	MousePosition = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
	return FReply::Handled();
}

FReply SPaperZDAnimDataSourceGrid::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (HoveredNodeIndex != INDEX_NONE)
	{
		SetSelectedNode(HoveredNodeIndex);
		UpdatePreviewAreaToMousePosition(MyGeometry);
	}

	return FReply::Handled();
}

void SPaperZDAnimDataSourceGrid::CacheNodeData(const FGeometry& AllottedGeometry)
{
	const int32 NumNodes = FMath::Min(NumNodesAttribute.Get(DefaultNumNodes), MAX_NUM_NODES);
	CachedNodeLocations.Empty(NumNodes);
	CachedMidPointLocations.Empty(NumNodes);

	//No sense creating a directional map if we don't have enough nodes
	if (NumNodes >= MIN_NUM_NODES)
	{
		//Add first node 
		const float AngleOffset = AngleOffsetAttribute.Get(DefaultAngleOffset);
		const float AngleSepparation = -360.0f / NumNodes;
		const FVector2D TopNormalized = FVector2D(0.0f, DrawRadius - Margin / AllottedGeometry.Size.GetMin()).GetRotated(AngleOffset);

		//Setup nodes in Geometry space
		for (int32 i = 0; i < NumNodes; i++)
		{
			const FVector2D NormNode = TopNormalized.GetRotated(AngleSepparation * i);
			CachedNodeLocations.Add(FromNormalToSlateSpace(NormNode, AllottedGeometry));
		}

		//Create mid points
		for (int32 i = 0; i < NumNodes; i++)
		{
			const FVector2D& PrevPoint = CachedNodeLocations[i];
			const FVector2D& NextPoint = CachedNodeLocations[(i + 1) % NumNodes];
			CachedMidPointLocations.Add((PrevPoint + NextPoint) / 2.0f);
		}
	}
}

bool SPaperZDAnimDataSourceGrid::CanPaintNodes() const
{
	return CachedNodeLocations.Num() > 0;
}

bool SPaperZDAnimDataSourceGrid::NodeHasData(int32 NodeIdx) const
{
	//We assume the nodes have data for clarity of drawing if unspecified
	return OnNodeHasData.IsBound() ? OnNodeHasData.Execute(NodeIdx) : true;
}

FVector2D SPaperZDAnimDataSourceGrid::FromNormalToSlateSpace(const FVector2D NormalSpace, const FGeometry& AllottedGeometry) const
{
	const FVector2D Center = AllottedGeometry.Size / 2.0f;
	const float GeometrySize = AllottedGeometry.Size.GetMin() / 2.0f;
	return NormalSpace * GeometrySize * FVector2D(1.0f, -1.0f) + Center; //Need to reverse the Y axis
}

FVector2D SPaperZDAnimDataSourceGrid::FromSlateToNormalSpace(const FVector2D LocalSpace, const FGeometry& AllottedGeometry) const
{
	const FVector2D Center = AllottedGeometry.Size / 2.0f;
	const float GeometrySize = AllottedGeometry.Size.GetMin() / 2.0f;
	return (LocalSpace - Center) / GeometrySize * FVector2D(1.0f, -1.0f); //Need to reverse the Y axis
}

int32 SPaperZDAnimDataSourceGrid::GetDirectionalAreaForPoint(const FVector2D& LocalPosition, const FGeometry& AllottedGeometry) const
{
	if (CachedNodeLocations.Num() > 0)
	{
		const FVector2D TopPosition(0.0f, 1.0f);
		const FVector2D NormalPosition = FromSlateToNormalSpace(LocalPosition, AllottedGeometry);
		const float Sign = NormalPosition.X != 0.0f ? FMath::Sign(NormalPosition.X) : 1.0f;
		const float AngleRad = FMath::Acos(NormalPosition.GetSafeNormal() | TopPosition) * Sign;
		const float AngleSepparation = 360.0f / CachedNodeLocations.Num();
		const int32 Area = (FMath::RadiansToDegrees(AngleRad) + AngleOffsetAttribute.Get(DefaultAngleOffset) + AngleSepparation / 2.0f + 360.0f) / AngleSepparation;
		return Area % CachedNodeLocations.Num();
	}

	return INDEX_NONE;
}

int32 SPaperZDAnimDataSourceGrid::GetDirectionalAreaForPointClamped(const FVector2D& LocalPosition, const FGeometry& AllottedGeometry) const
{
	FVector2D NonConstPosition = LocalPosition;
	if (!ClampPosition(NonConstPosition, AllottedGeometry))
	{
		return GetDirectionalAreaForPoint(LocalPosition, AllottedGeometry);
	}

	return INDEX_NONE;
}

struct FLine2D
{
	FVector2D Position;
	FVector2D Direction;

	FLine2D(FVector2D PointA, FVector2D PointB)
	 : Position(PointA)
	{
		Direction = (PointB - PointA).GetSafeNormal();
	}
};

inline float CrossProduct2D(FVector2D Direction0, FVector2D Direction1)
{
	// Also the parallelogram area
	return Direction0.X * Direction1.Y - Direction0.Y * Direction1.X;
}

bool ComputePointIntersectionBetweenLines2D(const FLine2D& Line0, const FLine2D& Line1, FVector2D& OutIntersectPoint)
{
	float d = CrossProduct2D(Line0.Direction, Line1.Direction);

	// Parallel case
	if (FMath::Abs(d) < SMALL_NUMBER)
	{
		return false;
	}

	float t = CrossProduct2D(Line1.Direction, Line0.Position - Line1.Position) / d;

	// Intersects in the wrong direction
	if (t < 0.5f)
	{
		return false;
	}

	OutIntersectPoint = Line0.Position + t * Line0.Direction;
	return true;
}

bool SPaperZDAnimDataSourceGrid::ClampPosition(FVector2D& InOutLocalPosition, const FGeometry& AllottedGeometry, bool bForceEdge /* = false */) const
{
	if (CachedNodeLocations.Num() > 0)
	{
		//Obtain the index of the node in which the intersection line begins (this is not the area, but the node itself)
		const FVector2D TopPosition(0.0f, 1.0f);
		const FVector2D NormalPosition = FromSlateToNormalSpace(InOutLocalPosition, AllottedGeometry);
		const float Sign = NormalPosition.X != 0.0f ? FMath::Sign(NormalPosition.X) : 1.0f;
		const float AngleRad = FMath::Acos(NormalPosition.GetSafeNormal() | TopPosition) * Sign;
		const float AngleSepparation = 360.0f / CachedNodeLocations.Num();
		int32 NodeIndex = (FMath::RadiansToDegrees(AngleRad) + AngleOffsetAttribute.Get(DefaultAngleOffset)+ 360.0f) / AngleSepparation;
		NodeIndex = NodeIndex % CachedNodeLocations.Num();

		const FVector2D& PointA = CachedNodeLocations[NodeIndex];
		const FVector2D& PointB = CachedNodeLocations[(NodeIndex + 1) % CachedNodeLocations.Num()];

		//Create 2 line objects for making calculations easier
		const FVector2D Center = AllottedGeometry.Size / 2.0f;
		FLine2D RadialLine(InOutLocalPosition, Center);
		FLine2D PolygonLine(PointA, PointB);

		FVector2D OutIntersectPoint;
		if (ComputePointIntersectionBetweenLines2D(PolygonLine, RadialLine, OutIntersectPoint))
		{
			if (FromSlateToNormalSpace(OutIntersectPoint, AllottedGeometry).SizeSquared() < FromSlateToNormalSpace(InOutLocalPosition, AllottedGeometry).SizeSquared())
			{
				InOutLocalPosition = OutIntersectPoint;
				return true;
			}
			else if (bForceEdge)
			{
				InOutLocalPosition = OutIntersectPoint;
			}
		}
	}

	return false;
}

void SPaperZDAnimDataSourceGrid::SetSelectedNode(int32 NewSelection)
{
	if (SelectedNodeIndex != NewSelection)
	{
		SelectedNodeIndex = NewSelection;
		OnNodeSelectionChange.ExecuteIfBound(SelectedNodeIndex);
	}
}

void SPaperZDAnimDataSourceGrid::UpdatePreviewAreaToMousePosition(const FGeometry& AllottedGeometry)
{
	const float AngleOffset = AngleOffsetAttribute.Get(DefaultAngleOffset);
	PreviewNodeNormalizedLocation = FromSlateToNormalSpace(MousePosition, AllottedGeometry).GetRotated(-AngleOffset);
	PreviewAreaIndex = GetDirectionalAreaForPoint(MousePosition, AllottedGeometry);
	OnPreviewAreaChange.ExecuteIfBound(PreviewAreaIndex);
}

FText SPaperZDAnimDataSourceGrid::GetTooltipNodeTitle() const
{
	if (bMovingPreviewNode)
	{
		FFormatNamedArguments Args;
		const float AngleOffset = AngleOffsetAttribute.Get(DefaultAngleOffset);
		const FVector2D Direction = PreviewNodeNormalizedLocation.GetRotated(AngleOffset).GetSafeNormal();
		Args.Add(TEXT("CoordX"), Direction.X);
		Args.Add(TEXT("CoordY"), Direction.Y);
		return FText::Format(LOCTEXT("DragPreviewNodeTitle", "Previewing Direction: ({CoordX}, {CoordY})."), Args);
	}
// 	else if (HoveredNodeIndex != INDEX_NONE)
// 	{
// 	}
	return FText::FromName(NAME_None);
}

FText SPaperZDAnimDataSourceGrid::GetTooltipNodeDetail() const
{
	if (bMovingPreviewNode)
	{
		FFormatNamedArguments Args;
		Args.Add(TEXT("Area"), PreviewAreaIndex);
		return FText::Format(LOCTEXT("DragPreviewNodeDetail", "Resulting area index: {Area}."), Args);
	}
	// 	else if (HoveredNodeIndex != INDEX_NONE)
	// 	{
	// 	}

	return FText::FromName(NAME_None);
}

void SPaperZDAnimDataSourceGrid::ShowToolTip()
{
	if (bShowTooltip)
	{
		SetToolTip(ToolTip);
	}
}

void SPaperZDAnimDataSourceGrid::ResetToolTip()
{
	SetToolTip(nullptr);
}

int32 SPaperZDAnimDataSourceGrid::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	// Fill the background
	FSlateDrawElement::MakeBox(OutDrawElements, LayerId + 1, AllottedGeometry.ToPaintGeometry(), BackgroundImage);

	//Lines and everything else require enough node data
	if (CanPaintNodes())
	{
		PaintAreas(AllottedGeometry, MyCullingRect, OutDrawElements, ++LayerId);
		PaintLines(AllottedGeometry, MyCullingRect, OutDrawElements, ++LayerId);
		PaintNodes(AllottedGeometry, MyCullingRect, OutDrawElements, ++LayerId);
	}
	
	return LayerId;
}

void SPaperZDAnimDataSourceGrid::PaintLines(const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32& DrawLayerId) const
{
	//Paint the radial lines
	const FVector2D CenterPoint = AllottedGeometry.Size / 2.0f;
	for (const FVector2D& MidPoint : CachedMidPointLocations)
	{
		TArray<FVector2D> LinePoints;
		LinePoints.Reserve(2);
		LinePoints.Add(CenterPoint);
		LinePoints.Add(MidPoint);
		FSlateDrawElement::MakeLines(OutDrawElements, DrawLayerId, AllottedGeometry.ToPaintGeometry(), LinePoints, ESlateDrawEffect::None, GridLinesColor, true);
	}

	//Draw the lines that connect each node, adding an extra one for closing the loop
	{
		TArray<FVector2D> LinePoints = CachedNodeLocations;
		LinePoints.Add(CachedNodeLocations[0]);
		FSlateDrawElement::MakeLines(OutDrawElements, DrawLayerId, AllottedGeometry.ToPaintGeometry(), LinePoints, ESlateDrawEffect::None, NodeLinesColor, true);
	}

	//Paint the lines that are born from the preview node
	if(!PreviewNodeNormalizedLocation.IsNearlyZero(0.05f))
	{
		//We force clamp the preview node onto the edge of the polygon for displaying the node
		const float AngleOffset = AngleOffsetAttribute.Get(DefaultAngleOffset);
		FVector2D PreviewEdgePosition = FromNormalToSlateSpace(PreviewNodeNormalizedLocation.GetRotated(AngleOffset), AllottedGeometry);
		ClampPosition(PreviewEdgePosition, AllottedGeometry, true);

		TArray<FVector2D> LinePoints;
		LinePoints.Reserve(2);
		LinePoints.Add(CenterPoint);
		LinePoints.Add(PreviewEdgePosition);

		FLinearColor Color = PreviewNodeColor.GetSpecifiedColor();
		Color.A = 0.25f;
		FSlateDrawElement::MakeLines(OutDrawElements, DrawLayerId, AllottedGeometry.ToPaintGeometry(), LinePoints, ESlateDrawEffect::None, Color, true);

		//Draw a small crosshair on the edge
		if (bMovingPreviewNode)
		{
			Color.A = 1.0f;
			LinePoints.Empty(2);
			LinePoints.Add(FVector2D(PreviewEdgePosition.X - PreviewCrosshairSize, PreviewEdgePosition.Y - PreviewCrosshairSize));
			LinePoints.Add(FVector2D(PreviewEdgePosition.X + PreviewCrosshairSize, PreviewEdgePosition.Y + PreviewCrosshairSize));
			FSlateDrawElement::MakeLines(OutDrawElements, DrawLayerId, AllottedGeometry.ToPaintGeometry(), LinePoints, ESlateDrawEffect::None, Color, true);

			LinePoints.Empty(2);
			LinePoints.Add(FVector2D(PreviewEdgePosition.X - PreviewCrosshairSize, PreviewEdgePosition.Y + PreviewCrosshairSize));
			LinePoints.Add(FVector2D(PreviewEdgePosition.X + PreviewCrosshairSize, PreviewEdgePosition.Y - PreviewCrosshairSize));
			FSlateDrawElement::MakeLines(OutDrawElements, DrawLayerId, AllottedGeometry.ToPaintGeometry(), LinePoints, ESlateDrawEffect::None, Color, true);
		}
	}

	//Paint the margin
	{
		TArray<FVector2D> LinePoints;
		LinePoints.Reserve(5);
		LinePoints.Add(FVector2D(Margin, Margin));
		LinePoints.Add(FVector2D(AllottedGeometry.Size.X - Margin, Margin));
		LinePoints.Add(FVector2D(AllottedGeometry.Size.X - Margin, AllottedGeometry.Size.Y - Margin));
		LinePoints.Add(FVector2D(Margin, AllottedGeometry.Size.Y - Margin));
		LinePoints.Add(FVector2D(Margin, Margin));
		FSlateDrawElement::MakeLines(OutDrawElements, DrawLayerId, AllottedGeometry.ToPaintGeometry(), LinePoints, ESlateDrawEffect::None, GraphOutlineColor, true);
	}
}

void SPaperZDAnimDataSourceGrid::PaintAreas(const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32& DrawLayerId) const
{
	//Paint the selected area, but not if its already selected
	if (CachedNodeLocations.IsValidIndex(SelectedNodeIndex))
	{
		TArray<FSlateVertex> Verts;
		Verts.Reserve(4);

		//Add the points: center, node, next mid-point, prev mid-point
		const FSlateRenderTransform& SlateRenderTransform = AllottedGeometry.ToPaintGeometry().GetAccumulatedRenderTransform();
		Verts.Add(FSlateVertex::Make<ESlateVertexRounding::Disabled>(SlateRenderTransform, FVector2f(AllottedGeometry.Size / 2.0f), FVector2f(0.0f, 1.0f), SelectedAreaColor)); //center
		Verts.Add(FSlateVertex::Make<ESlateVertexRounding::Disabled>(SlateRenderTransform, FVector2f(CachedNodeLocations[SelectedNodeIndex]), FVector2f(0.0f, 1.0f), SelectedAreaColor)); //node
		Verts.Add(FSlateVertex::Make<ESlateVertexRounding::Disabled>(SlateRenderTransform, FVector2f(CachedMidPointLocations[SelectedNodeIndex]), FVector2f(0.0f, 1.0f), SelectedAreaColor)); //next mid-point
		
		const int32 PrevMidPointIndex = (SelectedNodeIndex - 1 + CachedMidPointLocations.Num()) % CachedMidPointLocations.Num();
		Verts.Add(FSlateVertex::Make<ESlateVertexRounding::Disabled>(SlateRenderTransform, FVector2f(CachedMidPointLocations[PrevMidPointIndex]), FVector2f(0.0f, 1.0f), SelectedAreaColor)); //prev mid-point

		//Create indices
		TArray<SlateIndex> Indices = { 0 , 1 , 2 , 0 , 1 , 3 };

		//Draw
		const FSlateResourceHandle& ResourceHandle = FSlateApplication::Get().GetRenderer()->GetResourceHandle(*DirectionalAreaBrush);
		FSlateDrawElement::MakeCustomVerts(OutDrawElements, DrawLayerId, ResourceHandle, Verts, Indices, nullptr, 0, 0, ESlateDrawEffect::None);
	}

	//Paint the hovered area, but not if its already selected
	const int32 CursorArea = HoveredNodeIndex;
	if (CachedNodeLocations.IsValidIndex(CursorArea))
	{
		TArray<FSlateVertex> Verts;
		Verts.Reserve(4);

		//If the node is also selected, bleed a little so we can see the selected color as well
		FColor DrawColor = HightlightAreaColor;
		if (HoveredNodeIndex == SelectedNodeIndex)
		{
			DrawColor.A = 175;
		}

		//Add the points: center, node, next mid-point, prev mid-point
		const FSlateRenderTransform& SlateRenderTransform = AllottedGeometry.ToPaintGeometry().GetAccumulatedRenderTransform();
		Verts.Add(FSlateVertex::Make<ESlateVertexRounding::Disabled>(SlateRenderTransform, FVector2f(AllottedGeometry.Size / 2.0f), FVector2f(0.0f, 1.0f), DrawColor)); //center
		Verts.Add(FSlateVertex::Make<ESlateVertexRounding::Disabled>(SlateRenderTransform, FVector2f(CachedNodeLocations[CursorArea]), FVector2f(0.0f, 1.0f), DrawColor)); //node
		Verts.Add(FSlateVertex::Make<ESlateVertexRounding::Disabled>(SlateRenderTransform, FVector2f(CachedMidPointLocations[CursorArea]), FVector2f(0.0f, 1.0f), DrawColor)); //next mid-point

		const int32 PrevMidPointIndex = (CursorArea - 1 + CachedMidPointLocations.Num()) % CachedMidPointLocations.Num();
		Verts.Add(FSlateVertex::Make<ESlateVertexRounding::Disabled>(SlateRenderTransform, FVector2f(CachedMidPointLocations[PrevMidPointIndex]), FVector2f(0.0f, 1.0f), DrawColor)); //prev mid-point

		//Create indices
		TArray<SlateIndex> Indices = { 0 , 1 , 2 , 0 , 1 , 3 };

		//Draw
		const FSlateResourceHandle& ResourceHandle = FSlateApplication::Get().GetRenderer()->GetResourceHandle(*DirectionalAreaBrush);
		FSlateDrawElement::MakeCustomVerts(OutDrawElements, DrawLayerId, ResourceHandle, Verts, Indices, nullptr, 0, 0, ESlateDrawEffect::None);
	}
}

void SPaperZDAnimDataSourceGrid::PaintNodes(const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32& DrawLayerId) const
{
	//Paint nodes
	for (int32 i = 0; i < CachedNodeLocations.Num(); i++)
	{
		FLinearColor DrawColor = NodeColor.GetSpecifiedColor();
		if (i == SelectedNodeIndex)
		{
			DrawColor = SelectNodeColor.GetSpecifiedColor();
		}
		else if (i == HoveredNodeIndex)
		{
			DrawColor = HighlightNodeColor.GetSpecifiedColor();
		}

		//Modify the node painting if they don't hold any data
		if (!NodeHasData(i))
		{
			DrawColor.A = 0.35f;
		}

		FVector2D GridPosition = CachedNodeLocations[i] - (NodeSize * 0.5f);
		FSlateDrawElement::MakeBox(OutDrawElements, DrawLayerId + 1, AllottedGeometry.ToPaintGeometry(NodeSize, FSlateLayoutTransform(GridPosition)), NodeBrush, ESlateDrawEffect::None, DrawColor);
	}

	//Paint the preview node
	{
		const float AngleOffset = AngleOffsetAttribute.Get(DefaultAngleOffset);
		FVector2D GridPosition = FromNormalToSlateSpace(PreviewNodeNormalizedLocation.GetRotated(AngleOffset), AllottedGeometry);
		ClampPosition(GridPosition, AllottedGeometry);
		GridPosition -= NodeSize * 0.5f;

		FSlateDrawElement::MakeBox(OutDrawElements, DrawLayerId + 1, AllottedGeometry.ToPaintGeometry(NodeSize, FSlateLayoutTransform(GridPosition)), NodeBrush, ESlateDrawEffect::None, PreviewNodeColor.GetSpecifiedColor());
	}
}

FVector2D SPaperZDAnimDataSourceGrid::ComputeDesiredSize(float LayoutScaleMultiplier) const
{
	return FVector2D(250.0f, 250.0f);
}

bool SPaperZDAnimDataSourceGrid::SupportsKeyboardFocus() const
{
	return true;
}

#undef LOCTEXT_NAMESPACE
